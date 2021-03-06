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
//                            avtWellBoreFilter.h                            //
// ************************************************************************* //

#ifndef AVT_WellBore_FILTER_H
#define AVT_WellBore_FILTER_H


#include <avtDataTreeIterator.h>
#include <WellBoreAttributes.h>

#include <string>
#include <vector>

class vtkCellArray;
class vtkFloatArray;
class vtkPoints;
class vtkPolyData;

#define MAX_DETAIL_LEVELS 4

// ****************************************************************************
//  Class: avtWellBoreFilter
//
//  Purpose:
//      This operator is the implied operator associated with an WellBore plot.
//
//  Programmer: brugger -- generated by xml2avt
//  Creation:   Wed Aug 27 14:59:19 PST 2008
//
//  Modifications:
//      Eric Brugger, Mon Nov 10 13:06:30 PST 2008
//      Added the ability to add a well bore name and stem.
//
// ****************************************************************************

class avtWellBoreFilter : public avtSIMODataTreeIterator
{
  public:
                              avtWellBoreFilter(WellBoreAttributes &wb_atts);
    virtual                  ~avtWellBoreFilter();

    virtual const char       *GetType(void)   { return "avtWellBoreFilter"; };

    virtual const char       *GetDescription(void)
                                  { return "Performing WellBore"; };

    void                      SetAttributes(const WellBoreAttributes &wb_atts);

  protected:
    WellBoreAttributes        atts;

    float                    *cyl_pts[MAX_DETAIL_LEVELS];

    virtual avtDataTree_p     ExecuteDataTree(vtkDataSet *, int, std::string);
    virtual void              RefashionDataObjectInfo(void);

    void                      CalculateCylPts(void);
    bool                      GetPoint(int [3], const std::vector<int> &,
                                  int &);
    vtkPolyData              *CreateWell(const std::vector<int> &, int &,
                                  const std::string &, float *, float *, float *,
                                  float *, int [3], int [3]);
    void                      AddWellSegment(vtkPoints *, vtkFloatArray *,
                                  vtkCellArray *, vtkCellArray *, int &,
                                  double [3], double [3]);
};


#endif
