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
//                            avtMoleculeRenderer.C                          //
// ************************************************************************* //

#include <math.h>
#include <float.h>

#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkRectilinearGrid.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkToolkits.h>

#include <avtGLEWInitializer.h>
#include "avtMoleculeRenderer.h"

#include <avtCallback.h>
#include <avtOpenGLMoleculeRenderer.h>
#include <vtk/InitVTKRenderingConfig.h>
#ifdef VISIT_MANTA
  #include <avtMantaMoleculeRenderer.h>
#endif
#include <avtCallback.h>
#include <ImproperUseException.h>
#include <InvalidLimitsException.h>


// ****************************************************************************
//  Constructor:  avtMoleculeRenderer::avtMoleculeRenderer
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
//  Modifications:
//    Jeremy Meredith, Mon Aug 28 18:13:46 EDT 2006
//    Initialize specular parameters.
//
//    Jeremy Meredith, Thu Apr 22 14:11:20 EDT 2010
//    Added 2D mode.
//
// ****************************************************************************
avtMoleculeRenderer::avtMoleculeRenderer()
{
    initialized = false;
    primaryVariable = "";

    rendererImplementation = NULL;
    currentRendererIsValid = false;

    ambient_coeff = 0;
    spec_coeff = 0;
    spec_power = 0;
    spec_r = 0;
    spec_g = 0;
    spec_b = 0;
    is2D = false;
}

// ****************************************************************************
//  Destructor:  avtMoleculeRenderer::~avtMoleculeRenderer
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
//  Modifications:
//
// ****************************************************************************
avtMoleculeRenderer::~avtMoleculeRenderer()
{
    ReleaseGraphicsResources();
}

// ****************************************************************************
//  Method:  avtMoleculeRenderer::ReleaseGraphicsResources
//
//  Purpose:
//    Delete the renderer implementation.
//
//  Arguments:
//    none
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
// ****************************************************************************
void
avtMoleculeRenderer::ReleaseGraphicsResources()
{
    if (rendererImplementation)
    {
        VTKRen->GetRenderWindow()->MakeCurrent();
        delete rendererImplementation;
        rendererImplementation = NULL;
    }
}

// ****************************************************************************
//  Method: avtMoleculeRenderer::New
//
//  Purpose:
//      A static method that creates the correct type of renderer based on
//      whether we should use OpenGL or Mesa.
//
//  Returns:    A renderer that is of type derived from this class.
//
//  Programmer: Jeremy Meredith
//  Creation:   March 23, 2006
//
//  Modifications:
//
// ****************************************************************************
avtMoleculeRenderer *
avtMoleculeRenderer::New(void)
{
    return new avtMoleculeRenderer;
}

// ****************************************************************************
//  Method:  avtMoleculeRenderer::Render
//
//  Purpose:
//    Set up things necessary to call the renderer implentation.  Make a new
//    implementation object if things have changed.
//
//  Arguments:
//    ds         the dataset to render
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
//  Modifications:
//    Brad Whitlock, Fri Apr 7 11:36:04 PDT 2006
//    Pass window size to the renderer implementation.
//
//    Brad Whitlock, Wed Jun 10 14:08:34 PST 2009
//    I made Mesa support be conditional.
//
//    John Schreiner, Fri Feb 12 19:19:34 MST 2010
//    Removed window size parameter to Render().
//
//    Jeremy Meredith, Thu Apr 22 14:11:20 EDT 2010
//    Added 2D mode.
//
// ****************************************************************************

void
avtMoleculeRenderer::Render(vtkDataSet *ds)
{
    if (!currentRendererIsValid || !rendererImplementation)
    {
        if (rendererImplementation)
            delete rendererImplementation;
#ifdef VISIT_MANTA
        if (avtCallback::UseManta())
          rendererImplementation = new avtMantaMoleculeRenderer;
        else
#endif VISIT_MANTA
          rendererImplementation = new avtOpenGLMoleculeRenderer;

        currentRendererIsValid = true;

        rendererImplementation->SetLevelsLUT(levelsLUT);
    }

    // Do other initialization
    if (!initialized)
    {
        Initialize(ds);
    }

    // get data set
    if (ds->GetDataObjectType() != VTK_POLY_DATA)
    {
        EXCEPTION1(ImproperUseException,
                   "Inappropriate mesh type for Molecule Plot ");
    }
    vtkPolyData *polydata = (vtkPolyData*)ds;

    rendererImplementation->SetVTKRenderer(VTKRen);
    rendererImplementation->SetVTKActor(VTKActor);
    rendererImplementation->Render(polydata,
                                   atts,
                                   immediateModeRendering,
                                   varmin, varmax,
                                   ambient_coeff,
                                   spec_coeff, spec_power,
                                   spec_r, spec_g, spec_b,
                                   is2D);
}


// ****************************************************************************
//  Method:  avtMoleculeRenderer::Initialize
//
//  Purpose:
//    Calculate some one-time stuff with respect to the data set
//
//  Arguments:
//    ds      : the data set
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
//  Modifications:
//
// ****************************************************************************
void
avtMoleculeRenderer::Initialize(vtkDataSet *ds)
{
    // get data set
    vtkPolyData  *polydata = (vtkPolyData*)ds;

    initialized = true;
}

// ****************************************************************************
//  Method:  avtMoleculeRenderer::SetAtts
//
//  Purpose:
//    Set the attributes
//
//  Arguments:
//    a       : the new attributes
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
//  Modifications:
//
// ****************************************************************************
void
avtMoleculeRenderer::SetAtts(const AttributeGroup *a)
{
    const MoleculeAttributes *newAtts = (const MoleculeAttributes*)a;
    atts = *(const MoleculeAttributes*)a;

    initialized = false;
}

// ****************************************************************************
//  Method:  avtMoleculeRenderer::GlobalSetAmbientCoefficient
//
//  Purpose:
//    We've got to track this ourselves.
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
// ****************************************************************************
void
avtMoleculeRenderer::GlobalSetAmbientCoefficient(float coeff)
{
    ambient_coeff = coeff;
}

// ****************************************************************************
//  Method:  avtMoleculeRenderer::GlobalLightingOn
//
//  Purpose:
//    Track when GlobalLightingOn is called.  See the comment below.
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
// ****************************************************************************
void
avtMoleculeRenderer::GlobalLightingOn()
{
    //
    // Stupid hack -- when you turn off the ambient lighting,
    // it never sets the global ambient coefficient to zero.
    // However, it will repeatedly call GlobalLightingOn (followed by
    // calling GlobalSetAmbientCoefficient when it's necessary).  Thus,
    // if we set it to zero here, it will turn it off if there is no
    // ambient light, and it will get set to the true ambient coeff
    // if there is an ambient light when GlobalSetAmbientCoefficient
    // gets called later.
    //
    // This emulates the reason the avtSurfaceAndWireframeRenderer works.
    //
    ambient_coeff = 0.0;
}

// ****************************************************************************
//  Method:  avtMoleculeRenderer::GlobalLightingOff
//
//  Purpose:
//    Force ambient to 1.0.
//
//  Arguments:
//    none
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
// ****************************************************************************
void
avtMoleculeRenderer::GlobalLightingOff()
{
    ambient_coeff = 1.0;
}

// ****************************************************************************
//  Method:  avtMoleculeRenderer::SetSpecularProperties
//
//  Purpose:
//    Track specular properties.
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
// ****************************************************************************
void
avtMoleculeRenderer::SetSpecularProperties(bool flag, double coeff,
                                           double power,
                                           const ColorAttribute &color)
{
    spec_coeff = flag ? coeff : 0;
    spec_power = power;

    int r = color.Red();
    int g = color.Green();
    int b = color.Blue();
    spec_r = float(color.Red())/255.;
    spec_g = float(color.Green())/255.;
    spec_b = float(color.Blue())/255.;
}

// ****************************************************************************
//  Method:  avtMoleculeRenderer::InvalidateColors
//
//  Purpose:
//    This lets the rendererImplementation know that something
//    has caused its colors to potentially be invalid.
//
//  Programmer:  Jeremy Meredith
//  Creation:    March 23, 2006
//
// ****************************************************************************
void
avtMoleculeRenderer::InvalidateColors()
{
    if (rendererImplementation)
        rendererImplementation->InvalidateColors();
}

// ****************************************************************************
// Method: avtMoleculeRenderer::SetLevelsLUT
//
// Purpose:
//   Sets a lookup table for the renderer to use.
//
// Programmer: Brad Whitlock
// Creation:   Wed Mar 29 12:02:11 PDT 2006
//
// Modifications:
//
// ****************************************************************************

void
avtMoleculeRenderer::SetLevelsLUT(avtLookupTable *lut)
{
    levelsLUT = lut;

    if (rendererImplementation)
        rendererImplementation->SetLevelsLUT(lut);
}

// ****************************************************************************
// Method:  avtMoleculeRenderer::SetIs2D
//
// Purpose:
//   Tell the renderer it's in 2D mode.
//
// Arguments:
//   twoD       true for 2D, false for 3D
//
// Programmer:  Jeremy Meredith
// Creation:    April 22, 2010
//
// ****************************************************************************

void
avtMoleculeRenderer::SetIs2D(bool twoD)
{
    is2D = twoD;
}
