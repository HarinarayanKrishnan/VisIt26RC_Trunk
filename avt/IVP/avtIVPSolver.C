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
//                              avtIVPSolver.C                               //
// ************************************************************************* //

#include <avtIVPSolver.h>
#include <avtIVPStateHelper.h>


// ****************************************************************************
//  Method: avtIVPSolver::GetState
//
//  Purpose:
//      Gets the state of the IVP solver.
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
// ****************************************************************************

avtIVPSolver::avtIVPSolver() : convertToCartesian(0), convertToCylindrical(0),
                               order(1)
{
}


// ****************************************************************************
//  Method: avtIVPSolver::GetState
//
//  Purpose:
//      Gets the state of the IVP solver.
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
// ****************************************************************************

void
avtIVPSolver::GetState( avtIVPState& state )
{
    avtIVPStateHelper aiss(avtIVPStateHelper::GET, 0);
    this->AcceptStateVisitor(aiss);

    state.allocate(aiss.size());
    
    aiss.Reset(avtIVPStateHelper::GET, state._data);
    this->AcceptStateVisitor(aiss);
}


// ****************************************************************************
//  Method: avtIVPSolver::PutState
//
//  Purpose:
//      Sets the state of the IVP solver.
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
// ****************************************************************************

void 
avtIVPSolver::PutState(const avtIVPState& state)
{
    avtIVPStateHelper aiss(avtIVPStateHelper::PUT, state._data);
    this->AcceptStateVisitor(aiss);
}


avtIVPSolver::Result
avtIVPSolver::ConvertResult(const avtIVPField::Result &res) const
{
    if (res == avtIVPField::OK)
        return OK;
    else if (res == avtIVPField::OUTSIDE_SPATIAL)
        return OUTSIDE_SPATIAL;
    else if (res == avtIVPField::OUTSIDE_BOTH)
        return OUTSIDE_SPATIAL;
    else if (res == avtIVPField::OUTSIDE_TEMPORAL)
        return OUTSIDE_TEMPORAL;
    else
        return UNSPECIFIED_ERROR;
}
