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
//                       avtExpressionComponentMacro.C                       //
// ************************************************************************* //

#include <avtExpressionComponentMacro.h>

#include <stdio.h>

#include <ImproperUseException.h>


// ****************************************************************************
//  Method: avtExpressionComponentMacro constructor
//
//  Programmer: Hank Childs
//  Creation:   June 30, 2005
//
// ****************************************************************************

avtExpressionComponentMacro::avtExpressionComponentMacro()
{
    expr = "<NotSet>";
}


// ****************************************************************************
//  Method: avtExpressionComponentMacro destructor
//
//  Programmer: Hank Childs
//  Creation:   June 30, 2005
//
// ****************************************************************************

avtExpressionComponentMacro::~avtExpressionComponentMacro()
{
    ;
}


// ****************************************************************************
//  Method: avtExpressionComponentMacro::SetMacro
//
//  Purpose:
//      Sets the arguments to use in the macro.
//
//  Programmer: Hank Childs
//  Creation:   June 30, 2005
//
// ****************************************************************************

void
avtExpressionComponentMacro::SetMacro(const std::string &e, int c)
{
    expr = e;
    comp = c;
}


// ****************************************************************************
//  Method: avtExpressionComponentMacro::GetMacro
//
//  Purpose:
//      Uses macros to apply an expression and then take a component of the
//      result.
//
//  Programmer: Hank Childs
//  Creation:   June 30, 2005
//
// ****************************************************************************

void
avtExpressionComponentMacro::GetMacro(std::vector<std::string> &args, 
                                  std::string &ne, Expression::ExprType &type)
{
    if (expr == "<NotSet>")
        EXCEPTION0(ImproperUseException);

    char new_expr[1024];
    sprintf(new_expr, "%s(%s)[%d]", expr.c_str(), args[0].c_str(), comp);
    ne = new_expr;
    type = Expression::ScalarMeshVar;
}


