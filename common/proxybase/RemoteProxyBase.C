/*****************************************************************************
*
* Copyright (c) 2000 - 2012, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

#include <RemoteProxyBase.h>
#include <RemoteProcess.h>
#include <ExistingRemoteProcess.h>
#include <snprintf.h>

// ****************************************************************************
// Method: RemoteProxyBase::RemoteProxyBase
//
// Purpose: 
//   Constructor for the RemoteProxyBase class.
//
// Arguments:
//   compName : The name of the component.
//
// Programmer: Brad Whitlock
// Creation:   Fri May 2 14:56:19 PST 2003
//
// Modifications:
//  Brad Whitlock, Thu Mar 11 12:47:34 PDT 2004
//  Added KeepAliveRPC.
//
//  Brad Whitlock, Fri Dec  7 16:50:02 PST 2007
//  Added rpcSetup flag.
//
// ****************************************************************************

RemoteProxyBase::RemoteProxyBase(const std::string &compName) :
    componentName(compName), xfer(), quitRPC(), keepAliveRPC(),
    argv()
{
    component = 0;
    nWrite = nRead = 1;
    progressCallback = 0;
    progressCallbackData = 0;
    rpcSetup = false;
}

// ****************************************************************************
// Method: RemoteProxyBase::~RemoteProxyBase
//
// Purpose: 
//   Destructor for the RemoteProxyBase class.
//
// Programmer: Brad Whitlock
// Creation:   Fri May 2 14:56:48 PST 2003
//
// Modifications:
//   
// ****************************************************************************

RemoteProxyBase::~RemoteProxyBase()
{
    delete component;
}

// ****************************************************************************
// Method: RemoteProxyBase::Create
//
// Purpose: 
//   Creates the remote process and hooks up RPCs so that it can be used.
//
// Arguments:
//   profile         : The machine profile of the host where the proxy should 
//                     be created.
//   connectCallback : A callback that can be used to launch the component.
//   data            : Data to be used with the connect callback.
//
// Note:       
//
// Programmer: Brad Whitlock
// Creation:   Fri May 2 14:57:06 PST 2003
//
// Modifications:
//    Jeremy Meredith, Thu Oct  9 14:04:45 PDT 2003
//    Added ability to manually specify a client host name or to have it
//    parsed from the SSH_CLIENT (or related) environment variables.  Added
//    ability to specify an SSH port.
//
//    Brad Whitlock, Thu Mar 11 12:51:34 PDT 2004
//    I added keep alive RPC.
//
//    Jeremy Meredith, Thu May 24 10:20:57 EDT 2007
//    Added SSH tunneling argument; pass it along to RemoteProcess::Open.
//
//    Brad Whitlock, Fri Dec  7 16:50:17 PST 2007
//    Moved RPC creation into the SetupAllRPCs method.
//
//    Jeremy Meredith, Thu Feb 18 15:25:27 EST 2010
//    Split HostProfile int MachineProfile and LaunchProfile.
//
//    Eric Brugger, Mon May  2 16:42:59 PDT 2011
//    I added the ability to use a gateway machine when connecting to a
//    remote host.
//
//    Brad Whitlock, Tue Jun  5 15:57:00 PDT 2012
//    Pass in the MachineProfile.
//
// ****************************************************************************

void
RemoteProxyBase::Create(const MachineProfile &profile,
                        ConnectCallback *connectCallback,
                        void *connectCallbackData,
                        bool createAsThoughLocal)
{
    // Create a remote process object for the remote component.
    if(connectCallback == NULL)
    {
        component = new RemoteProcess(GetVisItString());
    }
    else
    {
        ExistingRemoteProcess *p = 
            new ExistingRemoteProcess(GetVisItString());
        p->SetConnectCallback(connectCallback);
        p->SetConnectCallbackData(connectCallbackData);
        component = p;
    }

    // Add the component's name as an extra argument.
    component->AddArgument(componentName);

    // Set the engine's launch progress callback.
    component->SetProgressCallback(progressCallback, progressCallbackData);

    // Add extra arguments to the remote process object.
    AddExtraArguments();

    //
    // Open the remote component.
    //
    component->Open(profile, nRead, nWrite, createAsThoughLocal);

    //
    // Hook up the sockets to the xfer object.
    //
    xfer.SetOutputConnection(component->GetReadConnection());
    xfer.SetInputConnection(component->GetWriteConnection());
    
    //
    // Set up the RPCs
    //
    SetupAllRPCs();

    //
    // List the objects that were hooked up. 
    //
    xfer.ListObjects();
}

// ****************************************************************************
// Method: RemoteProxyBase::SetupAllRPCs
//
// Purpose: 
//   Sets up all of the RPCs.
//
// Programmer: Brad Whitlock
// Creation:   Fri Dec  7 16:50:46 PST 2007
//
// Modifications:
//   
// ****************************************************************************

void
RemoteProxyBase::SetupAllRPCs()
{
    if(!rpcSetup)
    {
        xfer.Add(&quitRPC);
        xfer.Add(&keepAliveRPC);

        //
        // Set up the RPC's for the remote component.
        //
        SetupComponentRPCs();
        rpcSetup = true;
    }
}

// ****************************************************************************
// Method: RemoteProxyBase::GetXfer
//
// Purpose: 
//   Return a reference to Xfer
//
// Arguments:
//
// Returns:    A reference to Xfer.
//
// Note:       This method has the side-effect of setting up the RPCs.
//
// Programmer: Brad Whitlock
// Creation:   Fri Dec  7 16:57:38 PST 2007
//
// Modifications:
//   
// ****************************************************************************

const Xfer &
RemoteProxyBase::GetXfer()
{
    SetupAllRPCs();
    return xfer;
}

// ****************************************************************************
// Method: RemoteProxyBase::Close
//
// Purpose: 
//   Tells the remote component to close.
//
// Programmer: Brad Whitlock
// Creation:   Fri May 2 14:59:06 PST 2003
//
// Modifications:
//   
// ****************************************************************************

void
RemoteProxyBase::Close()
{
    quitRPC(true);
}

// ****************************************************************************
// Method: RemoteProxyBase::SendKeepAlive
//
// Purpose: 
//   Sends a keep alive RPC to the component.
//
// Note:       This method could throw an exception if the connection to the
//             component has been lost.
//
// Programmer: Brad Whitlock
// Creation:   Fri Mar 12 10:11:02 PDT 2004
//
// Modifications:
//   
// ****************************************************************************

void
RemoteProxyBase::SendKeepAlive()
{
    keepAliveRPC();
}

// ****************************************************************************
// Method: RemoteProxyBase::SetProgressCallback
//
// Purpose: 
//   This method sets a progress callback that is called while the remote
//   component is being launched.
//
// Arguments:
//   cb   : The progress callback.
//   data : Data to pass to the progress callback.
//
// Programmer: Brad Whitlock
// Creation:   Fri May 2 14:59:52 PST 2003
//
// Modifications:
//   
// ****************************************************************************

void
RemoteProxyBase::SetProgressCallback(bool (*cb)(void *, int), void *data)
{
    progressCallback = cb;
    progressCallbackData = data;
}

// ****************************************************************************
// Method: RemoteProxyBase::AddArgument
//
// Purpose: 
//   Adds an argument to the list of arguments to be passed to the component.
//
// Arguments:
//   arg : The argument to be added to the list.
//
// Programmer: Brad Whitlock
// Creation:   Fri May 2 15:00:55 PST 2003
//
// Modifications:
//   
// ****************************************************************************

void
RemoteProxyBase::AddArgument(const std::string &arg)
{
    argv.push_back(arg);
}

// ****************************************************************************
//  Method:  RemoteProxyBase::AddProfileArguments
//
//  Purpose:
//    Adds the appropriate arguments to a remote proxy.
//
//  Arguments:
//    profile         : the host profile used to set the arguments.
//    addParallelArgs : true if this process is going to launch itself
//                      in parallel, and false if the vcl has already
//                      created a parallel job and we just need to
//                      choose the parallel engine when needed
//
//  Programmer:  Jeremy Meredith
//  Creation:    June 26, 2003
//
//  Modifications:
//    Brad Whitlock, Wed Aug 4 17:40:57 PST 2004
//    Moved this method from ViewerRemoteProcessChooser and changed the code
//    a little.
//
//    Brad Whitlock, Tue Aug 10 17:16:55 PST 2004
//    Undefined a macro on Windows because it interferes with HostProfile.
//
//    Jeremy Meredith, Thu Sep 15 16:34:15 PDT 2005
//    Added ability to launch visit script under mpirun in order to set up
//    the environment on beowulf clusters (and similar).
//
//    Hank Childs, Sat Dec  3 20:17:07 PST 2005
//    Added argument for hardware acceleration.
//
//    Eric Brugger, Thu Feb 15 12:04:45 PST 2007
//    Added an argument for passing additional sublauncher arguments.
//
//    Dave Bremer, Wed Apr 16 17:54:14 PDT 2008
//    Added arguments for passing in commands to run pre and post the 
//    sublauncher command.
//
//    Jeremy Meredith, Wed Apr 30 12:22:01 EDT 2008
//    Always add -noloopback for parallel launches, since they may
//    occur on compute nodes.
//
//    Jeremy Meredith, Thu Feb 18 15:25:27 EST 2010
//    Split HostProfile int MachineProfile and LaunchProfile.
//    Added a new "directory" argument outside of the arbitrary args.
//
//    Jeremy Meredith, Fri Feb 19 09:55:03 EST 2010
//    Remove assumption that the machine profile has an active launch profile.
//
//    Mark C. Miller, Tue Oct 19 21:42:22 PDT 2010
//    Name of '-timeout' argument was changed to '-idle-timeout'.
//
//    Tom Fogal, Fri May  6 18:34:52 MDT 2011
//    Adjust for new X launch options.
//
//    Hank Childs, Fri Nov 18 07:23:04 PST 2011
//    Make sure -display gets added whenever we do HW acceleration (not just
//    when we launch x-servers).
//
// ****************************************************************************

void
RemoteProxyBase::AddProfileArguments(const MachineProfile &machine,
                                     bool addParallelArgs)
{
    // Add the directory arugment
    if (machine.GetDirectory() != "")
    {
        AddArgument("-dir");
        AddArgument(machine.GetDirectory());
    }

    const LaunchProfile *launch = machine.GetActiveLaunchProfile();
    if (!launch)
        return;

    //
    // Add the parallel arguments.
    //
    if (launch->GetParallel())
    {
        AddArgument("-noloopback");

        char temp[10];
        if (!addParallelArgs)
        {
            AddArgument("-par");
        }

        if (addParallelArgs)
        {
            SNPRINTF(temp, 10, "%d", launch->GetNumProcessors());
            AddArgument("-np");
            AddArgument(temp);
        }
        SetNumProcessors(launch->GetNumProcessors());

        if (launch->GetNumNodesSet() &&
            launch->GetNumNodes() > 0)
        {
            if (addParallelArgs)
            {
                SNPRINTF(temp, 10, "%d", launch->GetNumNodes());
                AddArgument("-nn");
                AddArgument(temp);
            }
            SetNumNodes(launch->GetNumNodes());
        }

        if (addParallelArgs)
        {
            if (launch->GetPartitionSet() &&
                launch->GetPartition().length() > 0)
            {
                AddArgument("-p");
                AddArgument(launch->GetPartition());
            }

            if (launch->GetBankSet() &&
                launch->GetBank().length() > 0)
            {
                AddArgument("-b");
                AddArgument(launch->GetBank());
            }

            if (launch->GetTimeLimitSet() &&
                launch->GetTimeLimit().length() > 0)
            {
                AddArgument("-t");
                AddArgument(launch->GetTimeLimit());
            }

            if (launch->GetLaunchMethodSet() &&
                launch->GetLaunchMethod().length() > 0)
            {
                AddArgument("-l");
                AddArgument(launch->GetLaunchMethod());
            }

            if (launch->GetLaunchArgsSet() &&
                launch->GetLaunchArgs().length() > 0)
            {
                AddArgument("-la");
                AddArgument(launch->GetLaunchArgs());
            }
        }
            if (launch->GetSublaunchArgsSet() &&
                launch->GetSublaunchArgs().length() > 0)
            {
                AddArgument("-sla");
                AddArgument(launch->GetSublaunchArgs());
            }

            if (launch->GetSublaunchPreCmdSet() &&
                launch->GetSublaunchPreCmd().length() > 0)
            {
                AddArgument("-slpre");
                AddArgument(launch->GetSublaunchPreCmd());
            }

            if (launch->GetSublaunchPostCmdSet() &&
                launch->GetSublaunchPostCmd().length() > 0)
            {
                AddArgument("-slpost");
                AddArgument(launch->GetSublaunchPostCmd());
            }

            if (launch->GetMachinefileSet() &&
                launch->GetMachinefile().length() > 0)
            {
                AddArgument("-machinefile");
                AddArgument(launch->GetMachinefile());
            }

            if (launch->GetVisitSetsUpEnv())
            {
                AddArgument("-setupenv");
            }
        //}
#if 0 // disabling dynamic load balancing for now
        if (launch->GetForceStatic())
        {
            if (addParallelArgs)
            {
                AddArgument("-forcestatic");
            }
            SetLoadBalancing(0);
        }

        if (launch->GetForceDynamic())
        {
            if (addParallelArgs)
            {
                AddArgument("-forcedynamic");
            }
            SetLoadBalancing(1);
        }
#else
        // force all static until speed issues are resolved
        if (addParallelArgs)
        {
            AddArgument("-forcestatic");
        }
        SetLoadBalancing(0);
#endif
    }

    if (launch->GetCanDoHWAccel())
    {
        AddArgument("-hw-accel");
        if(launch->GetLaunchXServers())
        {
            AddArgument("-launch-x");
            if(!launch->GetXArguments().empty())
            {
                AddArgument("-x-args");
                AddArgument(launch->GetXArguments());
            }
        }
        if(!launch->GetXDisplay().empty())
        {
            AddArgument("-display");
            AddArgument(launch->GetXDisplay());
        }
        AddArgument("-n-gpus-per-node");
        {
            std::ostringstream gn;
            gn << launch->GetGPUsPerNode();
            AddArgument(gn.str());
        }
    }

    // Add the timeout argument
    char temp[10];
    SNPRINTF(temp, 10, "%d", launch->GetTimeout());
    AddArgument("-idle-timeout");
    AddArgument(temp);

    //
    // Add any additional arguments specified in the profile
    //
    for (size_t i = 0; i < launch->GetArguments().size(); ++i)
        AddArgument(launch->GetArguments()[i]);
}

// ****************************************************************************
// Method: RemoteProxyBase::Parallel
//
// Purpose: 
//   Returns whether the application on the other side is parallel.
//
// Returns:    false.
//
// Programmer: Brad Whitlock
// Creation:   Tue May 6 10:15:46 PDT 2003
//
// Modifications:
//   
// ****************************************************************************

bool
RemoteProxyBase::Parallel() const
{
    return false;
}

// ****************************************************************************
// Method: RemoteProxyBase::GetVisItString
//
// Purpose: 
//   Returns the path to the visit command on the remote computer.
//
// Returns:    The path to the visit command.
//
// Programmer: Brad Whitlock
// Creation:   Fri May 2 15:01:39 PST 2003
//
// Modifications:
//   Kathleen Bonnell, Fri Oct 8 08:50:11 PDT 2010
//   On Windows, append "visit" instead of "bin/visit".
//   
//   Kathleen Bonnell, Fri Nov 19 17:34:04 MST 2010
//   Revert windows-specific change.
//
//   Brad Whitlock, Thu Dec 8 12:16:34 PST 2011
//   If we find :/ or :\ in the -dir argument then assume the remote computer
//   is a Windows computer and we should handle it a little differently.
//
// ****************************************************************************

std::string
RemoteProxyBase::GetVisItString() const
{ 
    // The -dir flag means that the visit script is not in our path, so we
    // must prepend it to the name of the visit script.  Do that check now.
    std::string visitString = "visit";
    for (size_t i = 0 ; i < argv.size() ; ++i)
    {
        if (argv[i] == "-dir" && (i + 1) < argv.size())
        {
            const std::string &dirArg = argv[i + 1];
            visitString = dirArg;
            // If the dirArg contains these characters then assume the remote path is Windows
            if(visitString.find(":\\") != std::string::npos || visitString.find(":/") != std::string::npos)
            {
                if (dirArg[dirArg.size() - 1] != '/' && dirArg[dirArg.size()-1] != '\\')
                    visitString += "\\";
                visitString += "visit.exe";
            }
            else
            {
                if (dirArg[dirArg.size() - 1] != '/')
                    visitString += "/";
                visitString += "bin/visit";
            }
            ++i;
        }
    }

    return visitString;
}

// ****************************************************************************
// Method: RemoteProxyBase::AddExtraArguments
//
// Purpose: 
//   Adds extra arguments to the component before it is launched.
//
// Programmer: Brad Whitlock
// Creation:   Fri May 2 15:03:15 PST 2003
//
// Modifications:
//   Brad Whitlock, Tue Jun 10 14:47:35 PST 2003
//   Let -dir pass through.
//
// ****************************************************************************

void
RemoteProxyBase::AddExtraArguments()
{
    //
    // Add any extra arguments to the component before opening it.
    //
    for(size_t i = 0; i < argv.size(); ++i)
        component->AddArgument(argv[i]);
}

// ****************************************************************************
// Method: RemoteProxyBase::GetReadConnection
//
// Purpose: 
//   Get the component's i'th read connection.
//
// Arguments:
//   i : The index of the read connection. Default=0
//
// Returns:    The i'th connection.
//
// Note:       
//
// Programmer: Brad Whitlock
// Creation:   Wed Nov 21 11:44:29 PST 2007
//
// Modifications:
//   
// ****************************************************************************

Connection *
RemoteProxyBase::GetReadConnection(int i) const
{
    return component ? component->GetReadConnection(i) : 0;
}

// ****************************************************************************
// Method: RemoteProxyBase::GetWriteConnection
//
// Purpose: 
//   Get the component's i'th write connection.
//
// Arguments:
//   i : The index of the write connection. Default=0
//
// Returns:    The i'th connection.
//
// Note:       
//
// Programmer: Brad Whitlock
// Creation:   Wed Nov 21 11:44:29 PST 2007
//
// Modifications:
//   
// ****************************************************************************

Connection *
RemoteProxyBase::GetWriteConnection(int i) const
{
    return component ? component->GetWriteConnection(i) : 0;
}
