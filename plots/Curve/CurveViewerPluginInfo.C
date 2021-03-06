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
//  File: CurveViewerPluginInfo.C
// ************************************************************************* //

#include <CurvePluginInfo.h>
#include <QApplication>
#include <avtCurvePlot.h>

// ****************************************************************************
//  Function:  GetViewerInfo
//
//  Purpose:
//    Return a new ViewerPluginInfo for the Curve plot.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************
extern "C" PLOT_EXPORT ViewerPlotPluginInfo* Curve_GetViewerInfo()
{
    CurveViewerPluginInfo::InitializeGlobalObjects();
    return new CurveViewerPluginInfo;
}

//
// Storage for static data elements.
//
CurveAttributes *CurveViewerPluginInfo::clientAtts = NULL;
CurveAttributes *CurveViewerPluginInfo::defaultAtts = NULL;

// ****************************************************************************
//  Method:  CurveViewerPluginInfo::InitializeGlobalObjects
//
//  Purpose:
//    Initialize the plot atts.
//
//  Programmer: whitlocb -- generated by xml2info
//  Creation:   Fri Mar 26 15:09:32 PST 2004
//
//  Modifications:
//    Kathleen Bonnell, Mon Oct 31 17:05:35 PST 2005
//    Initialize colorIndex. 
//
// ****************************************************************************
static int colorIndex;
void
CurveViewerPluginInfo::InitializeGlobalObjects()
{
    colorIndex = 0;
    CurveViewerPluginInfo::clientAtts  = new CurveAttributes;
    CurveViewerPluginInfo::defaultAtts = new CurveAttributes;
}


// ****************************************************************************
//  Method: CurveViewerPluginInfo::GetClientAtts
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
CurveViewerPluginInfo::GetClientAtts()
{
    return clientAtts;
}

// ****************************************************************************
//  Method: CurveViewerPluginInfo::GetDefaultAtts
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
CurveViewerPluginInfo::GetDefaultAtts()
{
    return defaultAtts;
}

// ****************************************************************************
//  Method: CurveViewerPluginInfo::SetClientAtts
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
CurveViewerPluginInfo::SetClientAtts(AttributeSubject *atts)
{
    *clientAtts = *(CurveAttributes *)atts;
    clientAtts->Notify();
}

// ****************************************************************************
//  Method: CurveViewerPluginInfo::GetClientAtts
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
CurveViewerPluginInfo::GetClientAtts(AttributeSubject *atts)
{
    *(CurveAttributes *)atts = *clientAtts;
}

// ****************************************************************************
//  Method: CurveViewerPluginInfo::AllocAvtPlot
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
CurveViewerPluginInfo::AllocAvtPlot()
{
    return new avtCurvePlot;
}

bool
CurveViewerPluginInfo::PermitsCurveViewScaling() const
{
    return true;
}

// ****************************************************************************
//  Method: CurveViewerPluginInfo::InitializePlotAtts
//
//  Purpose:
//    Initialize the plot attributes to the default attributes.
//
//  Arguments:
//    atts      The attribute subject to initialize.
//    md        The metadata used to initialize.
//    atts      The variable name used to initialize.
//
//  Programmer: whitlocb -- generated by xml2info
//  Creation:   Fri Mar 26 15:09:32 PST 2004
//
//  Modifications:
//    Kathleen Bonnell, Mon Oct 31 17:05:35 PST 2005
//    Added call to SetColor.
//
// ****************************************************************************

void
CurveViewerPluginInfo::InitializePlotAtts(AttributeSubject *atts,
    ViewerPlot *)
{
    *(CurveAttributes*)atts = *defaultAtts;
    SetColor(atts);
}


// ****************************************************************************
// Method: CurveViewerPluginInfo::GetVariableTypes
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
CurveViewerPluginInfo::GetVariableTypes() const
{
    return VAR_CATEGORY_CURVE;
}

// ****************************************************************************
//  Method: CurveViewerPluginInfo::GetMenuName
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
CurveViewerPluginInfo::GetMenuName() const
{
    return new QString(qApp->translate("PlotNames", "Curve"));
}

// ****************************************************************************
//  Method: CurveViewerPluginInfo::XPMIconData
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

#include <Curve.xpm>
const char **
CurveViewerPluginInfo::XPMIconData() const
{
    return Curve_xpm;
}

// ****************************************************************************
//  Method: CurveViewerPluginInfo::SetColor
//
//  Purpose:
//    Sets the color if the CycleColors att is set to true. 
//
//  Arguments:
//    atts      The attribute subject. 
//
//  Programmer: Kathleen Bonnell 
//  Creation:   October 31, 2005 
//
//  Modifications:
//
//    Hank Childs, Sat Mar 13 10:35:34 PST 2010
//    Change Boolean test to comparison with enumeration.
//
// ****************************************************************************
#include <avtColorTables.h>

void
CurveViewerPluginInfo::SetColor(AttributeSubject *atts)
{
    CurveAttributes *curveAtts = (CurveAttributes *)atts;

    if (curveAtts->GetCurveColorSource() == CurveAttributes::Cycle)
    {
        ColorAttribute c;
        unsigned char rgb[3] = {0,0,0};

        //
        // Try and get the color for the colorIndex'th color in the default
        // discrete color table.
        //
        avtColorTables *ct = avtColorTables::Instance();
        if(ct->GetControlPointColor(ct->GetDefaultDiscreteColorTable(),
           colorIndex, rgb))
        {
            c.SetRed(int(rgb[0]));
            c.SetGreen(int(rgb[1]));
            c.SetBlue(int(rgb[2]));
        }
        curveAtts->SetCurveColor(c);
        // Increment the color index.
        colorIndex = (colorIndex + 1) % ct->GetNumColors();
    }
}

