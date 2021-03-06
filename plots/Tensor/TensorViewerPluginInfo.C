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
//  File: TensorViewerPluginInfo.C
// ************************************************************************* //

#include <TensorPluginInfo.h>
#include <QApplication>
#include <avtTensorPlot.h>

// ****************************************************************************
//  Function:  GetViewerInfo
//
//  Purpose:
//    Return a new ViewerPluginInfo for the Tensor plot.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************
extern "C" PLOT_EXPORT ViewerPlotPluginInfo* Tensor_GetViewerInfo()
{
    TensorViewerPluginInfo::InitializeGlobalObjects();
    return new TensorViewerPluginInfo;
}

//
// Storage for static data elements.
//
TensorAttributes *TensorViewerPluginInfo::clientAtts = NULL;
TensorAttributes *TensorViewerPluginInfo::defaultAtts = NULL;

// ****************************************************************************
//  Method:  TensorViewerPluginInfo::InitializeGlobalObjects
//
//  Purpose:
//    Initialize the plot atts.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************
void
TensorViewerPluginInfo::InitializeGlobalObjects()
{
    TensorViewerPluginInfo::clientAtts  = new TensorAttributes;
    TensorViewerPluginInfo::defaultAtts = new TensorAttributes;
}

// ****************************************************************************
//  Method: TensorViewerPluginInfo::GetClientAtts
//
//  Purpose:
//    Return a pointer to the viewer client attributes.
//
//  Returns:    A pointer to the viewer client attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

AttributeSubject *
TensorViewerPluginInfo::GetClientAtts()
{
    return clientAtts;
}

// ****************************************************************************
//  Method: TensorViewerPluginInfo::GetDefaultAtts
//
//  Purpose:
//    Return a pointer to the viewer default attributes.
//
//  Returns:    A pointer to the viewer default attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

AttributeSubject *
TensorViewerPluginInfo::GetDefaultAtts()
{
    return defaultAtts;
}

// ****************************************************************************
//  Method: TensorViewerPluginInfo::SetClientAtts
//
//  Purpose:
//    Set the viewer client attributes.
//
//  Arguments:
//    atts      A pointer to the new client attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
TensorViewerPluginInfo::SetClientAtts(AttributeSubject *atts)
{
    *clientAtts = *(TensorAttributes *)atts;
    clientAtts->Notify();
}

// ****************************************************************************
//  Method: TensorViewerPluginInfo::GetClientAtts
//
//  Purpose:
//    Get the viewer client attributes.
//
//  Arguments:
//    atts      A pointer to return the client default attributes in.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
TensorViewerPluginInfo::GetClientAtts(AttributeSubject *atts)
{
    *(TensorAttributes *)atts = *clientAtts;
}

// ****************************************************************************
//  Method: TensorViewerPluginInfo::AllocAvtPlot
//
//  Purpose:
//    Return a pointer to a newly allocated avt plot.
//
//  Returns:    A pointer to the newly allocated avt plot.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

avtPlot *
TensorViewerPluginInfo::AllocAvtPlot()
{
    return new avtTensorPlot;
}

// ****************************************************************************
//  Method: TensorViewerPluginInfo::InitializePlotAtts
//
//  Purpose:
//    Initialize the plot attributes to the default attributes.
//
//  Arguments:
//    atts      The attribute subject to initialize.
//    plot      The viewer plot whose attributes are getting initialized.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
TensorViewerPluginInfo::InitializePlotAtts(AttributeSubject *atts,
    ViewerPlot *)
{
    *(TensorAttributes*)atts = *defaultAtts;
}

// ****************************************************************************
// Method: TensorViewerPluginInfo::GetVariableTypes
//
// Purpose: 
//   Returns a flag indicating the types of variables that can be put in the
//   plot's variable list.
//
// Returns:    A flag indicating the types of variables that can be put in
//             the plot's variable list.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

int
TensorViewerPluginInfo::GetVariableTypes() const
{
    return VAR_CATEGORY_TENSOR | VAR_CATEGORY_SYMMETRIC_TENSOR;
}

// ****************************************************************************
//  Method: TensorViewerPluginInfo::GetMenuName
//
//  Purpose:
//    Return a pointer to the name to use in the viewer menus.
//
//  Returns:    A pointer to the name to use in the viewer menus.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

QString *
TensorViewerPluginInfo::GetMenuName() const
{
    return new QString(qApp->translate("PlotNames", "Tensor"));
}

// ****************************************************************************
//  Method: TensorViewerPluginInfo::XPMIconData
//
//  Purpose:
//    Return a pointer to the icon data.
//
//  Returns:    A pointer to the icon data.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

#include <Tensor.xpm>
const char **
TensorViewerPluginInfo::XPMIconData() const
{
    return Tensor_xpm;
}

