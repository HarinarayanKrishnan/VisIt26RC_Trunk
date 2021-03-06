/*****************************************************************************
*
* Copyright (c) 2011, Allinea
* All rights reserved.
*
* This file has been contributed to the VisIt project, which is
* Copyright (c) Lawrence Livermore National Security, LLC. For  details, see
* https://visit.llnl.gov/.  The full copyright notice is contained in the 
* file COPYRIGHT located at the root of the VisIt distribution or at 
* http://www.llnl.gov/visit/copyright.html.
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

#include "visitdata.h"

VisItData::VisItData() : mMesh(0), mVariable(0)
{
}

VisItData::~VisItData()
{
    if (mMesh)
        delete mMesh;
    if (mVariable)
        delete mVariable;
}

//! Replaces the current mesh with the specified mesh
/*! The mesh coordinates are generated for the new current mesh. No action is taken if the
 *  specified mesh holds the same information as the current one. */
void
VisItData::updateMesh(const VisItMesh &mesh)
{
    if (!mMesh || !(*mMesh==mesh))
    {
        delete mMesh;
        mMesh = new VisItMesh(mesh);
        mMesh->generate();
    }
}

//! Replaces the current variable info with that specified
/*! No action is taken if the specified mesh holds the same information as the current one */
void
VisItData::updateVariable(const VisItVariable &var)
{
    if (!mVariable || !(*mVariable==var))
    {
        delete mVariable;
        mVariable = new VisItVariable(var);
    }
}
