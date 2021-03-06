/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkTensorReduceFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2001/03/20 14:10:58 $
  Version:   $Revision: 1.1 $
  Thanks:    Hank Childs, B Division, Lawrence Livermore Nat'l Laboratory

Copyright (c) 1993-2000 Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither name of Ken Martin, Will Schroeder, or Bill Lorensen nor the names
   of any contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "vtkTensorReduceFilter.h"

#include <vtkCell.h>
#include <vtkCellData.h>
#include <vtkDataSet.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkVisItUtility.h>


vtkStandardNewMacro(vtkTensorReduceFilter);


vtkTensorReduceFilter::vtkTensorReduceFilter()
{
  stride = 10;
  numEls = -1;
}


void vtkTensorReduceFilter::SetStride(int s)
{
  numEls = -1;
  stride = s;
}


void vtkTensorReduceFilter::SetNumberOfElements(int n)
{
  stride = -1;
  numEls = n;
}

// ****************************************************************************
// Modifications:
//
//    Kathleen Bonnell, Tue Aug 30 11:11:56 PDT 2005 
//    Copy other Point and Cell data. 
//
//    Kathleen Biagas, Wed Sep 5 13:10:18 MST 2012 
//    Preserve coordinate and tensor data types.
//
// ****************************************************************************

void vtkTensorReduceFilter::Execute(void)
{
  vtkDataSet *input  = this->GetInput();
  vtkPolyData *output = this->GetOutput();

  vtkCellData *inCd = input->GetCellData();
  vtkPointData *inPd = input->GetPointData();
  vtkCellData *outCd = output->GetCellData();
  vtkPointData *outPd = output->GetPointData();

  vtkDataArray *inCtensors = inCd->GetTensors();
  vtkDataArray *inPtensors = inPd->GetTensors();

  int npts = input->GetNumberOfPoints();
  int ncells = input->GetNumberOfCells();

  if (inPtensors == NULL && inCtensors == NULL)
    {
    vtkErrorMacro(<<"No tensors to reduce");
    return;
    }

  int inPType = (inPtensors ? inPtensors->GetDataType() : VTK_FLOAT);
  int inCType = (inCtensors ? inCtensors->GetDataType() : VTK_FLOAT);

  // Determine what the stride is.
  if (stride <= 0 && numEls <= 0)
    {
    vtkErrorMacro(<<"Invalid stride");
    return;
    }

  float actingStride = stride;
  if (actingStride <= 0)
    {
    int totalTensors = 0;
    if (inPtensors != NULL)
    {
        totalTensors += npts;
    }
    if (inCtensors != NULL)
    {
        totalTensors += ncells;
    }
    actingStride = ceil(((float) totalTensors) / ((float) numEls));
    }

  vtkPoints *outpts = vtkVisItUtility::NewPoints(input);
  vtkDataArray *outTensors;
  if (inPType == VTK_DOUBLE || inCType == VTK_DOUBLE)
      outTensors = vtkDoubleArray::New();
  else 
      outTensors = vtkFloatArray::New();
  outTensors->SetNumberOfComponents(9);

  float nextToTake = 0.;
  int count = 0;
  if (inPtensors != NULL)
    {
    outPd->CopyAllocate(inPd, npts);
    outTensors->SetName(inPtensors->GetName());
    for (int i = 0 ; i < npts ; i++)
      {
      if (i >= nextToTake)
        {
        nextToTake += actingStride;

        double pt[3];
        input->GetPoint(i, pt);
        outpts->InsertNextPoint(pt);

        double v[9];
        inPtensors->GetTuple(i, v);
        outTensors->InsertNextTuple(v);
        outPd->CopyData(inPd, i, count++);
        }
      }
      outPd->Squeeze();
    }

  nextToTake = 0.;
  count = 0;
  if (inCtensors != NULL)
    {
    outCd->CopyAllocate(inCd, ncells);
    outTensors->SetName(inCtensors->GetName());
    for (int i = 0 ; i < ncells ; i++)
      {
      if (i >= nextToTake)
        {
        nextToTake += actingStride;

        vtkCell *cell = input->GetCell(i);
        double pt[3];
        cell->GetParametricCenter(pt);
        outpts->InsertNextPoint(pt);

        double v[9];
        inCtensors->GetTuple(i, v);
        outTensors->InsertNextTuple(v);
        outCd->CopyData(inCd, i, count++);
        }
      }
      outCd->Squeeze();
    }

  int nOutPts = outpts->GetNumberOfPoints();
  output->SetPoints(outpts);
  outpts->Delete();
  output->GetPointData()->SetTensors(outTensors);
  outTensors->Delete();

  output->Allocate(nOutPts);
  vtkIdType onevertex[1];
  for (int i = 0 ; i < nOutPts ; i++)
    {
    onevertex[0] = i;
    output->InsertNextCell(VTK_VERTEX, 1, onevertex);
    }
}

  
void vtkTensorReduceFilter::PrintSelf(ostream &os, vtkIndent indent)
{
   this->Superclass::PrintSelf(os, indent);
   os << indent << "Stride: " << this->stride << "\n";
   os << indent << "Target number of tensors: " << this->numEls << "\n";
}

