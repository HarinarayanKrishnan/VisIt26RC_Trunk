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

#include "MaterialTetrahedron.h"

// ****************************************************************************
//  Default Constructor:  MaterialTetrahedron::MaterialTetrahedron
//
//  Programmer:  Jeremy Meredith
//  Creation:    December 11, 2000
//
// ****************************************************************************
MaterialTetrahedron::MaterialTetrahedron()
{
}

// ****************************************************************************
//  Constructor:  MaterialTetrahedron::MaterialTetrahedron
//
//  Purpose:
//    Initialize the object and make sure there is enough room at each
//    node to store the v.f.'s.
//
//  Arguments:
//    nmat_      the number of materials
//
//  Programmer:  Jeremy Meredith
//  Creation:    December 11, 2000
//
// ****************************************************************************
MaterialTetrahedron::MaterialTetrahedron(int nmat_)
{
    nmat = nmat_;
    node[0].matvf.resize(nmat);
    node[1].matvf.resize(nmat);
    node[2].matvf.resize(nmat);
    node[3].matvf.resize(nmat);
}

// ****************************************************************************
//  Copy Constructor:  MaterialTetrahedron::MaterialTetrahedron
//
//  Arguments:
//    rhs        the source MaterialTetrahedron
//
//  Programmer:  Jeremy Meredith
//  Creation:    December 11, 2000
//
//  Modifications:
//    Jeremy Meredith, Tue Sep 18 11:54:51 PDT 2001
//    Made it copy the origindex.
//
// ****************************************************************************
MaterialTetrahedron::MaterialTetrahedron(const MaterialTetrahedron &rhs)
{
    for (int n=0; n<4; n++)
    {
        node[n].origindex = rhs.node[n].origindex;
        node[n].matvf     = rhs.node[n].matvf;
        for (int i=0; i<MAX_NODES_PER_ZONE; i++)
            node[n].weight[i] = rhs.node[n].weight[i];
    }
}
