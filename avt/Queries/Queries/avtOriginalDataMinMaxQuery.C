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
//                      avtOriginalDataMinMaxQuery.C                         //
// ************************************************************************* //

#include <avtOriginalDataMinMaxQuery.h>

#include <avtCondenseDatasetFilter.h>
#include <avtExpressionEvaluatorFilter.h>
#include <avtOriginatingSource.h>



// ****************************************************************************
//  Method: avtOriginalDataMinMaxQuery::avtOriginalDataMinMaxQuery
//
//  Purpose:
//      Construct an avtOriginalDataMinMaxQuery object.
//
//  Programmer:   Kathleen Bonnell 
//  Creation:     February 10, 2004
//
//  Modifications:
//    Kathleen Bonnell, Wed Mar 31 16:07:50 PST 2004
//    Added optional args.
//
//    Kathleen Bonnell, Wed Apr 14 18:05:08 PDT 2004 
//    Added condense filter. 
//
//    Kathleen Bonnell, Tue Jun 29 08:14:35 PDT 2004 
//    Removed condense filter. 
//
// ****************************************************************************

avtOriginalDataMinMaxQuery::avtOriginalDataMinMaxQuery(bool min, bool max)
    : avtMinMaxQuery(min, max)
{
    eef = new avtExpressionEvaluatorFilter;
}


// ****************************************************************************
//  Method: avtOriginalDataMinMaxQuery::~avtOriginalDataMinMaxQuery
//
//  Purpose:
//      Destruct an avtOriginalDataMinMaxQuery object.
//
//  Programmer:   Kathleen Bonnell 
//  Creation:     February 10, 2004 
//
//  Modifications:
//    Kathleen Bonnell, Wed Apr 14 18:05:08 PDT 2004 
//    Added condense filter. 
//
//    Kathleen Bonnell, Tue Jun 29 08:14:35 PDT 2004 
//    Removed condense filter. 
//
// ****************************************************************************

avtOriginalDataMinMaxQuery::~avtOriginalDataMinMaxQuery()
{
    if (eef != NULL)
    {
        delete eef;
        eef = NULL;
    }
}


// ****************************************************************************
//  Method: avtOriginalDataMinMaxQuery::ApplyFilters
//
//  Purpose:
//    Applies avtExpressionEvaluatorFilter to the input data object and returns
//    the output from the filter.
//
//  Programmer: Kathleen Bonnell
//  Creation:   February 10, 2004 
//
//  Modifications:
//    Kathleen Bonnell, Fri Feb 20 17:03:48 PST 2004
//    Create new pipeline spec so that input can be load balanced.
//
//    Kathleen Bonnell, Wed Mar 31 16:07:50 PST 2004
//    Added logic for time-varying case.
//
//    Kathleen Bonnell, Wed Apr 14 18:05:08 PDT 2004 
//    Added condense filter. 
//
//    Kathleen Bonnell, Tue Apr 20 09:36:58 PDT 2004 
//    Ensure that we are working with correct var and timestep. 
//
//    Kathleen Bonnell, Tue Jun 29 08:14:35 PDT 2004 
//    Removed condense filter. 
//
//    Hank Childs, Wed Dec 22 15:33:59 PST 2010
//    Set up the contract differently if we are parallelizing over time.
//
// ****************************************************************************

avtDataObject_p
avtOriginalDataMinMaxQuery::ApplyFilters(avtDataObject_p inData)
{
    Preparation(inData);

    avtDataRequest_p dataRequest = 
        inData->GetOriginatingSource()->GetFullDataRequest();

    if (dataRequest->GetVariable() != queryAtts.GetVariables()[0] ||
        dataRequest->GetTimestep() != queryAtts.GetTimeStep() ||
        timeVarying)
    {
        dataRequest = new avtDataRequest(queryAtts.GetVariables()[0].c_str(), 
                                         queryAtts.GetTimeStep(), 
                                         dataRequest->GetRestriction());
    }

    avtContract_p contract =
        new avtContract(dataRequest, queryAtts.GetPipeIndex()); 
    if (ParallelizingOverTime())
    {
        // Make sure we set up our request to do streaming.
        contract->SetOnDemandStreaming(true);
        contract->UseLoadBalancing(false);
    }

    avtDataObject_p temp;
    CopyTo(temp, inData);
    eef->SetInput(temp);
    avtDataObject_p retObj = eef->GetOutput();
    retObj->Update(contract);
    return retObj;
}


