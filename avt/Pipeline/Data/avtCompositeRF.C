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
//                               avtCompositeRF.C                            //
// ************************************************************************* //

#include <avtCompositeRF.h>

#include <float.h>

#include <avtLightingModel.h>
#include <avtOpacityMap.h>
#include <avtPointExtractor.h>
#include <avtRay.h>


// ****************************************************************************
//  Method: avtCompositeRF constructor
//
//  Arguments:
//      l       The lighting model to use.
//      m       The opacity map.
//      sm      The opacity map for the secondary variable.
//
//  Programmer: Hank Childs
//  Creation:   December 1, 2000
//
//  Modifications:
//
//    Hank Childs, Sun Dec  2 15:55:28 PST 2001
//    Add support for secondary variables.
//
//    Hank Childs, Sat Jan  7 17:50:22 PST 2006
//    Add support for kernel based sampling.
//
// ****************************************************************************

avtCompositeRF::avtCompositeRF(avtLightingModel *l, avtOpacityMap *m,
                               avtOpacityMap *sm)
    : avtRayFunction(l)
{
    map = m;
    table = map->GetTable();
    secondaryMap = sm;
    secondaryTable = secondaryMap->GetTable();
    colorVariableIndex   = 0;
    opacityVariableIndex = 0;
    weightVariableIndex  = -1;

    int entries = secondaryMap->GetNumberOfTableEntries();
    double *opacities = new double[entries];
    const RGBA *table = secondaryMap->GetTable();
    for (int i = 0 ; i < entries ; i++)
    {
        opacities[i] = table[i].A;
    }
    rangeMaxTable.SetTable(entries, opacities);
    // No need to free "opacities", since the rangeMaxTable now owns it.
}


// ****************************************************************************
//  Method: avtCompositeRF destructor
//
//  Purpose:
//      Defines the destructor.  Note: this should not be inlined in the header
//      because it causes problems for certain compilers.
//
//  Programmer: Hank Childs
//  Creation:   February 5, 2004
//
// ****************************************************************************

avtCompositeRF::~avtCompositeRF()
{
    ;
}


// ****************************************************************************
//  Method: avtCompositeRF::GetRayValue
//
//  Purpose:
//      Gets the value of the ray based on the sample points and the opacity
//      map.
//
//  Arguments:
//      ray         The ray to composite.
//      gradients   The gradients at each sample point.
//      rgb         The value of the pixel.
//      depth       The depth of rgb.
//
//  Programmer:     Hank Childs
//  Creation:       December 1, 2000
//
//  Modifications:
//
//    Hank Childs, Sat Feb  3 21:03:40 PST 2001
//    Modified routine to return a color instead of a value.
//
//    Hank Childs, Mon Feb 12 17:24:19 PST 2001
//    Made shading happen as ray attenuation.
//
//    Hank Childs, Tue Feb 13 16:27:48 PST 2001
//    Made compositing work with opaque images.
//
//    Hank Chlids, Mon Feb 19 09:01:31 PST 2001
//    Opacity is a float instead of an unsigned char.
//
//    Hank Childs, Tue Sep 18 10:20:35 PDT 2001
//    Cast for compiler warning.
//
//    Hank Childs, Wed Nov 14 14:51:34 PST 2001
//    Add support for multiple variables.
//
//    Hank Childs, Sun Dec  2 15:55:28 PST 2001
//    Full support for multiple variables.
//
//    Hank Childs, Fri Dec  3 14:34:59 PST 2004
//    Allow a fully opaque sample to terminate a ray.  Also make a correction
//    so low opacity samples don't have a huge impact. ['1735]
//
//    Hank Childs, Sun Aug 31 08:04:42 PDT 2008
//    Add support for shading that actually works.
//
//    Hank Childs, Mon Dec 29 09:22:47 PST 2008
//    Fix composite to be a true integration.
//
// ****************************************************************************

void
avtCompositeRF::GetRayValue(const avtRay *ray, 
                            unsigned char rgb[3], double depth)
{
    //
    // Some compilers do very poor optimizations, so make sure that we don't
    // do an offset from the pointer at each step by creating automatic
    // variables and using those.
    //
    const int     numSamples  = ray->numSamples;
    const bool   *validSample = ray->validSample;

    // For right now, only work with one variable.
    const double  *sample      = ray->sample[colorVariableIndex];
    const double  *sample2     = ray->sample[opacityVariableIndex];
    const double  *weight      = NULL;
    double        min_weight  = 0.;
    double        min_weight_denom = 0.;
    if (weightVariableIndex >= 0)
    {
        weight = ray->sample[weightVariableIndex];
        min_weight = avtPointExtractor::GetMinimumWeightCutoff();
        if (min_weight > 0.)
            min_weight_denom = 1./min_weight;
    }

    int maxSample = IndexOfDepth(depth, numSamples);

    static double threshold = 254./255.;

    double opacity = 0.;
    double trgb[3];
    trgb[0] = 0.;
    trgb[1] = 0.;
    trgb[2] = 0.;
    int z;
    double distanceToReachFullOpacity = 1./250.;
    double distanceCoveredPerSample = 1./maxSample;
    double oneSamplesContribution = distanceCoveredPerSample/distanceToReachFullOpacity;
    for (z = 0 ; z < maxSample ; z++)
    {
        if (validSample[z])
        {
            const RGBA &color = table[map->Quantize(sample[z])];
            const RGBA &opac  
                          = secondaryTable[secondaryMap->Quantize(sample2[z])];

            //
            // Only calculate further when we get non-zero opacity.
            //
            if (opac.A > 0)
            {
                double tableOpac = opac.A;
                if (weight != NULL)
                {
                    if (weight[z] < min_weight)
                        tableOpac *= weight[z]*min_weight_denom;
                }
                double samplesOpacity = tableOpac * oneSamplesContribution;
                samplesOpacity = (samplesOpacity > 1. ? 1. : samplesOpacity);

                unsigned char rgb[3] = { color.R, color.G, color.B };
                lighting->AddLighting(z, ray, rgb);

                double ff = (1-opacity)*samplesOpacity;
                trgb[0] = trgb[0] + ff*rgb[0];
                trgb[1] = trgb[1] + ff*rgb[1];
                trgb[2] = trgb[2] + ff*rgb[2];

                opacity = opacity + (1-opacity)*samplesOpacity;
            }
            if (opacity > threshold)
            {
                break;
            }
        }
    }

    if (z >= maxSample)
    {
        //
        // The pixel is not completely opaque, so incorporate the background.
        //
        trgb[0] = trgb[0] + (unsigned char)((1-opacity)*rgb[0]);
        trgb[1] = trgb[1] + (unsigned char)((1-opacity)*rgb[1]);
        trgb[2] = trgb[2] + (unsigned char)((1-opacity)*rgb[2]);
    }

    //
    // Copy the temporary rgb into the output rgb.
    //
    rgb[0] = (unsigned char) trgb[0];
    rgb[1] = (unsigned char) trgb[1];
    rgb[2] = (unsigned char) trgb[2];
}


// ****************************************************************************
//  Method: avtCompositeRF::CanContributeToPicture
//
//  Purpose:
//      Determines if a cell can contribute to the final picture.  Cells with
//      zero opacity all the way through are the only cells that cannot
//      contribute to the final picture.
//
//  Arguments:
//      nVerts  The number of vertices.
//      vals    The variable values.
//
//  Programmer: Hank Childs
//  Creation:   December 7, 2001
//
// ****************************************************************************

bool
avtCompositeRF::CanContributeToPicture(int nVerts,
                                       const double (*vals)[AVT_VARIABLE_LIMIT])
{
    double min = +FLT_MAX;
    double max = -FLT_MIN;
    for (int i = 0 ; i < nVerts ; i++)
    {
        if (vals[i][opacityVariableIndex] < min)
        {
            min = vals[i][opacityVariableIndex];
        }
        if (vals[i][opacityVariableIndex] > max)
        {
            max = vals[i][opacityVariableIndex];
        }
    }

    int  minIndex = secondaryMap->Quantize(min);
    int  maxIndex = secondaryMap->Quantize(max);
    double opacMax = rangeMaxTable.GetMaximumOverRange(minIndex, maxIndex);

    return (opacMax > 0. ? true : false);
}


