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

// ************************************************************************* //
//                      VisWinRenderingWithoutWindow.C                       //
// ************************************************************************* //

#include <VisWinRenderingWithoutWindow.h>

#include <Environment.h>
#include <avtCallback.h>
#include <visit-config.h>

#include <vtkMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#ifdef EXTERNAL_VTK_BUILD
#if __APPLE__
#include <vtkCocoaRenderWindow.h>
#include "VisWinRenderingCocoaHideWindow.h"
void UnMapWindow(vtkRenderWindow* v)
{
    vtkCocoaRenderWindow* vx=dynamic_cast<vtkCocoaRenderWindow*>(v);
    if(vx) VisWinRenderingCocoa::HideRenderWindow(vx->GetRootWindow());
}
#elif __unix__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vtkXOpenGLRenderWindow.h>
void UnMapWindow(vtkRenderWindow* v)
{
    vtkXOpenGLRenderWindow* vx = dynamic_cast<vtkXOpenGLRenderWindow*>(v);
    if(vx) XUnmapWindow(vx->GetDisplayId(),vx->GetWindowId());
}
#else //TODO: Hide Window if using External VTK
void UnMapWindow(vtkRenderWindow* v) { /*do nothing..*/  }
#endif

#endif

#define DS_NOT_CHECKED    0
#define DS_NOT_AVAILABLE  1
#define DS_AVAILABLE      2

// ****************************************************************************
//  Method: VisWinRenderingWithoutWindow constructor
//
//  Programmer: Hank Childs
//  Creation:   February 1, 2002
//
//  Modifications:
//    Kathleen Bonnell, Tue Feb 11 11:28:03 PST 2003 
//    Removed iren.
//
//    Tom Fogal, Tue Nov 24 11:25:39 MST 2009
//    Make sure to set offscreen before other initialization.
//
//    Kathleen Biagas, Wed Oct 31 17:29:26 PDT 2012
//    'vtkWin32OpenGLRenderWindow' needs OffscreenRendering set, too.
//
//    Cyrus Harrison, Sat Nov  3 19:37:42 PDT 2012
//    For osx w/o mesa, 'vtkCocoaRenderWindow' needs OffscreenRendering set, 
//    as well.
//
// ****************************************************************************

VisWinRenderingWithoutWindow::VisWinRenderingWithoutWindow(
                                                    VisWindowColleagueProxy &p)
   : VisWinRendering(p)
{
    //
    // It is kind of assumed that these new calls will return Mesa derived
    // types, but there is nothing from this class's perspective saying we have
    // to -- OpenGL offscreen rendering is fully supported.  So: it might be
    // Mesa that we are getting, but we don't care.
    //
    renWin = vtkRenderWindow::New();
#ifdef EXTERNAL_VTK_BUILD
    if(std::string(renWin->GetClassName()) == "vtkOSOpenGLRenderWindow")
    if(std::string(renWin->GetClassName()) == "vtkWin32OpenGLRenderWindow")
        renWin->OffScreenRenderingOn();
#else
        renWin->OffScreenRenderingOn();
#endif
    InitializeRenderWindow(renWin);

    displayStatus = DS_NOT_CHECKED;
}


// ****************************************************************************
//  Method: VisWinRenderingWithoutWindow destructor
//
//  Programmer: Hank Childs
//  Creation:   February 1, 2002
//
//  Modifications:
//    Kathleen Bonnell, Tue Feb 11 11:28:03 PST 2003 
//    Removed iren.
//
// ****************************************************************************
 
VisWinRenderingWithoutWindow::~VisWinRenderingWithoutWindow()
{
    if (renWin != NULL)
    {
        renWin->Delete();
        renWin = NULL;
    }
}

// ****************************************************************************
//  Method: VisWinRenderingWithoutWindow::GetRenderWindow
//
//  Purpose:
//      Gets the render window.  This is a virtual function that allows the
//      base type to operate on the window, even though it is not aware that
//      there is no window.
//
//  Returns:    The render window typed as the base class.
//
//  Programmer: Hank Childs
//  Creation:   February 1, 2002
//
// ****************************************************************************
 
vtkRenderWindow *
VisWinRenderingWithoutWindow::GetRenderWindow(void)
{
    return renWin;
}

// ****************************************************************************
// Method: VisWinRenderingWithoutWindow::RenderRenderWindow
//
// Purpose: 
//   Render the render window when it is safe to do so.
//
// Arguments:
//
// Returns:    
//
// Note:       
//
// Programmer: Brad Whitlock
// Creation:   Wed Mar 13 16:06:22 PDT 2013
//
// Modifications:
//   
// ****************************************************************************

void
VisWinRenderingWithoutWindow::RenderRenderWindow(void)
{
#if defined(__unix__) && !defined(__APPLE__) && !defined(VISIT_USE_MANGLED_MESA)
    if(displayStatus == DS_NOT_CHECKED)
    {
        // On X11 systems not using mangled mesa, make sure that the DISPLAY is set.
        if(Environment::get("DISPLAY").empty())
            displayStatus = DS_NOT_AVAILABLE;
        else
            displayStatus = DS_AVAILABLE;
    }

    if(displayStatus == DS_AVAILABLE)
    {
        GetRenderWindow()->Render();
    }
    else
    {
        avtCallback::IssueWarning("VisIt was not built with support for "
            "software-based offscreen rendering. This means that the DISPLAY "
            "variable must be set to a valid X-server display in order to render "
            "an image. If you are running client/server, consider adding -X to "
            "your SSH arguments.");
    }
#else
    GetRenderWindow()->Render();
#endif
}

// ****************************************************************************
//  Method: VisWinRenderingWithoutWindow::RealizeRenderWindow
//
//  Purpose:
//      Forces VTK to initialize the underlying window.
//
//  Programmer: Tom Fogal
//  Creation:   December 9th, 2009
//
//
//  Modifications:
//   Cyrus Harrison, Sat Nov  3 23:51:13 PDT 2012
//   Force large window size for offscreen cocoa.
//
//   Brad Whitlock, Wed Mar 13 16:07:35 PDT 2013
//   Don't directly call Render.
//
// ****************************************************************************

void
VisWinRenderingWithoutWindow::RealizeRenderWindow(void)
{
  // We'd like to do something lighter weight, but this seems to be the only
  // way to *force* VTK to initialize in all cases.  The good news is that this
  // method is typically called before we've got data in the RW, so it
  // shouldn't be as heavy as it looks at first glance.

  //
  // SetSize doesn't work as expected with vtkCocoaRenderWindow in 
  // an offscreen setting.
  // B/c of this we are forced to create a large window, the size of 
  // which bounds our offscreen rendering.
  //
  if(std::string(renWin->GetClassName()) == "vtkCocoaRenderWindow")
  {
      //TODO: we may want to query to find the largest valid size
      // OSX limits windows to 10Kx10K, however OpenGL contexts
      // are limited further.
      renWin->SetSize(4096,4096);
      renWin->SetPosition(-10000,-10000);
  }
  else
  {
      renWin->SetSize(300,300);
  }
  RenderRenderWindow();

#ifdef EXTERNAL_VTK_BUILD
  std::string cname = renWin->GetClassName();
  if(cname != "vtkOSOpenGLRenderWindow") UnMapWindow(renWin);
#endif
}

// ****************************************************************************
//  Method: VisWinRenderingWithoutWindow::SetImmediateRenderingMode
//
//  Purpose:
//      Prevent VTK from using display lists.
//
//  Programmer: Brad Whitlock
//  Creation:   Fri Sep 30 18:20:10 PDT 2011
//
//  Notes: This offscreen rendering colleague is responsible for rendering
//         using Mesa. As of VTK 5.8 and Mesa 7.8.2, there are some rare
//         glitches with offscreen Mesa vbo's that cause text to go missing.
//         By forcing all of VTK to not use display lists, we avoid the 
//         problems.
//
//  Modifications:
//
// ****************************************************************************

void
VisWinRenderingWithoutWindow::SetImmediateModeRendering(bool)
{
    // In offscreen rendering, never use display lists.
    vtkMapper::GlobalImmediateModeRenderingOn();
}
