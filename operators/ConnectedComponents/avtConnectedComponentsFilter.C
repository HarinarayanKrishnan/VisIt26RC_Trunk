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
//  File: avtConnectedComponentsFilter.C
// ************************************************************************* //

#include <avtConnectedComponentsFilter.h>

#include <Expression.h>
#include <ExpressionList.h>
#include <ParsingExprList.h>

#include <string>
#include <vector>


// ****************************************************************************
//  Method: avtConnectedComponentsFilter constructor
//
//  Programmer: hchilds -- generated by xml2avt
//  Creation:   Thu Sep 23 14:25:44 PST 2010
//
// ****************************************************************************

avtConnectedComponentsFilter::avtConnectedComponentsFilter()
{
}


// ****************************************************************************
//  Method: avtConnectedComponentsFilter destructor
//
//  Programmer: hchilds -- generated by xml2avt
//  Creation:   Thu Sep 23 14:25:44 PST 2010
//
//  Modifications:
//
// ****************************************************************************

avtConnectedComponentsFilter::~avtConnectedComponentsFilter()
{
}


// ****************************************************************************
//  Method:  avtConnectedComponentsFilter::Create
//
//  Programmer: hchilds -- generated by xml2avt
//  Creation:   Thu Sep 23 14:25:44 PST 2010
//
// ****************************************************************************

avtFilter *
avtConnectedComponentsFilter::Create()
{
    return new avtConnectedComponentsFilter();
}


// ****************************************************************************
//  Method:      avtConnectedComponentsFilter::SetAtts
//
//  Purpose:
//      Sets the state of the filter based on the attribute object.
//
//  Arguments:
//      a        The attributes to use.
//
//  Programmer: hchilds -- generated by xml2avt
//  Creation:   Thu Sep 23 14:25:44 PST 2010
//
// ****************************************************************************

void
avtConnectedComponentsFilter::SetAtts(const AttributeGroup *a)
{
    atts = *(const ConnectedComponentsAttributes*)a;
}


// ****************************************************************************
//  Method: avtConnectedComponentsFilter::Equivalent
//
//  Purpose:
//      Returns true if creating a new avtConnectedComponentsFilter with the given
//      parameters would result in an equivalent avtConnectedComponentsFilter.
//
//  Programmer: hchilds -- generated by xml2avt
//  Creation:   Thu Sep 23 14:25:44 PST 2010
//
// ****************************************************************************

bool
avtConnectedComponentsFilter::Equivalent(const AttributeGroup *a)
{
    return (atts == *(ConnectedComponentsAttributes*)a);
}


// ****************************************************************************
//  Method: avtConnectedComponentsField::InferVariableNameFromContract
//
//  Purpose:
//      Studies the contract and determines the name of the variable we
//      are supposed to produce.
//
//  Programmer: Hank Childs
//  Creation:   September 24, 2010
//
// ****************************************************************************

void
avtConnectedComponentsFilter::InferVariableNameFromContract(avtContract_p c)
{
    avtDataRequest_p in_dr = c->GetDataRequest();

    bool foundIt = false;
    if (strncmp(in_dr->GetVariable(), "operators/ConnectedComponents/",
                strlen("operators/ConnectedComponents/")) == 0)
    {
        foundIt = true;
        varname = in_dr->GetVariable()+strlen("operators/ConnectedComponents/");
    }

    if (!foundIt)
    {
        std::vector<CharStrRef>   vars2nd = in_dr->GetSecondaryVariablesWithoutDuplicates();
        for (int i = 0 ; i < vars2nd.size() ; i++)
        if (strncmp(*(vars2nd[i]), "operators/ConnectedComponents/",
                    strlen("operators/ConnectedComponents/")) == 0)
        {
            foundIt = true;
            varname = *(vars2nd[i])+strlen("operators/ConnectedComponents/");
        }
    }

    if (!foundIt)
    {
        EXCEPTION1(VisItException, "Unable to determine variable name from contract");
    }
}


// ****************************************************************************
//  Method: avtConnectedComponentsFilter::GetCustomExpressions
//
//  Purpose:
//      Gets the expression definitions that should be used when executing
//      this filter.
//
//  Programmer: Hank Childs
//  Creation:   September 24, 2010
//
//  Modifications:
//  Cyrus Harrison, Thu Dec 16 09:24:31 PST 2010
//  Support explicit enable/disable of the ghost zone neighbors optimization.
//
// ****************************************************************************

void
avtConnectedComponentsFilter::GetCustomExpressions(std::vector<Expression> &elist)
{
    Expression exp;
    char fullname[1024];
    SNPRINTF(fullname, 1024, "operators/ConnectedComponents/%s", varname.c_str());
    exp.SetName(fullname);
    char defn[1024];
    std::string gzopt = "\"true\"";
    if (!atts.GetEnableGhostNeighborsOptimization())
        gzopt  = "\"false\"";
    SNPRINTF(defn, 1024, "conn_components(%s,%s)", varname.c_str(),gzopt.c_str());
    exp.SetDefinition(defn);
    exp.SetType(Expression::ScalarMeshVar);
    elist.push_back(exp);
}


// ****************************************************************************
//  Method: avtConnectedComponentsFilter::GetVariableToEvaluated
//
//  Purpose:
//      Gets the expressions that should be evaluated when this filter
//      executes.
//
//  Programmer: Hank Childs
//  Creation:   September 24, 2010
//
// ****************************************************************************

void
avtConnectedComponentsFilter::GetVariablesToEvaluate(std::vector<std::string> &list)
{
    char fullname[1024];
    SNPRINTF(fullname, 1024, "operators/ConnectedComponents/%s", varname.c_str());
    list.push_back(fullname);
}


