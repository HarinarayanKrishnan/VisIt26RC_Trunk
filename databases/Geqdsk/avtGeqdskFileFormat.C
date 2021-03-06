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
//                            avtGeqdskFileFormat.C                          //
//
// See:
// https://fusion.gat.com/conferences/snowmass/working/mfe/physics/p3/equilibria/g_eqdsk_s.pdf
// 
// For information on the file layout
//
// ************************************************************************* //

#include <avtGeqdskFileFormat.h>

#include <string>

#include <vtkFloatArray.h>
#include <vtkRectilinearGrid.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellType.h>

#include <vtkPointData.h>
#include <vtkVisItUtility.h>

#include <avtDatabaseMetaData.h>
#include <avtCurveMetaData.h>

#include <DBOptionsAttributes.h>
#include <Expression.h>

#include <InvalidVariableException.h>
#include <InvalidFilesException.h>

using     std::string;


// ****************************************************************************
//  Method: avtGeqdskFileFormat constructor
//
//  Programmer: allen -- generated by xml2avt
//  Creation:   Thu Aug 11 15:17:56 PST 2011
//
// ****************************************************************************

avtGeqdskFileFormat::avtGeqdskFileFormat(const char *filename, DBOptionsAttributes *readOpts)
  : avtMTSDFileFormat(&filename, 1), nw(0), nh(0)
{
    if (filename == "")
        return;

    std::ifstream f;
    f.open(filename);

    char tmp[1024], tmpInt[8];
    
    // Read the first line: case information, dummy var, and nw, and nh
    char id[32], date[32], run[32], time[32];
    float xdum;
    int idum;

    f.getline(tmp, 1024);

    if(!f.good())
    {
      EXCEPTION1( InvalidFilesException, "Read past end of file." );
    }

    // The first line contains 60 FORTRAN characters format (6a8,3i4),
    // 6 alpha characters of length 8 and 3 integers of length 4.

    // Strip off the last three int and convert them to ints.
    strncpy(tmpInt, &(tmp[48]), 4 );
    idum = atoi( tmpInt );
    strncpy(tmpInt, &(tmp[52]), 4 );
    nw = atoi( tmpInt );
    strncpy(tmpInt, &(tmp[56]), 4 );
    nh = atoi( tmpInt );

    if( nw < 0 || nh < 0 )
    {
        EXCEPTION1( InvalidVariableException,
                    "Can not read the first line - number of horizontal R and/or vertical Z grid points." );
    }

    int n = 3;

    tmp[48] = 0;
    std::string alphaStr(tmp);

    // One format :   EFIT    03/16/2004    #118898  3400ms
    if( alphaStr.find("EFIT") != std::string::npos &&
        sscanf( tmp, "%s %s %s %s", id, date, run, time ) == 4 )
    {
      cycles.resize( 1 );
      cycles[0] = atoi( time );

      std::string timeStr(time);

      times.resize( 1 );
      times[0] = atof( time );

      if( timeStr.find("ms") != std::string::npos )
        times[0] *= 1.0e-3;

      n = 7;
    }

    // Second format: TRXPL 09May2008 ALFA ALFA TRANSP(403 t=   0.9000
    else if( alphaStr.find("TRXPL") != std::string::npos &&
             sscanf( tmp, "%s %s ALFA ALFA TRANSP(%s t= %s",
                     id, date, run, time ) == 4 )
    {
      cycles.resize( 1 );
      cycles[0] = (int) (atof( time ) * 1000.0);

      times.resize( 1 );
      times[0] = atof( time );

      n = 7;
    }

    // Unkown - so no times or cycles.
    else
    {
      cycles.resize( 1 );
      cycles[0] = 0;

      times.resize( 1 );
      times[0] = 0;

      n = 7;
    }

    if( n != 7 )
    {
        EXCEPTION1( InvalidVariableException,
                    "Can not read the first line - number of horizontal R and/or vertical Z grid points." );
    }


    // Read the second line ...
    f.getline(tmp, 1024);
    if(!f.good())
    {
      EXCEPTION1( InvalidFilesException, "Read past end of file." );
    }

    n = sscanf( tmp, "%f %f %f %f %f",
                &rdim, &zdim, &rcentr, &rleft, &zmid );

    if( n != 5 )
    {
      EXCEPTION1( InvalidVariableException,
                  "Can not read second line - physical dimensions of the grid." );
    }

    // Read the third line ...
    f.getline(tmp, 1024);
    if(!f.good())
    {
      EXCEPTION1( InvalidFilesException, "Read past end of file." );
    }

    n = sscanf( tmp, "%f %f %f %f %f",
                &rmaxis, &zmaxis, &simag, &sibry, &bcentr );

    if( n != 5 )
    {
      EXCEPTION1( InvalidVariableException, "Can not read third line - location of magnetic axis and the poloidal flux." );
    }

    // Read the forth line ...
    f.getline(tmp, 1024);
    if(!f.good())
    {
      EXCEPTION1( InvalidFilesException, "Read past end of file" );
    }

    n = sscanf( tmp, "%f %f %f %f %f",
                &current, &simag, &xdum, &rmaxis, &xdum );

    if( n != 5 )
    {
      EXCEPTION1( InvalidVariableException, "Can not read forth line - current and magnetic axis." );
    }

    // Read the fifth line ...
    f.getline(tmp, 1024);
    if(!f.good())
    {
      EXCEPTION1( InvalidFilesException, "Read past end of file." );
    }

    n = sscanf( tmp, "%f %f %f %f %f",
                &zmaxis, &xdum, &sibry, &xdum, &xdum );

    if( n != 5 )
    {
      EXCEPTION1( InvalidVariableException, "Can not read fifth line - magnetic axis." );
    }

    // Read fpol - Poloidal current function in m-T, F = RBT on flux grid
    ReadData( f, &fpol, nw );

    // Read pres - Plasma pressure in nt/m2 on uniform flux grid
    ReadData( f, &pres, nw );

    // Read ffprim - FF’(ψ) in (mT)2 / (Weber/rad) on uniform flux grid
    ReadData( f, &ffprim, nw );

    // Read pprime - P’(ψ) in (nt /m2) / (Weber/rad) on uniform flux grid
    ReadData( f, &pprime, nw );

    // Read psizr - Poloidal flux in Weber/rad on the rectangular grid points
    ReadData( f, &psirz, nw*nh );

    // Read qpsi - q values on uniform flux grid from axis to boundary
    ReadData( f, &qpsi, nw );

    // Read the number of boundary and/or limiter points.
    f.getline(tmp, 1024);
    if(!f.good())
    {
      EXCEPTION1( InvalidFilesException, "Read past end of file." );
    }

    n = sscanf( tmp, "%d %d", &nbbbs, &limitr );

    if( n != 2 )
    {
      EXCEPTION1( InvalidVariableException,
                  "Can not read the number of boundary and/or limiter points." );
    }

    // Read RZ of boundary points in meter. Note the data is written
    // merged R and Z together so read together.
    ReadData( f, &rzbbbs, 2*nbbbs );

    // Read RZ of surrounding limiter contour in meter. Note the data
    // is written merged R and Z together so read together.
    ReadData( f, &rzlim, 2*limitr );
}

// ****************************************************************************
//  Method: avtEMSTDFileFormat::ReadData
//
//  Purpose:
//      Reads the data
//
//  Programmer: allen -- generated by xml2avt
//  Creation:   Thu Aug 11 15:17:56 PST 2011
//
// ****************************************************************************

void
avtGeqdskFileFormat::ReadData( std::ifstream &f, float **var, int nVar )
{
  // Read fpol - Poloidal current function in m-T, F = RBT on flux grid
  *var = new float[nVar];

  float *varPtr = *var;

  int cc = 0;

  while(f.good())
  {
    char tmp[1024];
    f.getline(tmp, 1024);
    if(!f.good())
    {
      EXCEPTION1( InvalidFilesException, "Read past end of file." );
    }

    float fdum[5];

    // According to the format there are at most 5 values dumped out:
    // https://fusion.gat.com/conferences/snowmass/working/mfe/physics/p3/equilibria/g_eqdsk_s.pdf
    // as such, scan appropriately based on what might be left to read
    // in the file.
    int n, count = (cc > nVar-5 ? nVar%5 : 5 );

    if( count == 5 )
      n = sscanf( tmp, "%f %f %f %f %f",
                  &fdum[0], &fdum[1], &fdum[2], &fdum[3], &fdum[4] );
    
    else if( count == 4 )
      n = sscanf( tmp, "%f %f %f %f",
                  &fdum[0], &fdum[1], &fdum[2], &fdum[3] );
    
    else if( count == 3 )
      n = sscanf( tmp, "%f %f %f",
                  &fdum[0], &fdum[1], &fdum[2] );
    
    else if( count == 2 )
      n = sscanf( tmp, "%f %f",
                  &fdum[0], &fdum[1] );
    
    else if( count == 1 )
      n = sscanf( tmp, "%f",
                  &fdum[0] );
    
    if( n != count )
    {
      EXCEPTION1( InvalidVariableException, "Can not read variable." );
    }

    for( int i=0; i<count; ++i )
      *varPtr++ = fdum[i];

    cc += count;

    if( cc == nVar )
    {
      return;
    }
  }

  EXCEPTION1( InvalidFilesException, "Read past end of file." );
}


// ****************************************************************************
//  Method: avtEMSTDFileFormat::GetNTimesteps
//
//  Purpose:
//      Tells the rest of the code how many timesteps there are in this file.
//
//  Programmer: allen -- generated by xml2avt
//  Creation:   Thu Aug 11 15:17:56 PST 2011
//
// ****************************************************************************

int
avtGeqdskFileFormat::GetNTimesteps(void)
{
    return 1;
}


// ****************************************************************************
//  Method: avtGeqdskFileFormat::FreeUpResources
//
//  Purpose:
//      When VisIt is done focusing on a particular timestep, it asks that
//      timestep to free up any resources (memory, file descriptors) that
//      it has associated with it.  This method is the mechanism for doing
//      that.
//
//  Programmer: allen -- generated by xml2avt
//  Creation:   Thu Aug 11 15:17:56 PST 2011
//
// ****************************************************************************

void
avtGeqdskFileFormat::FreeUpResources(void)
{
}


// ****************************************************************************
//  Method: avtGeqdskFileFormat::PopulateDatabaseMetaData
//
//  Purpose:
//      This database meta-data object is like a table of contents for the
//      file.  By populating it, you are telling the rest of VisIt what
//      information it can request from you.
//
//  Programmer: allen -- generated by xml2avt
//  Creation:   Thu Aug 11 15:17:56 PST 2011
//
// ****************************************************************************

void
avtGeqdskFileFormat::PopulateDatabaseMetaData(avtDatabaseMetaData *md,
                                              int timeState)
{
    avtMeshMetaData* mmd;
    int spatialDims = 3;
    int topologicalDims;
    double extents[6];
    int bounds[3];

    // Rectangular mesh
    topologicalDims = 3;

    mmd = new avtMeshMetaData("rectangular", 1, 1, 1, 0,
                              spatialDims, topologicalDims,
                              AVT_RECTILINEAR_MESH);

    extents[0] = rleft;                extents[1] = rleft + rdim;
    extents[2] = 0;                    extents[3] = 0;
    extents[4] = zmid - zdim / 2.0;    extents[5] = zmid + zdim / 2.0;

    bounds[0] = nw;    bounds[1] = 1;    bounds[2] = nh;

    mmd->SetExtents( extents );
    mmd->SetBounds( bounds );
    mmd->SetNumberCells( (nw-1)*(nh-1) );
    mmd->xLabel = "R";
    mmd->yLabel = "Phi";
    mmd->zLabel = "Z";
    mmd->xUnits = "meters";
    mmd->yUnits = "meters";
    mmd->zUnits = "meters";
    md->Add(mmd);


    // Boundary mesh
    topologicalDims = 1;

    mmd = new avtMeshMetaData("boundary", 1, 1, 1, 0,
                              spatialDims, topologicalDims,
                              AVT_UNSTRUCTURED_MESH);

    extents[0] = rleft + rdim;         extents[1] = rleft;
    extents[2] = 0;                    extents[3] = 0;
    extents[4] = zmid + zdim / 2.0;    extents[5] = zmid - zdim / 2.0;

    for( int i=0; i<nbbbs; ++i )
    {
      if( extents[0] > rzbbbs[i*2] )  extents[0] = rzbbbs[i*2];
      if( extents[1] < rzbbbs[i*2] )  extents[1] = rzbbbs[i*2];

      if( extents[4] > rzbbbs[i*2+1] )  extents[4] = rzbbbs[i*2+1];
      if( extents[5] < rzbbbs[i*2+1] )  extents[5] = rzbbbs[i*2+1];
    }

    bounds[0] = nbbbs-1;    bounds[1] = 1;    bounds[2] = 1;

    mmd->SetExtents( extents );
    mmd->SetBounds( bounds );
    mmd->SetNumberCells( nbbbs-1 );
    mmd->xLabel = "R";
    mmd->yLabel = "Phi";
    mmd->zLabel = "Z";
    mmd->xUnits = "meters";
    mmd->yUnits = "meters";
    mmd->zUnits = "meters";
    md->Add(mmd);


    // Limiter mesh 
    topologicalDims = 1;

    mmd = new avtMeshMetaData("limiter", 1, 1, 1, 0,
                              spatialDims, topologicalDims,
                              AVT_UNSTRUCTURED_MESH);

    extents[0] = rleft + rdim;         extents[1] = rleft;
    extents[2] = 0;                    extents[3] = 0;
    extents[4] = zmid + zdim / 2.0;    extents[5] = zmid - zdim / 2.0;

    for( int i=0; i<limitr; ++i )
    {
      if( extents[0] > rzlim[i*2] )  extents[0] = rzlim[i*2];
      if( extents[1] < rzlim[i*2] )  extents[1] = rzlim[i*2];

      if( extents[4] > rzlim[i*2+1] )  extents[4] = rzlim[i*2+1];
      if( extents[5] < rzlim[i*2+1] )  extents[5] = rzlim[i*2+1];
    }

    bounds[0] = limitr-1;    bounds[1] = 1;    bounds[2] = 1;

    mmd->SetExtents( extents );
    mmd->SetBounds( bounds );
    mmd->SetNumberCells( limitr-1 );
    mmd->xLabel = "R";
    mmd->yLabel = "Phi";
    mmd->zLabel = "Z";
    mmd->xUnits = "meters";
    mmd->yUnits = "meters";
    mmd->zUnits = "meters";
    md->Add(mmd);


    // Rect scalar value psirz
    extents[0] =  1e12;
    extents[1] = -1e12;
    
    for (int i=0; i<nw*nh; i++)
    {
      if( extents[0] > psirz[i] )
          extents[0] = psirz[i];
      if( extents[1] < psirz[i] )
        extents[1] = psirz[i];
    }

    avtScalarMetaData *scalar = new avtScalarMetaData();
    scalar->name = std::string("psirz");
    scalar->meshName = string("rectangular"); 
    scalar->centering =  AVT_NODECENT;
    scalar->hasDataExtents = true;
    scalar->SetExtents(extents);
    scalar->units = std::string("weber/rad");
    md->Add(scalar);


    // Curve fpol
    avtCurveMetaData *curve = new avtCurveMetaData;
    curve->name = "fpol";

    curve->minDataExtents =  1e12;
    curve->maxDataExtents = -1e12;
    curve->hasDataExtents = true;
    
    for (int i=0; i<nw; i++)
    {
      if( curve->minDataExtents > fpol[i] )
          curve->minDataExtents = fpol[i];
      if( curve->maxDataExtents < fpol[i] )
        curve->maxDataExtents = fpol[i];
    }

    curve->centering = AVT_NODECENT;
    curve->xLabel = string("Poloidal flux");
    curve->xUnits = std::string("weber/rad");
    curve->yLabel = string("Poloidal current");
    curve->yUnits = std::string("m-T");
    curve->hasSpatialExtents = true;
    curve->minSpatialExtents = simag;
    curve->maxSpatialExtents = sibry;
    md->Add(curve);


    // Curve pres
    curve = new avtCurveMetaData;
    curve->name = "pres";
    curve->centering = AVT_NODECENT;

    curve->minDataExtents =  1e12;
    curve->maxDataExtents = -1e12;
    curve->hasDataExtents = true;
    
    for (int i=0; i<nw; i++)
    {
      if( curve->minDataExtents > pres[i] )
          curve->minDataExtents = pres[i];
      if( curve->maxDataExtents < pres[i] )
        curve->maxDataExtents = pres[i];
    }

    curve->xLabel = string("Poloidal flux");
    curve->xUnits = std::string("weber/rad");
    curve->yLabel = string("Plasma pressure");
    curve->yUnits = std::string("nt/m^2");
    curve->hasSpatialExtents = true;
    curve->minSpatialExtents = simag;
    curve->maxSpatialExtents = sibry;
    md->Add(curve);


    // Curve ffprim
    curve = new avtCurveMetaData;
    curve->name = "ffprim";
    curve->centering = AVT_NODECENT;

    curve->minDataExtents =  1e12;
    curve->maxDataExtents = -1e12;
    curve->hasDataExtents = true;
    
    for (int i=0; i<nw; i++)
    {
      if( curve->minDataExtents > ffprim[i] )
          curve->minDataExtents = ffprim[i];
      if( curve->maxDataExtents < ffprim[i] )
        curve->maxDataExtents = ffprim[i];
    }

    curve->xLabel = string("Poloidal flux");
    curve->xUnits = std::string("weber/rad");
    curve->yLabel = string("FF'(ψ)");
    curve->yUnits = std::string("(mT)^2/(weber/rad)");
    curve->hasSpatialExtents = true;
    curve->minSpatialExtents = simag;
    curve->maxSpatialExtents = sibry;
    md->Add(curve);


    // Curve pprime
    curve = new avtCurveMetaData;
    curve->name = "pprime";
    curve->centering = AVT_NODECENT;

    curve->minDataExtents =  1e12;
    curve->maxDataExtents = -1e12;
    curve->hasDataExtents = true;
    
    for (int i=0; i<nw; i++)
    {
      if( curve->minDataExtents > pprime[i] )
          curve->minDataExtents = pprime[i];
      if( curve->maxDataExtents < pprime[i] )
        curve->maxDataExtents = pprime[i];
    }

    curve->xLabel = string("Poloidal flux");
    curve->xUnits = std::string("weber/rad");
    curve->yLabel = string("P'(ψ)");
    curve->yUnits = std::string("(nt/m^2)/(weber/rad)");
    curve->hasSpatialExtents = true;
    curve->minSpatialExtents = simag;
    curve->maxSpatialExtents = sibry;
    md->Add(curve);


    // Curve qpsi
    curve = new avtCurveMetaData;
    curve->name = "qpsi";
    curve->centering = AVT_NODECENT;

    curve->minDataExtents =  1e12;
    curve->maxDataExtents = -1e12;
    curve->hasDataExtents = true;
    
    for (int i=0; i<nw; i++)
    {
      if( curve->minDataExtents > qpsi[i] )
          curve->minDataExtents = qpsi[i];
      if( curve->maxDataExtents < qpsi[i] )
        curve->maxDataExtents = qpsi[i];
    }

    curve->xLabel = string("Poloidal flux");
    curve->xUnits = std::string("weber/rad");
    curve->yLabel = string("q values");
    curve->yUnits = std::string("");
    curve->hasSpatialExtents = true;
    curve->minSpatialExtents = simag;
    curve->maxSpatialExtents = sibry;
    md->Add(curve);

    md->SetCyclesAreAccurate(true);
    md->SetCycles( cycles );

    md->SetTimesAreAccurate(true);
    md->SetTimes( times );

}


// ****************************************************************************
//  Method: avtGeqdskFileFormat::GetMesh
//
//  Purpose:
//      Gets the mesh associated with this file.  The mesh is returned as a
//      derived type of vtkDataSet (ie vtkRectilinearGrid, vtkStructuredGrid,
//      vtkUnstructuredGrid, etc).
//
//  Arguments:
//      timestate   The index of the timestate.  If GetNTimesteps returned
//                  'N' time steps, this is guaranteed to be between 0 and N-1.
//      meshname    The name of the mesh of interest.  This can be ignored if
//                  there is only one mesh.
//
//  Programmer: allen -- generated by xml2avt
//  Creation:   Thu Aug 11 15:17:56 PST 2011
//
// ****************************************************************************

vtkDataSet *
avtGeqdskFileFormat::GetMesh(int timestate, const char *meshname)
{
  if( string(meshname) == string("rectangular") )
  {
    float dr = rdim / (float) (nw-1);
    float dz = zdim / (float) (nh-1);

    float rmin = rleft;
    float zmin = zmid - zdim / 2.0;

    vtkPoints* vpoints = vtkPoints::New();
    vpoints->SetDataTypeToFloat();
    vpoints->SetNumberOfPoints(nw*nh);

    int cc = 0;

    for( int i=0; i<nw; ++i )
    {
      float r = rmin + (float) i * dr;

      for( int j=0; j<nh; ++j )
      {
        float z = zmin + (float) j * dz;

        vpoints->SetPoint(cc++, r, 0, z );
      }
    }

    int dims[3] = {nw,1,nh};

    vtkStructuredGrid* sgrid = vtkStructuredGrid::New();
    sgrid->SetDimensions(&(dims[0]));
    sgrid->SetPoints(vpoints);

    vpoints->Delete();

    return sgrid;
  }

  else if( string(meshname) == string("boundary") ||
           string(meshname) == string("limiter") )
  {
    float *rzVals;
    int numNodes;

    if( string(meshname) == string("boundary") )
    {
      rzVals = rzbbbs;
      numNodes = nbbbs;
    }
    else if( string(meshname) == string("limiter") )
    {
      rzVals = rzlim;
      numNodes = limitr;
    }

    vtkUnstructuredGrid* ugridPtr = vtkUnstructuredGrid::New();

    vtkPoints* vpoints = vtkPoints::New();
    vpoints->SetDataTypeToFloat();
    vpoints->SetNumberOfPoints(nw*nh);

    ugridPtr->SetPoints(vpoints);

    for (size_t i=0; i<numNodes; ++i)
      vpoints->SetPoint(i, rzVals[i*2], 0, rzVals[i*2+1] );

    ugridPtr->Allocate(numNodes-1);

    unsigned int cellVerts = 2; // cell's connected node indices
    std::vector<vtkIdType> verts(cellVerts);

    for (size_t i=0; i<numNodes-1; ++i)
    {
      verts[0] = i;
      verts[1] = i + 1;

      ugridPtr->InsertNextCell(VTK_LINE, cellVerts, &verts[0]);
    }

    vpoints->Delete();

    return ugridPtr;
  }

  else if( string(meshname) == string("fpol") ||
           string(meshname) == string("pres") ||
           string(meshname) == string("ffprim") ||
           string(meshname) == string("pprime") ||
           string(meshname) == string("qpsi") )
  {
    float *var;

    if( string(meshname) == string("fpol") )
      var = fpol;
    else if( string(meshname) == string("pres") )
      var = pres;
    else if( string(meshname) == string("ffprim") )
      var = ffprim;
    else if( string(meshname) == string("pprime") )
      var = pprime;
    else if( string(meshname) == string("qpsi") )
      var = qpsi;

    // Create 1-D RectilinearGrid
    vtkFloatArray* vals = vtkFloatArray::New();
    vals->SetNumberOfComponents(1);
    vals->SetNumberOfTuples(nw);
    vals->SetName(meshname);

    vtkRectilinearGrid* rg = vtkVisItUtility::Create1DRGrid(nw, VTK_FLOAT);
    rg->GetPointData()->SetScalars(vals);

    vtkFloatArray* xc = vtkFloatArray::SafeDownCast(rg->GetXCoordinates());

    float xmin = simag;
    float xmax = sibry;
    float dx = (xmax - xmin) / (float) (nw-1);

    for (int i=0; i<nw; i++)
    {
      xc->SetValue(i, xmin + (float) i * dx);
      vals->SetValue(i, var[i]);
    }

    // Done, so clean up memory and return
    vals->Delete();

    return rg;

  }
  else
    return NULL;
}


// ****************************************************************************
//  Method: avtGeqdskFileFormat::GetVar
//
//  Purpose:
//      Gets a scalar variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      timestate  The index of the timestate.  If GetNTimesteps returned
//                 'N' time steps, this is guaranteed to be between 0 and N-1.
//      varname    The name of the variable requested.
//
//  Programmer: allen -- generated by xml2avt
//  Creation:   Thu Aug 11 15:17:56 PST 2011
//
// ****************************************************************************

vtkDataArray *
avtGeqdskFileFormat::GetVar(int timestate, const char *varname)
{
  if( std::string( varname ) == std::string( "psirz" ) )
  {
    vtkDataArray *rv = vtkFloatArray::New();

    rv->SetNumberOfTuples(nw*nh);

    int cc = 0;

    for( int i=0; i<nw; ++i )
    {
      for( int j=0; j<nh; ++j )
      {
        // NOTE: The data is transposed!!
        rv->SetTuple(j*nw+i, &psirz[i*nh+j]);
      }
    }

    return rv;
  }
  else
    return NULL;
}


// ****************************************************************************
//  Method: avtGeqdskFileFormat::GetVectorVar
//
//  Purpose:
//      Gets a vector variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      timestate  The index of the timestate.  If GetNTimesteps returned
//                 'N' time steps, this is guaranteed to be between 0 and N-1.
//      varname    The name of the variable requested.
//
//  Programmer: allen -- generated by xml2avt
//  Creation:   Thu Aug 11 15:17:56 PST 2011
//
// ****************************************************************************

vtkDataArray *
avtGeqdskFileFormat::GetVectorVar(int timestate, const char *varname)
{
  return NULL;
}

// ****************************************************************************
//  Method: avtGeqdskFileFormat::GetCycles
//
//  Purpose:
//      Returns the cycles
//
//  Arguments:
//      c          the cycles
//
//  Programmer: allen
//  Creation:   
//
// ****************************************************************************


void avtGeqdskFileFormat::GetCycles(std::vector<int> &c)
{
    c = cycles;
}


// ****************************************************************************
//  Method: avtGeqdskFileFormat::GetTimes
//
//  Purpose:
//      Returns the times
//
//  Arguments:
//      t          the times
//
//  Programmer: allen
//  Creation:   
//
// ****************************************************************************

void avtGeqdskFileFormat::GetTimes(std::vector<double> &t)
{
    t = times;
}
