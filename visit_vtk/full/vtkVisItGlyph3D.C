/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkVisItGlyph3D.cxx,v $
  Language:  C++
  Date:      $Date: 2002/12/17 02:05:38 $
  Version:   $Revision: 1.107 $

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include <vtkVisItGlyph3D.h>

#include <vtkCell.h>
#include <vtkDataSet.h>
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkIdList.h>
#include <vtkIdTypeArray.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkUnsignedCharArray.h>
#include <vtkAppendPolyData.h>

vtkCxxRevisionMacro(vtkVisItGlyph3D, "$Revision: 1.107 $");
vtkStandardNewMacro(vtkVisItGlyph3D);

// Construct object with scaling on, scaling mode is by scalar value,
// scale factor = 1.0, the range is (0,1), orient geometry is on, and
// orientation is by vector. Clamping and indexing are turned off. No
// initial sources are defined.
vtkVisItGlyph3D::vtkVisItGlyph3D()
{
  this->Scaling = 1;
  this->ColorMode = VTK_COLOR_BY_SCALE;
  this->ScaleMode = VTK_SCALE_BY_SCALAR;
  this->ScaleFactor = 1.0;
  this->Range[0] = 0.0;
  this->Range[1] = 1.0;
  this->Orient = 1;
  this->VectorMode = VTK_USE_VECTOR;
  this->Clamping = 0;
  this->IndexMode = VTK_INDEXING_OFF;
  this->NumberOfRequiredInputs = 1;
  this->GeneratePointIds = 0;
  this->TreatVectorsAs2D = 0;
  this->PointIdsName = NULL;
  this->SetPointIdsName("InputPointIds");
  this->InputScalarsSelection = NULL;
  this->InputVectorsSelection = NULL;
  this->InputNormalsSelection = NULL;

  this->ScalarsForColoring = NULL;
  this->ScalarsForScaling = NULL;
  this->VectorsForColoring = NULL;
  this->VectorsForScaling = NULL;
  this->TensorsForScaling = NULL;

  this->UseFullFrameScaling = 0;
  this->FullFrameScaling[0] = 1.;
  this->FullFrameScaling[1] = 1.;
  this->FullFrameScaling[2] = 1.;
}

vtkVisItGlyph3D::~vtkVisItGlyph3D()
{
  if (this->PointIdsName)
    {
    delete []PointIdsName;
    }
  this->SetInputScalarsSelection(NULL);
  this->SetInputVectorsSelection(NULL);
  this->SetInputNormalsSelection(NULL);

  this->SetScalarsForColoring(NULL);
  this->SetScalarsForScaling(NULL);
  this->SetVectorsForColoring(NULL);
  this->SetVectorsForScaling(NULL);
  this->SetTensorsForScaling(NULL);
}

//*****************************************************************************
//  Modifications:
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004
//    Allow user to set different scalars/vectors for coloring and scaling,
//    but still perserved default vtkGlyph3D behavior if those ivars aren't
//    populated.
//
//    Hank Childs, Fri Aug 27 15:15:20 PDT 2004
//    Renamed ghost data arrays.
//
//    Kathleen Bonnell, Tue Oct 12 16:42:16 PDT 2004 
//    Ensure that avtOriginalCellNumbers and avtOriginalNodeNumbers arrays
//    get copied to output (if they exist in input). 
//
//    Kathleen Bonnell, Fri Nov 12 11:50:33 PST 2004
//    Retrieve VectorsForScaling. 
//
//    Hank Childs, Fri Sep 14 09:54:13 PDT 2007
//    Add support for treating vectors as 2D.
//
//    Hank Childs, Fri Jan 29 16:25:29 PST 2010
//    Add support for treating vectors as 2D ... for data that is 10^15.
//
//    Hank Childs, Sun Jun 27 12:18:41 PDT 2010
//    Only glyph points that have VTK_VERTEXs.  Also pass edges, quads, etc
//    through the filter.
//
//    John Schmidt, Thu Nov 15 13:08:21 MST 2012
//    Added capability to scale by a 3x3 tensor.  Use in scaling the Box
//    glyph.
//
//*****************************************************************************
void vtkVisItGlyph3D::Execute()
{
  vtkPointData *pd;
  vtkDataArray *inScalars = NULL;
  vtkDataArray *inScalars_forColoring = NULL;
  vtkDataArray *inScalars_forScaling = NULL;

  vtkDataArray *inVectors = NULL;
  vtkDataArray *inVectors_forColoring = NULL;
  vtkDataArray *inVectors_forScaling = NULL;
  vtkDataArray *inTensors_forScaling = NULL;
  int requestedGhostLevel;
  unsigned char* inGhostLevels=0;
  vtkDataArray *inNormals = NULL, *sourceNormals = NULL;
  vtkIdType numPts, numSourcePts, numSourceCells, i;
  int index;
  vtkPoints *sourcePts = NULL;
  vtkPoints *newPts;
  vtkDataArray *newScalars=NULL;
  vtkDataArray *newVectors=NULL;
  vtkDataArray *newNormals=NULL;
  double x[3], v[3];
  double vNew[3], s = 0.0, vMag = 0.0, value;
  vtkTransform *trans = vtkTransform::New();
  vtkMatrix4x4 *def_mat = vtkMatrix4x4::New();
  vtkCell *cell;
  vtkIdList *cellPts;
  int npts;
  vtkIdList *pts;
  vtkIdType ptIncr, cellId;
  int haveVectors, haveNormals;
  double scalex,scaley,scalez, den;
  vtkPolyData *output = vtkPolyData::New();
  vtkPointData *outputPD = output->GetPointData();
  vtkCellData *outputCD = output->GetCellData();
  vtkDataSet *input = this->GetInput();

  vtkDataArray *inOrigNodes = NULL;
  vtkDataArray *inOrigCells = NULL;
  vtkDataArray *outOrigNodes = NULL;
  vtkDataArray *outOrigCells = NULL;

  int numberOfSources = this->GetNumberOfSources();
  vtkPolyData *defaultSource = NULL;
  vtkIdTypeArray *pointIds=0;

  vtkDebugMacro(<<"Generating glyphs");

  pts = vtkIdList::New();
  pts->Allocate(VTK_CELL_SIZE);

  if (!input)
    {
    vtkErrorMacro(<<"No input");
    return;
    }

  pd = input->GetPointData();
  inScalars = pd->GetScalars(this->InputScalarsSelection);
  inVectors = pd->GetVectors(this->InputVectorsSelection);
  inNormals = pd->GetNormals(this->InputNormalsSelection);

  inScalars_forColoring = pd->GetArray(this->ScalarsForColoring);
  inScalars_forScaling  = pd->GetArray(this->ScalarsForScaling);
  inVectors_forColoring = pd->GetArray(this->VectorsForColoring);
  inVectors_forScaling  = pd->GetArray(this->VectorsForScaling);
  inTensors_forScaling  = pd->GetArray(this->TensorsForScaling);

  inOrigNodes = pd->GetArray("avtOriginalNodeNumbers");
  inOrigCells = pd->GetArray("avtOriginalCellNumbers");

  vtkDataArray* temp = 0;
  if (pd)
    {
    temp = pd->GetArray("avtGhostZones");
    }
  if ( (!temp) || (temp->GetDataType() != VTK_UNSIGNED_CHAR)
    || (temp->GetNumberOfComponents() != 1))
    {
    vtkDebugMacro("No appropriate ghost levels field available.");
    }
  else
    {
    inGhostLevels = ((vtkUnsignedCharArray*)temp)->GetPointer(0);
    }

  requestedGhostLevel = output->GetUpdateGhostLevel();
  
  
  numPts = input->GetNumberOfPoints();
  if (numPts < 1)
    {
    vtkDebugMacro(<<"No points to glyph!");
    pts->Delete();
    trans->Delete();
    return;
    }

  // Check input for consistency
  //
  if ( (den = this->Range[1] - this->Range[0]) == 0.0 )
    {
    den = 1.0;
    }
  if ( this->VectorMode != VTK_VECTOR_ROTATION_OFF &&
       ((this->VectorMode == VTK_USE_VECTOR && inVectors_forScaling != NULL) ||
        (this->VectorMode == VTK_USE_NORMAL && inNormals != NULL)) )
    {
    haveVectors = 1;
    }
  else if ( this->VectorMode != VTK_VECTOR_ROTATION_OFF &&
       ((this->VectorMode == VTK_USE_VECTOR && inVectors != NULL) ||
        (this->VectorMode == VTK_USE_NORMAL && inNormals != NULL)) )
    {
    haveVectors = 1;
    }
  else
    {
    haveVectors = 0;
    }


  if ( (this->IndexMode == VTK_INDEXING_BY_SCALAR && 
       (!inScalars && !inScalars_forScaling)) ||
       (this->IndexMode == VTK_INDEXING_BY_VECTOR &&
       (((!inVectors && !inVectors_forScaling) && 
           this->VectorMode == VTK_USE_VECTOR) ||
        (!inNormals && this->VectorMode == VTK_USE_NORMAL))) )
    {
    if ( this->GetSource(0) == NULL )
      {
      vtkErrorMacro(<<"Indexing on but don't have data to index with");
      pts->Delete();
      trans->Delete();
      def_mat->Delete();
      return;
      }
    else
      {
      vtkWarningMacro(<<"Turning indexing off: no data to index with");
      this->IndexMode = VTK_INDEXING_OFF;
      }
    }

  // Allocate storage for output PolyData
  //
  outputPD->CopyScalarsOff();
  outputPD->CopyVectorsOff();
  outputPD->CopyNormalsOff();

  if (!this->GetSource(0))
    {
    defaultSource = vtkPolyData::New();
    defaultSource->Allocate();
    vtkPoints *defaultPoints = vtkPoints::New();
    defaultPoints->Allocate(6);
    defaultPoints->InsertNextPoint(0, 0, 0);
    defaultPoints->InsertNextPoint(1, 0, 0);
    vtkIdType defaultPointIds[2];
    defaultPointIds[0] = 0;
    defaultPointIds[1] = 1;
    defaultSource->SetPoints(defaultPoints);
    defaultSource->InsertNextCell(VTK_LINE, 2, defaultPointIds);
    defaultSource->SetUpdateExtent(0, 1, 0);
    this->SetSource(defaultSource);
    defaultSource->Delete();
    defaultSource = NULL;
    defaultPoints->Delete();
    defaultPoints = NULL;
    }

  if ( this->IndexMode != VTK_INDEXING_OFF )
    {
    pd = NULL;
    numSourcePts = numSourceCells = 0;
    haveNormals = 1;
    for (numSourcePts=numSourceCells=i=0; i < numberOfSources; i++)
      {
      if ( this->GetSource(i) != NULL )
        {
        numSourcePts += this->GetSource(i)->GetNumberOfPoints();
        numSourceCells += this->GetSource(i)->GetNumberOfCells();
        if ( !(sourceNormals = this->GetSource(i)->GetPointData()->GetNormals()) )
          {
          haveNormals = 0;
          }
        }
      }
    }
  else
    {
    sourcePts = this->GetSource(0)->GetPoints();
    numSourcePts = sourcePts->GetNumberOfPoints();
    numSourceCells = this->GetSource(0)->GetNumberOfCells();

    sourceNormals = this->GetSource(0)->GetPointData()->GetNormals();
    if ( sourceNormals )
      {
      haveNormals = 1;
      }
    else
      {
      haveNormals = 0;
      }

    // Prepare to copy output.
    pd = this->GetSource(0)->GetPointData();
    outputPD->CopyAllocate(pd,numPts*numSourcePts);
    }

  newPts = vtkPoints::New();
  newPts->Allocate(numPts*numSourcePts);
  if ( this->GeneratePointIds )
    {
    pointIds = vtkIdTypeArray::New();
    pointIds->SetName(this->PointIdsName);
    pointIds->Allocate(numPts*numSourcePts);
    outputPD->AddArray(pointIds);
    }
  if ( this->ColorMode == VTK_COLOR_BY_SCALAR && inScalars_forColoring)
    {
    newScalars = inScalars_forColoring->NewInstance();
    newScalars->SetNumberOfComponents(inScalars_forColoring->GetNumberOfComponents());
    newScalars->Allocate(inScalars_forColoring->GetNumberOfComponents()*
                         numPts*numSourcePts);
    newScalars->SetName(inScalars_forColoring->GetName());
    }
  else if ( this->ColorMode == VTK_COLOR_BY_SCALAR && inScalars)
    {
    newScalars = inScalars->NewInstance();
    newScalars->SetNumberOfComponents(inScalars->GetNumberOfComponents());
    newScalars->Allocate(inScalars->GetNumberOfComponents()*numPts*numSourcePts);
    newScalars->SetName(inScalars->GetName());
    }
  else if ( (this->ColorMode == VTK_COLOR_BY_SCALE) && inScalars)
    {
    newScalars = vtkFloatArray::New();
    newScalars->Allocate(numPts*numSourcePts);
    newScalars->SetName("GlyphScale");
    if (this->ScaleMode == VTK_SCALE_BY_SCALAR)
      {
      newScalars->SetName(inScalars->GetName());
      }
    }
  else if ( (this->ColorMode == VTK_COLOR_BY_VECTOR) && inVectors_forColoring)
    {
    newScalars = vtkFloatArray::New();
    newScalars->Allocate(numPts*numSourcePts);
    newScalars->SetName("VectorMagnitude");
    }
  else if ( (this->ColorMode == VTK_COLOR_BY_VECTOR) && haveVectors)
    {
    newScalars = vtkFloatArray::New();
    newScalars->Allocate(numPts*numSourcePts);
    newScalars->SetName("VectorMagnitude");
    }
  if ( haveVectors )
    {
    newVectors = vtkFloatArray::New();
    newVectors->SetNumberOfComponents(3);
    newVectors->Allocate(3*numPts*numSourcePts);
    newVectors->SetName("GlyphVector");
    }
  if ( haveNormals )
    {
    newNormals = vtkFloatArray::New();
    newNormals->SetNumberOfComponents(3);
    newNormals->Allocate(3*numPts*numSourcePts);
    newNormals->SetName("Normals");
    }
  if ( inOrigNodes )
    {
    outOrigNodes = inOrigNodes->NewInstance();
    outOrigNodes->SetNumberOfComponents(inOrigNodes->GetNumberOfComponents());
    outOrigNodes->Allocate(inOrigNodes->GetNumberOfComponents()*numSourceCells*numPts);
    outOrigNodes->SetName(inOrigNodes->GetName());
    }
  if ( inOrigCells )
    {
    outOrigCells = inOrigCells->NewInstance();
    outOrigCells->SetNumberOfComponents(inOrigCells->GetNumberOfComponents());
    outOrigCells->Allocate(inOrigCells->GetNumberOfComponents()*numSourceCells*numPts);
    outOrigCells->SetName(inOrigCells->GetName());
    }

  int connSize = 0;
  int numVerts = 0;
  int numCells = input->GetNumberOfCells();
  vtkIdList *ptIds = vtkIdList::New();
  for (i = 0 ; i < numCells ; i++)
  {
      vtkIdType c = input->GetCellType(i);
      if (c == VTK_VERTEX)
          numVerts++;
      else if (c == VTK_POLY_VERTEX)
      {
          input->GetCellPoints(i, ptIds);
          numVerts += ptIds->GetNumberOfIds();
      }
      else
      {
          input->GetCellPoints(i, ptIds);
          connSize += ptIds->GetNumberOfIds()+1;
      }
  }
  // Setting up for calls to PolyData::InsertNextCell()
  if (this->IndexMode != VTK_INDEXING_OFF )
    {
    output->Allocate(3*numPts*numSourceCells,numPts*numSourceCells);
    }
  else
    {
    output->Allocate(this->GetSource(0),3*numPts*numSourceCells,numPts*numSourceCells);
    }

  // Traverse all Input points, transforming Source points and copying
  // point attributes.
  //
  ptIncr=0;
  for (int cellIdx = 0 ; cellIdx < numCells ; cellIdx++)
    {
    vtkIdType c = input->GetCellType(cellIdx);
    if (c != VTK_VERTEX && c != VTK_POLY_VERTEX)
       continue;
    input->GetCellPoints(cellIdx, ptIds);
    // only 1 iteration for VTK_VERTEX, multiple for VTK_POLY_VERTEX
    for (int j = 0 ; j < ptIds->GetNumberOfIds() ; j++) 
    {
    vtkIdType inPtId = ptIds->GetId(j);
    scalex = scaley = scalez = 1.0;
    if ( (cellIdx % 10000) == 0 )
      {
      this->UpdateProgress ((double)inPtId/numPts);
      if (this->GetAbortExecute())
        {
        break;
        }
      }

    // Get the scalar and vector data
    if ( inTensors_forScaling )
      {
        if (this->ScaleMode == VTK_SCALE_BY_TENSOR) 
          {
          // def_mat is Identity at its creation, only change needed elements.
          double* tensor = inScalars_forScaling->GetTuple9(inPtId);
          def_mat->SetElement(0,0,tensor[0]);
          def_mat->SetElement(0,1,tensor[1]);
          def_mat->SetElement(0,2,tensor[2]);

          def_mat->SetElement(1,0,tensor[3]);
          def_mat->SetElement(1,1,tensor[4]);
          def_mat->SetElement(1,2,tensor[5]);

          def_mat->SetElement(2,0,tensor[6]);
          def_mat->SetElement(2,1,tensor[7]);
          def_mat->SetElement(2,2,tensor[8]);
          }
      }
    else if ( inScalars_forScaling )
      {
      s = inScalars_forScaling->GetComponent(inPtId, 0);
      if ( this->ScaleMode == VTK_SCALE_BY_SCALAR ||
           this->ScaleMode == VTK_DATA_SCALING_OFF )
        {
        scalex = scaley = scalez = s;
        }
      }
    else if ( inScalars )
      {
      s = inScalars->GetComponent(inPtId, 0);
      if ( this->ScaleMode == VTK_SCALE_BY_SCALAR ||
           this->ScaleMode == VTK_DATA_SCALING_OFF )
        {
        scalex = scaley = scalez = s;
        }
      }
    
    if ( haveVectors )
      {
      if ( this->VectorMode == VTK_USE_NORMAL )
        {
        inNormals->GetTuple(inPtId, v);
        }
      else if (inVectors_forScaling)
        {
        inVectors_forScaling->GetTuple(inPtId, v);
        }
      else 
        {
        inVectors->GetTuple(inPtId, v);
        }
      if (this->TreatVectorsAs2D)
        {
        v[2] = 0.;
        }
      vMag = vtkMath::Norm(v);
      if ( this->ScaleMode == VTK_SCALE_BY_VECTORCOMPONENTS )
        {
        scalex = v[0];
        scaley = v[1];
        scalez = v[2];
        }
      else if ( this->ScaleMode == VTK_SCALE_BY_VECTOR )
        {
        scalex = scaley = scalez = vMag;
        }
      }
    
    // Clamp data scale if enabled
    if ( this->Clamping )
      {
      scalex = (scalex < this->Range[0] ? this->Range[0] :
                (scalex > this->Range[1] ? this->Range[1] : scalex));
      scalex = (scalex - this->Range[0]) / den;
      scaley = (scaley < this->Range[0] ? this->Range[0] :
                (scaley > this->Range[1] ? this->Range[1] : scaley));
      scaley = (scaley - this->Range[0]) / den;
      scalez = (scalez < this->Range[0] ? this->Range[0] :
                (scalez > this->Range[1] ? this->Range[1] : scalez));
      scalez = (scalez - this->Range[0]) / den;
      }
    
    // Compute index into table of glyphs
    index = 0;
    if ( this->IndexMode == VTK_INDEXING_OFF )
      {
      index = 0;
      }
    else 
      {
      if ( this->IndexMode == VTK_INDEXING_BY_SCALAR )
        {
        value = s;
        }
      else
        {
        value = vMag;
        }
      
      index = (int) ((double)(value - this->Range[0]) * numberOfSources / den);
      index = (index < 0 ? 0 :
              (index >= numberOfSources ? (numberOfSources-1) : index));
      
      if ( this->GetSource(index) != NULL )
        {
        sourcePts = this->GetSource(index)->GetPoints();
        sourceNormals = this->GetSource(index)->GetPointData()->GetNormals();
        numSourcePts = sourcePts->GetNumberOfPoints();
        numSourceCells = this->GetSource(index)->GetNumberOfCells();
        }
      }
    
    // Make sure we're not indexing into empty glyph
    if ( this->GetSource(index) == NULL )
      {
      continue;
      }

    // Check ghost points.
    // If we are processing a piece, we do not want to duplicate 
    // glyphs on the borders.  The corrct check here is:
    // ghostLevel > 0.  I am leaving this over glyphing here because
    // it make a nice example (sphereGhost.tcl) to show the 
    // point ghost levels with the glyph filter.  I am not certain 
    // of the usefullness of point ghost levels over 1, but I will have
    // to think about it.
    if (inGhostLevels && inGhostLevels[inPtId] > requestedGhostLevel)
      {
      continue;
      }
    
    // Now begin copying/transforming glyph
    trans->Identity();

    // Make 100% sure this is putting the glyphs at Z=0 for 2D.  Floating
    // point error can creep in with 10^15 extents.
    if (this->TreatVectorsAs2D == 1)
    {
        trans->Scale(1,1,0);
    }
   
    double *inNode = NULL; 
    double *inCell = NULL; 
    if (inOrigNodes)
    {
        inNode = inOrigNodes->GetTuple(inPtId);
    }
    if (inOrigCells)
    {
        inCell = inOrigCells->GetTuple(inPtId);
    }

    // Copy all topology (transformation independent)
    for (cellId=0; cellId < numSourceCells; cellId++)
      {
      cell = this->GetSource(index)->GetCell(cellId);
      cellPts = cell->GetPointIds();
      npts = cellPts->GetNumberOfIds();
      for (pts->Reset(), i=0; i < npts; i++) 
        {
        pts->InsertId(i,cellPts->GetId(i) + ptIncr);
        }
      output->InsertNextCell(cell->GetCellType(),pts);
      if (outOrigNodes)
        {
        outOrigNodes->InsertNextTuple(inNode);
        }
      if (outOrigCells)
        {
        outOrigCells->InsertNextTuple(inCell);
        }
      }
    
    // translate Source to Input point
    input->GetPoint(inPtId, x);
    trans->Translate(x[0], x[1], x[2]);

    // Transform Source by Tensor
    trans->Concatenate(def_mat);    
    
    if ( haveVectors )
      {
      // Copy Input vector
      for (i=0; i < numSourcePts; i++) 
        {
        newVectors->InsertTuple(i+ptIncr, v);
        }
      if (this->Orient && (vMag > 0.0))
        {
        // if there is no y or z component
        if ( v[1] == 0.0 && v[2] == 0.0 )
          {
          if (v[0] < 0) //just flip x if we need to
            {
            trans->RotateWXYZ(180.0,0,1,0);
            }
          }
        else
          {
          vNew[0] = (v[0]+vMag) / 2.0;
          vNew[1] = v[1] / 2.0;
          vNew[2] = v[2] / 2.0;
          trans->RotateWXYZ((double)180.0,vNew[0],vNew[1],vNew[2]);
          }
        }
      }
    
    // determine scale factor from scalars if appropriate
    if ( inScalars_forColoring || inScalars )
      {
      // Copy scalar value
      if (this->ColorMode == VTK_COLOR_BY_SCALE)
        {
        for (i=0; i < numSourcePts; i++) 
          {
          newScalars->InsertTuple(i+ptIncr, &scalex); // = scaley = scalez
          }
        }
      else if (this->ColorMode == VTK_COLOR_BY_SCALAR && inScalars_forColoring)
        {
        for (i=0; i < numSourcePts; i++)
          {
          outputPD->CopyTuple(inScalars_forColoring, newScalars, inPtId, ptIncr+i);
          }
        }
      else if (this->ColorMode == VTK_COLOR_BY_SCALAR && inScalars)
        {
        for (i=0; i < numSourcePts; i++)
          {
          outputPD->CopyTuple(inScalars, newScalars, inPtId, ptIncr+i);
          }
        }
      }

    if (inVectors_forColoring && this->ColorMode == VTK_COLOR_BY_VECTOR)
      {
      inVectors_forColoring->GetTuple(inPtId, v);
      vMag = vtkMath::Norm(v);
      for (i=0; i < numSourcePts; i++) 
        {
        newScalars->InsertTuple(i+ptIncr, &vMag);
        }
      }
    else if (haveVectors && this->ColorMode == VTK_COLOR_BY_VECTOR)
      {
      for (i=0; i < numSourcePts; i++) 
        {
        newScalars->InsertTuple(i+ptIncr, &vMag);
        }
      }
    
    // scale data if appropriate
    if ( this->Scaling )
      {
      if ( this->ScaleMode == VTK_DATA_SCALING_OFF )
        {
        scalex = scaley = scalez = this->ScaleFactor;
        }
      else
        {
        scalex *= this->ScaleFactor;
        scaley *= this->ScaleFactor;
        scalez *= this->ScaleFactor;
        }
      
      if ( scalex == 0.0 )
        {
        scalex = 1.0e-10;
        }
      if ( scaley == 0.0 )
        {
        scaley = 1.0e-10;
        }
      if ( scalez == 0.0 )
        {
        scalez = 1.0e-10;
        }
      trans->Scale(scalex,scaley,scalez);
      }

      // If we are using full frame scaling then add an additional
      // transform to undo what fullframe will do.
      if (this->UseFullFrameScaling)
        {
          trans->Scale(1. / this->FullFrameScaling[0],
                       1. / this->FullFrameScaling[1],
                       1. / this->FullFrameScaling[2]);
        }


    // multiply points and normals by resulting matrix
    trans->TransformPoints(sourcePts,newPts);
    
    if ( haveNormals )
      {
      trans->TransformNormals(sourceNormals,newNormals);
      }
    
    // Copy point data from source (if possible)
    if ( pd ) 
      {
      for (i=0; i < numSourcePts; i++)
        {
        outputPD->CopyData(pd,i,ptIncr+i);
        }
      }

    // If point ids are to be generated, do it here
    if ( this->GeneratePointIds )
      {
      for (i=0; i < numSourcePts; i++)
        {
        pointIds->InsertNextValue(inPtId);
        }
      }

    ptIncr += numSourcePts;
    } 
  }
  
  // Update ourselves and release memory
  //
  output->SetPoints(newPts);
  newPts->Delete();

  if (newScalars)
    {
    outputPD->SetScalars(newScalars);
    newScalars->Delete();
    }

  if (newVectors)
    {
    outputPD->SetVectors(newVectors);
    newVectors->Delete();
    }

  if (newNormals)
    {
    outputPD->SetNormals(newNormals);
    newNormals->Delete();
    }
  if (outOrigNodes)
    {
    outputCD->AddArray(outOrigNodes);
    outOrigNodes->Delete();
    }
  if (outOrigCells)
    {
    outputCD->AddArray(outOrigCells);
    outOrigCells->Delete();
    }

  output->Squeeze();
  trans->Delete();
  def_mat->Delete();
  pts->Delete();

  if (connSize > 0 && (input->GetDataObjectType() == VTK_POLY_DATA || 
                       input->GetDataObjectType() == VTK_UNSTRUCTURED_GRID))
  {
     vtkPolyData *in_polydata = (vtkPolyData *) input;

     // we have non-verts ... make a separate output for the non-verts and then 
     // append them together.
     vtkPolyData *output2 = vtkPolyData::New();
     output2->SetPoints(in_polydata->GetPoints());
     output2->GetPointData()->ShallowCopy(in_polydata->GetPointData());
     // ignore cell data ... it won't match up with glyphed verts anyway.
     output2->Allocate(connSize);
     for (i = 0 ; i < numCells ; i++)
     {
       vtkIdType c = input->GetCellType(i);
       if (c == VTK_VERTEX || c == VTK_POLY_VERTEX)
           continue;
       input->GetCellPoints(i, ptIds);
       output2->InsertNextCell(c, ptIds);
     }
     vtkAppendPolyData *appender = vtkAppendPolyData::New();
     appender->AddInput(output2);
     appender->AddInput(output);
     appender->Update();
     GetOutput()->ShallowCopy(appender->GetOutput());
     output2->Delete();
     appender->Delete();
  }
  else
     GetOutput()->ShallowCopy(output);
  output->Delete();
  ptIds->Delete();
}

//----------------------------------------------------------------------------
// Since indexing determines size of outputs, EstimatedWholeMemorySize is
// truly an estimate.  Ignore Indexing (although for a best estimate we
// should average the size of the sources instead of using 0).
void vtkVisItGlyph3D::ExecuteInformation()
{
  if (this->GetInput() == NULL)
    {
    vtkErrorMacro("Missing input");
    return;
    }
}


// Set the number of source objects in the glyph table. This should be
// done prior to specifying more than one source.
void vtkVisItGlyph3D::SetNumberOfSources(int num)
{
  // one more because input has index 0.
  this->SetNumberOfInputs(num+1);
}

int vtkVisItGlyph3D::GetNumberOfSources()
{
  // one less because input has index 0.
  return this->NumberOfInputs - 1;
}

// Specify a source object at a specified table location.
void vtkVisItGlyph3D::SetSource(int id, vtkPolyData *pd)
{
  if (id < 0)
    {
    vtkErrorMacro("Bad index " << id << " for source.");
    return;
    }
  this->vtkProcessObject::SetNthInput(id + 1, pd);
}

// Get a pointer to a source object at a specified table location.
vtkPolyData *vtkVisItGlyph3D::GetSource(int id)
{
  if ( id < 0 || id >= this->GetNumberOfSources() )
    {
    return NULL;
    }
  else
    {
    return (vtkPolyData *)this->Inputs[id+1];
    }
}

void vtkVisItGlyph3D::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Generate Point Ids "
     << (this->GeneratePointIds ? "On\n" : "Off\n");

  os << indent << "PointIdsName: " << (this->PointIdsName ? this->PointIdsName
       : "(none)") << "\n";

  os << indent << "Color Mode: " << this->GetColorModeAsString() << endl;

  if ( this->GetNumberOfSources() < 2 )
    {
    if ( this->GetSource(0) != NULL )
      {
      os << indent << "Source: (" << this->GetSource(0) << ")\n";
      }
    else
      {
      os << indent << "Source: (none)\n";
      }
    }
  else
    {
    os << indent << "A table of " << this->GetNumberOfSources() << " glyphs has been defined\n";
    }

  os << indent << "Scaling: " << (this->Scaling ? "On\n" : "Off\n");
  
  os << indent << "Scale Mode: ";
  if ( this->ScaleMode == VTK_SCALE_BY_SCALAR )
    {
    os << "Scale by scalar\n";
    }
  else if ( this->ScaleMode == VTK_SCALE_BY_VECTOR )
    {
    os << "Scale by vector\n";
    }
  else
    {
    os << "Data scaling is turned off\n";
    }

  os << indent << "Scale Factor: " << this->ScaleFactor << "\n";
  os << indent << "Clamping: " << (this->Clamping ? "On\n" : "Off\n");
  os << indent << "Range: (" << this->Range[0] << ", " << this->Range[1] << ")\n";
  os << indent << "Orient: " << (this->Orient ? "On\n" : "Off\n");
  os << indent << "Orient Mode: " << (this->VectorMode == VTK_USE_VECTOR ?
                                       "Orient by vector\n" : "Orient by normal\n");
  os << indent << "Index Mode: ";
  if ( this->IndexMode == VTK_INDEXING_BY_SCALAR )
    {
    os << "Index by scalar value\n";
    }
  else if ( this->IndexMode == VTK_INDEXING_BY_VECTOR )
    {
    os << "Index by vector value\n";
    }
  else
    {
    os << "Indexing off\n";
    }
  os << indent << "InputScalarsSelection: " 
     << (this->InputScalarsSelection ? this->InputScalarsSelection : "(none)") << "\n";
  os << indent << "InputVectorsSelection: " 
     << (this->InputVectorsSelection ? this->InputVectorsSelection : "(none)") << "\n";
  os << indent << "InputNormalsSelection: " 
     << (this->InputNormalsSelection ? this->InputNormalsSelection : "(none)") << "\n";
}

void vtkVisItGlyph3D::ComputeInputUpdateExtents( vtkDataObject *output )
{
  vtkPolyData *outPd;

  if (this->GetInput() == NULL)
    {
    vtkErrorMacro("Missing input");
    return;
    }

  output = output;
  outPd = this->GetOutput();
  if (this->GetSource())
    {
    this->GetSource()->SetUpdateExtent(0, 1, 0);
    }
  this->GetInput()->SetUpdateExtent(outPd->GetUpdatePiece(),
                                    outPd->GetUpdateNumberOfPieces(),
                                    outPd->GetUpdateGhostLevel());
  this->GetInput()->RequestExactExtentOn();
}

int
vtkVisItGlyph3D::SetFullFrameScaling(int useIt, const double *s)
{
  int ret = ((useIt != this->UseFullFrameScaling) ||
             (s[0] != this->FullFrameScaling[0]) ||
             (s[1] != this->FullFrameScaling[1]) ||
             (s[2] != this->FullFrameScaling[2])) ? 1 : 0;

  this->UseFullFrameScaling = useIt;

  if(useIt)
  {
      this->FullFrameScaling[0] = s[0];
      this->FullFrameScaling[1] = s[1];
      this->FullFrameScaling[2] = s[2];
  }
  else
  {
      this->FullFrameScaling[0] = 1.;
      this->FullFrameScaling[1] = 1.;
      this->FullFrameScaling[2] = 1.;
  }

  if(ret==1)
      this->Modified();

  return ret;
}
