/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkDataSetRemoveGhostCells.C,v $
  Language:  C++
  Date:      $Date: 2001/05/01 20:49:40 $
  Version:   $Revision: 1.1 $


Copyright (c) 1993-2001 Ken Martin, Will Schroeder, Bill Lorensen 
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
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkDataSetRemoveGhostCells.h"

#include <vtkCellData.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkIdTypeArray.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkRectilinearGrid.h>
#include <vtkStructuredGrid.h>
#include <vtkUnsignedCharArray.h>
#include <vtkUnstructuredGrid.h>

#include <vtkVisItUtility.h>
#include <vtkVisItExtractGrid.h>
#include <vtkVisItExtractRectilinearGrid.h>

#include <avtGhostData.h>

//------------------------------------------------------------------------------
// Modifications:
//   Kathleen Bonnell, Wed Mar  6 13:48:48 PST 2002
//   Use vtk Macro instead of old New method.
//------------------------------------------------------------------------------
vtkStandardNewMacro(vtkDataSetRemoveGhostCells);


// ****************************************************************************
// Modifications:
//
//   Hank Childs, Mon Aug 30 16:27:16 PDT 2004
//   Remove references to ghost level, since it is no longer a data member.
//
//   Hank Childs, Fri Aug  3 16:56:36 PDT 2007
//   Initialize GhostNodeTypesToRemove.
//
//   Hank Childs, Sun Oct 28 10:48:50 PST 2007
//   Initialize GhostZoneTypesToRemove.
//
// ****************************************************************************

// Construct with ghost level = 1.
vtkDataSetRemoveGhostCells::vtkDataSetRemoveGhostCells()
{
    GhostNodeTypesToRemove = 255;
    GhostZoneTypesToRemove = 255;
}


void vtkDataSetRemoveGhostCells::Execute()
{
  vtkDataSet *input = this->GetInput();

  switch(input->GetDataObjectType())
  {

      case VTK_RECTILINEAR_GRID :
          RectilinearGridExecute();
          break;
      case VTK_POLY_DATA :
          PolyDataExecute();
          break;

      case VTK_UNSTRUCTURED_GRID :
          UnstructuredGridExecute();
          break;

      case VTK_STRUCTURED_GRID :
          StructuredGridExecute();
          break;

      default: 
          vtkDebugMacro(<<"vtkDataSetRemoveGhostCells not set up to "
                          "operate on this data type" );
          vtkDataSet *output = this->GetOutput();
          output->DeepCopy(input); 
          break;
  }
}

// ****************************************************************************
//  Modifications:
//
//    Hank Childs, Thu Mar  2 11:14:53 PST 2006
//    Created.
//
//    Hank Childs, Fri Jun  9 12:54:36 PDT 2006
//    Remove unused variables.
//
//    Jeremy Meredith, Tue Oct 14 15:30:07 EDT 2008
//    Moved the count of the number of output cells up sooner, and
//    use it to determine if all the cells are selected -- and if they
//    are, then just short-circuit out.
//
// ****************************************************************************

void vtkDataSetRemoveGhostCells::GenericExecute()
{
    int  i;

    vtkDataSet *ds = this->GetInput();
    vtkDataArray *arr = ds->GetCellData()->GetArray("avtGhostZones");
    if (arr == NULL)
    {
        GetOutput()->ShallowCopy(ds);
        return;
    }
    int nOut = 0;
    int nCells = ds->GetNumberOfCells();
    for (i = 0 ; i < nCells ; i++)
        if (arr->GetTuple1(i) == 0)
            nOut++;

    // If *all* the cells are selected, exit early, returning the input
    if (nOut == nCells)
    {
        GetOutput()->ShallowCopy(ds);
        return;
    }
     
    vtkPoints *ptsObj = vtkVisItUtility::GetPoints(ds);
    vtkUnstructuredGrid *ugrid = vtkUnstructuredGrid::New();
    ugrid->SetPoints(ptsObj);
    ptsObj->Delete();
    ugrid->GetPointData()->ShallowCopy(ds->GetPointData());
    ugrid->Allocate(8*nOut);
   
    int cellId = 0;
    vtkCellData *inCD  = ds->GetCellData();
    vtkCellData *outCD = ugrid->GetCellData();
    outCD->CopyAllocate(inCD, nOut);
    vtkIdList *ptList = vtkIdList::New();
    for (i = 0 ; i < nCells ; i++)
    {
        if (arr->GetTuple1(i) != 0)
            continue;
   
        ds->GetCellPoints(i, ptList);
        int type = ds->GetCellType(i);
        ugrid->InsertNextCell(type, ptList);
        outCD->CopyData(inCD, i, cellId++);
    }
    ptList->Delete();

    ugrid->Squeeze();
    SetOutput(ugrid);
    ugrid->Delete();
}


// ***************************************************************************
//  Modifications:
//
//    Kathleen Bonnell, Fri Aug 31 08:50:30 PDT 2001
//    Removed check of field data.  No longer necessary with new vtk.
//
//    Hank Childs, Fri Oct 19 08:33:28 PDT 2001
//    Fix memory leak.
//
//    Hank Childs, Sat Jun 29 16:27:41 PDT 2002
//    Copy over field data.
//
//    Kathleen Bonnell, Wed Jul 10 16:02:56 PDT 2002 
//    Reworked code to utilize vtk RemoveGhostCells method. 
//
//    Hank Childs, Thu Aug 26 10:53:28 PDT 2004
//    Renamed ghost arrays.  Added logic to remove cells that are ghosted,
//    since we can no longer use VTK's routine.
//
//    Hank Childs, Sun Oct 28 10:48:50 PST 2007
//    Added support for GhostZoneTypesToRemove.
//
//    Hank Childs & Cyrus Harrison, Mon Jan 14 08:36:03 PST 2008
//    Removed shallow copy of input dataset b/c this would copy the links
//    (if they existed) potentially providing bogus connectivity info after
//    the ghost zones were removed.
//
// ****************************************************************************

void vtkDataSetRemoveGhostCells::UnstructuredGridExecute()
{
  vtkIdType i;

  vtkDebugMacro(<< "Executing remove ghost cells filter for unstructured grid");
 
  vtkUnstructuredGrid *input  = (vtkUnstructuredGrid*)this->GetInput();
  vtkUnstructuredGrid *output = (vtkUnstructuredGrid*)this->GetOutput();
 
  output->vtkPointSet::ShallowCopy(input);
  output->SetPoints(input->GetPoints());
  output->GetPointData()->ShallowCopy(input->GetPointData());
  output->GetFieldData()->ShallowCopy(input->GetFieldData());

  vtkDataArray *arr = input->GetCellData()->GetArray("avtGhostZones");
  if (arr == NULL)
  {
      output->GetCellData()->ShallowCopy(input->GetCellData());
      output->SetCells(input->GetCellTypesArray(), input->GetCellLocationsArray(),
                       input->GetCells());
      return;
  }
  if (arr->GetDataType() != VTK_UNSIGNED_CHAR)
  {
      vtkErrorMacro("Can only operate on unsigned char ghost data");
      return;
  }
  vtkUnsignedCharArray *ghosts = (vtkUnsignedCharArray *) arr;

  vtkIdType ncells = input->GetNumberOfCells();
  int totalSize = input->GetCells()->GetSize();

  // Over-allocate for now.
  vtkIdType *buff = new vtkIdType[totalSize];
  vtkIdType *b = buff;

  vtkUnsignedCharArray *cellTypes = vtkUnsignedCharArray::New();
  cellTypes->SetNumberOfValues(ncells);
  unsigned char *ct = cellTypes->GetPointer(0);

  vtkIdTypeArray *cellLocations = vtkIdTypeArray::New();
  cellLocations->SetNumberOfValues(ncells);
  vtkIdType *cl = cellLocations->GetPointer(0);

  vtkCellData *inCD = input->GetCellData();
  vtkCellData *outCD = output->GetCellData();
  outCD->CopyAllocate(inCD, ncells);
  int currentIndex = 0;
  vtkIdType cellId = 0;
  for (i = 0 ; i < ncells ; i++)
  {
      unsigned char effectiveVal =ghosts->GetValue(i) & GhostZoneTypesToRemove;
      if (avtGhostData::IsGhostZone(effectiveVal))
          continue;

      vtkIdType npts;
      vtkIdType *pts;
      input->GetCellPoints(i, npts, pts);
      *ct++ = input->GetCellType(i);
      *cl++ = currentIndex;
      *b++ = npts;
      currentIndex += npts+1;
      for (vtkIdType j = 0 ; j < npts ; j++)
          *b++ = pts[j];
      outCD->CopyData(inCD, i, cellId++);
  }

  // We copied all of the cell information into a buffer.  Now that we know
  // the correct size, copy it into a properly typed VTK object.
  vtkIdTypeArray *nlist = vtkIdTypeArray::New();
  nlist->SetNumberOfValues(currentIndex);
  vtkIdType *nl = nlist->GetPointer(0);
  for (i = 0 ; i < currentIndex ; i++) 
      nl[i] = buff[i];
  delete [] buff;

  vtkCellArray *cells = vtkCellArray::New();
  cells->SetCells(cellId, nlist);
  nlist->Delete();

  output->SetCells(cellTypes, cellLocations, cells);
  cellTypes->Delete();
  cellLocations->Delete();
  cells->Delete();

  output->Squeeze();
  if (GhostZoneTypesToRemove == 255)
    output->GetCellData()->RemoveArray("avtGhostZones");
}


// ***************************************************************************
//  Modifications:
//
//    Kathleen Bonnell, Fri Aug 31 08:50:30 PDT 2001
//    Removed check of field data, as it is no longer necessary with VTK 4.0 
//    Made pts of type vtkIdType to match VTK 4.0 API.
//
//    Hank Childs, Sat Jun 29 16:27:41 PDT 2002
//    Copy over field data.
//
//    Kathleen Bonnell, Wed Jul 10 16:02:56 PDT 2002 
//    Reworked code to utilize vtk RemoveGhostCells method. 
//
//    Hank Childs, Fri Sep 13 20:19:52 PDT 2002
//    Reverted back to removing ghost zones by hand since the VTK
//    implementation is buggy.
//
//    Hank Childs, Sun Jun 27 09:49:22 PDT 2004
//    Account for ghost nodes as well as ghost zones.
//
//    Jeremy Meredith, Wed Jul 21 17:37:09 PDT 2004
//    I added a new value for ghost nodes that has new meaning.  Specifically
//    *all* nodes had to be nonzero in a cell for that cell to be removed.
//    With my changes, if *any* node for a cell is "2", then that cell is
//    removed.  This is consistent with the concept of IBLANKing for nodal
//    data, because when a nodal value is meant to be hidden, you must remove
//    any cells that contain that node.  There are other uses for ghost
//    nodes, in particular for removing the polygons between domains.  See
//    http://www.nas.nasa.gov/FAST/RND-93-010.walatka-clucas/htmldocs/chp5.file_io.html
//    for a discussion of this -- the relevant pieces have been pasted into
//    avtOVERFLOWFileFormat::GetMesh.
//
//    Hank Childs, Wed Aug 11 15:33:31 PDT 2004
//    Allow for the presence of both ghost zones and ghost nodes.  This can
//    happen when ghost zones are created by the database and then ghost nodes
//    are created by the reflect filter, for example.  Also remove ghost nodes
//    array when we are done with it, since it just slows down processing.
//
//    Hank Childs, Fri Aug 27 15:10:50 PDT 2004
//    Use the new ghost data names.
//
//    Hank Childs, Fri Aug  3 13:34:38 PDT 2007
//    Added support for GhostNodeTypesToRemove.
//
//    Hank Childs, Sun Oct 28 10:48:50 PST 2007
//    Added support for GhostZoneTypesToRemove.
//
// ***************************************************************************

void vtkDataSetRemoveGhostCells::PolyDataExecute()
{
  vtkPolyData *input   = (vtkPolyData*) this->GetInput();
  vtkPolyData *output  = (vtkPolyData*) this->GetOutput();

  vtkUnsignedCharArray *ghost_zones = (vtkUnsignedCharArray *)
                              input->GetCellData()->GetArray("avtGhostZones");

  vtkUnsignedCharArray *ghost_nodes = (vtkUnsignedCharArray *)
                              input->GetPointData()->GetArray("avtGhostNodes");

  if ((ghost_zones == NULL) && (ghost_nodes == NULL))
    {
    output->ShallowCopy(input);
    return;
    }

  output->SetPoints(input->GetPoints());
  output->GetPointData()->PassData(input->GetPointData());
  output->GetFieldData()->PassData(input->GetFieldData());

  vtkCellData *inCD  = input->GetCellData();
  vtkCellData *outCD = output->GetCellData();
  outCD->CopyAllocate(inCD);
 
  bool usingGhostZones = (ghost_zones != NULL);
  bool usingGhostNodes = (ghost_nodes != NULL);
  unsigned char *zone_ptr = NULL;
  unsigned char *node_ptr = NULL;
  if (usingGhostZones)
    zone_ptr = ghost_zones->GetPointer(0);
  if (usingGhostNodes)
    node_ptr = ghost_nodes->GetPointer(0);

  int nCells = input->GetNumberOfCells();
  output->Allocate(nCells);
  input->BuildCells();
  vtkIdType npts;
  vtkIdType *pts;
  int cell = 0;
  for (int i = 0 ; i < nCells ; i++)
    {
    if (usingGhostZones)
      {
      unsigned char effectiveVal = zone_ptr[i] & GhostZoneTypesToRemove;
      if (avtGhostData::IsGhostZone(effectiveVal))
        {
        continue;
        }
      }

    input->GetCellPoints(i, npts, pts);

    if (usingGhostNodes)
      {
      bool haveOneRealNode = false;
      bool haveOneRemovedNode = false;
      for (int j = 0 ; j < npts ; j++)
        {
        unsigned char effectiveVal = node_ptr[pts[j]] & GhostNodeTypesToRemove;
        if (!avtGhostData::DiscardFaceIfAllNodesAreOfThisType(effectiveVal))
            haveOneRealNode = true;
        if (avtGhostData::DiscardFaceIfOneNodeIsOfThisType(effectiveVal))
            haveOneRemovedNode = true;
        }
      if (!haveOneRealNode || haveOneRemovedNode)
        continue;
      }

    output->InsertNextCell(input->GetCellType(i), npts, pts);
    outCD->CopyData(inCD, i, cell++);
    }
  if (GhostZoneTypesToRemove == 255)
    output->GetCellData()->RemoveArray("avtGhostZones");
  output->GetPointData()->RemoveArray("avtGhostNodes");
  output->Squeeze();
}


// ***************************************************************************
//  Modifications:
//
//    Kathleen Bonnell, Fri Aug 31 08:50:30 PDT 2001
//    VTK 4.0 compatibility issues: Removed check of field data.  
//    Use vtkDataArray for rectilinear grid coordinates instead of vtkScalars,
//    use correct access methods for this array.
//
//    Hank Childs, Sat Jun 29 16:27:41 PDT 2002
//    Copy over field data and set the dimensions.
//
//    Kathleen Bonnell, Wed Jul 10 16:02:56 PDT 2002 
//    Reworked code to utilize vtk ExtractRectilinearGrid filter. 
//    Use new avtRealDims array.
//
//    Hank Childs, Sun Nov  9 13:19:45 PST 2003
//    Tell output that it doesn't have ghost zones, even if we couldn't handle
//    it correctly.  This makes the life of the wireframe filter much easier.
//
//    Hank Childs, Fri Aug 27 15:10:50 PDT 2004
//    Use the new ghost data names.
//
//    Hank Childs, Thu Mar  2 11:14:53 PST 2006
//    Call generic execute if there are no real dims.
//
//    Hank Childs, Sun Oct 28 10:56:37 PST 2007
//    Added support for GhostZoneTypesToRemove.
//
//    Cyrus Harrison, Fri Mar 26 15:29:47 PDT 2010
//    Guard against invalid 'avtRealDims' data.
//
// ***************************************************************************

void vtkDataSetRemoveGhostCells::RectilinearGridExecute()
{
  vtkRectilinearGrid *input  = (vtkRectilinearGrid*)this->GetInput();
  vtkRectilinearGrid *output  = (vtkRectilinearGrid*)this->GetOutput();

  vtkDataArray *realDims = input->GetFieldData()->GetArray("avtRealDims");


    bool realDimsOk = false;

    if(realDims &&
       realDims->GetDataType() == VTK_INT &&
       realDims->GetNumberOfComponents() == 1)
    {
        // make sure the realDims data isn't garbage (the facelist filter
        // will undermine us here if we don't double check)
        vtkIntArray *iarr = (vtkIntArray *)realDims;

        int ddims[3];
        int rdims[6];
        input->GetDimensions(ddims);
        memcpy(rdims,iarr->GetPointer(0),sizeof(int)*6);

        if( ((rdims[1] - rdims[0]) < ddims[0]) &&
            ((rdims[3] - rdims[2]) < ddims[1]) &&
            ((rdims[5] - rdims[4]) < ddims[2]) )
        realDimsOk = true;
    }

    if (!realDimsOk)
    {
        GenericExecute();
        return;
    }

  vtkDebugMacro(<< "Executing remove ghost cells filter for rectilinear grid");

  int i, voi[6];
  for (i = 0; i < 6; i++)
    {
    voi[i] = (int) realDims->GetComponent(i, 0);
    }

  vtkUnsignedCharArray *arr = (vtkUnsignedCharArray *)
                               input->GetCellData()->GetArray("avtGhostZones");
  if (GhostZoneTypesToRemove != 255 && arr != NULL)
  {
    unsigned char *ghosts = arr->GetPointer(0);
    int dims[3];
    input->GetDimensions(dims);

    // Check to make sure that the zones we are going to remove are
    // uniformly of the type we should remove.
    // It will modify the voi if necessary.
    ConfirmRegion(ghosts, dims, voi);
  }
 
  vtkVisItExtractRectilinearGrid *extractor = vtkVisItExtractRectilinearGrid::New();
  extractor->SetInput(input);
  extractor->SetVOI(voi);
  extractor->GetOutput()->Update();
 
  output->ShallowCopy(extractor->GetOutput());
  extractor->Delete();
  output->GetFieldData()->PassData(input->GetFieldData());
  output->GetFieldData()->RemoveArray("avtRealDims");
  if (GhostZoneTypesToRemove == 255)
  {
    output->GetCellData()->RemoveArray("avtGhostZones");
  }
}


// ***************************************************************************
//  Modifications:
//
//    Kathleen Bonnell, Fri Aug 31 08:50:30 PDT 2001
//    Removed check of field data.  No longer necessary with new vtk.
//
//    Hank Childs, Sun Apr 14 10:12:30 PDT 2002
//    Removed SetOutput call, due to a weird bug with VTK 4.0.  It wasn't
//    needed in the first place, anyway.
//
//    Hank Childs, Sat Jun 29 16:27:41 PDT 2002
//    Copy over field data and set the dimensions.
//
//    Kathleen Bonnell, Wed Jul 10 16:02:56 PDT 2002
//    Reworked code to properly use vtkExtractGrid filter.
//    Use new avtRealDims array.
//
//    Hank Childs, Fri Aug 27 15:10:50 PDT 2004
//    Use the new ghost data names.
//
//    Hank Childs, Thu Mar  2 11:14:53 PST 2006
//    Call generic execute if there are no real dims.
//
//    Hank Childs, Sun Oct 28 10:56:37 PST 2007
//    Added support for GhostZoneTypesToRemove.
//
// ***************************************************************************

void vtkDataSetRemoveGhostCells::StructuredGridExecute()
{
  vtkStructuredGrid *input  = (vtkStructuredGrid*)this->GetInput();
  vtkStructuredGrid *output = (vtkStructuredGrid*)this->GetOutput();
 
  vtkDataArray *realDims = input->GetFieldData()->GetArray("avtRealDims");
  if (!realDims || (realDims->GetDataType() != VTK_INT)
    || (realDims->GetNumberOfComponents() != 1))
    {
    GenericExecute();
    return;
    }

  vtkDebugMacro(<< "Executing remove ghost cells filter for structured grid");
 
   int i, voi[6];
   for (i = 0; i < 6; i++)
     {
     voi[i] = (int) realDims->GetComponent(i, 0);
     }
 
  vtkUnsignedCharArray *arr = (vtkUnsignedCharArray *)
                               input->GetCellData()->GetArray("avtGhostZones");
  if (GhostZoneTypesToRemove != 255 && arr != NULL)
  {
    unsigned char *ghosts = arr->GetPointer(0);
    int dims[3];
    input->GetDimensions(dims);

    // Check to make sure that the zones we are going to remove are
    // uniformly of the type we should remove.
    // It will modify the voi if necessary.
    ConfirmRegion(ghosts, dims, voi);
  }
 
  vtkVisItExtractGrid *extractor = vtkVisItExtractGrid::New();
  extractor->SetInput(input);
  extractor->SetVOI(voi);
  extractor->GetOutput()->Update();
 
  output->ShallowCopy(extractor->GetOutput());
  extractor->Delete();
  output->GetFieldData()->PassData(input->GetFieldData());
  output->GetFieldData()->RemoveArray("avtRealDims");
  if (GhostZoneTypesToRemove == 255)
  {
    output->GetCellData()->RemoveArray("avtGhostZones");
  }
}

void vtkDataSetRemoveGhostCells::ConfirmRegion(unsigned char *ghosts,
                                               int *pt_dims, int *voi)
{
    int  i, j, k;

    int dims[3];
    dims[0] = (pt_dims[0] > 1 ? pt_dims[0]-1 : 1);
    dims[1] = (pt_dims[1] > 1 ? pt_dims[1]-1 : 1);
    dims[2] = (pt_dims[2] > 1 ? pt_dims[2]-1 : 1);

    if (voi[0] > 0)
    {
        bool allGhost = true;
        i = voi[0]-1;
        for (j = 0 ; j < dims[1] && allGhost ; j++)
           for (k = 0 ; k < dims[2] && allGhost ; k++)
           {
               int c = k*dims[1]*dims[0] + j*dims[0] + i;
               unsigned char effectiveVal = ghosts[c] & GhostZoneTypesToRemove;
               if (! avtGhostData::IsGhostZone(effectiveVal))
                   allGhost = false;
           }
        if (!allGhost)
            voi[0] = 0;
    }
    if (voi[1] < dims[0])
    {
        bool allGhost = true;
        i = dims[0]-1;
        for (j = 0 ; j < dims[1] && allGhost ; j++)
           for (k = 0 ; k < dims[2] && allGhost ; k++)
           {
               int c = k*dims[1]*dims[0] + j*dims[0] + i;
               unsigned char effectiveVal = ghosts[c] & GhostZoneTypesToRemove;
               if (! avtGhostData::IsGhostZone(effectiveVal))
                   allGhost = false;
           }
        if (!allGhost)
            voi[1] = dims[0];
    }
    if (voi[2] > 0)
    {
        bool allGhost = true;
        j = voi[2]-1;
        for (i = 0 ; i < dims[0] && allGhost ; i++)
           for (k = 0 ; k < dims[2] && allGhost ; k++)
           {
               int c = k*dims[1]*dims[0] + j*dims[0] + i;
               unsigned char effectiveVal = ghosts[c] & GhostZoneTypesToRemove;
               if (! avtGhostData::IsGhostZone(effectiveVal))
                   allGhost = false;
           }
        if (!allGhost)
            voi[2] = 0;
    }
    if (voi[3] < dims[1])
    {
        bool allGhost = true;
        j = dims[1]-1;
        for (i = 0 ; i < dims[0] && allGhost ; i++)
           for (k = 0 ; k < dims[2] && allGhost ; k++)
           {
               int c = k*dims[1]*dims[0] + j*dims[0] + i;
               unsigned char effectiveVal = ghosts[c] & GhostZoneTypesToRemove;
               if (! avtGhostData::IsGhostZone(effectiveVal))
                   allGhost = false;
           }
        if (!allGhost)
            voi[3] = dims[1];
    }
    if (voi[4] > 0)
    {
        bool allGhost = true;
        k = voi[4]-1;
        for (i = 0 ; i < dims[0] && allGhost ; i++)
           for (j = 0 ; j < dims[1] && allGhost ; j++)
           {
               int c = k*dims[1]*dims[0] + j*dims[0] + i;
               unsigned char effectiveVal = ghosts[c] & GhostZoneTypesToRemove;
               if (! avtGhostData::IsGhostZone(effectiveVal))
                   allGhost = false;
           }
        if (!allGhost)
            voi[4] = 0;
    }
    if (voi[5] < dims[2])
    {
        bool allGhost = true;
        k = dims[2]-1;
        for (i = 0 ; i < dims[0] && allGhost ; i++)
           for (j = 0 ; j < dims[1] && allGhost ; j++)
           {
               int c = k*dims[1]*dims[0] + j*dims[0] + i;
               unsigned char effectiveVal = ghosts[c] & GhostZoneTypesToRemove;
               if (! avtGhostData::IsGhostZone(effectiveVal))
                   allGhost = false;
           }
        if (!allGhost)
            voi[5] = dims[2];
    }
}


// ****************************************************************************
// Modifications:
//   Kathleen Bonnell, Wed Mar  6 13:48:48 PST 2002
//   Call superclass's method the new vtk way.
//
//   Hank Childs, Mon Aug 30 16:27:16 PDT 2004
//   Remove references to ghost level, since it is no longer a data member.
//
// ****************************************************************************
void vtkDataSetRemoveGhostCells::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
