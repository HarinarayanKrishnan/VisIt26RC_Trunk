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
//                            avtViSUSFileFormat.C                           //
// ************************************************************************* //

#include <math.h>

#include <idx_input.h>

#include <avtViSUSFileFormat.h>

#include <map>
#include <string>
#include <vector>

#include <vtkFloatArray.h>
#include <vtkRectilinearGrid.h>
#include <vtkUnstructuredGrid.h>

#include <avtDatabase.h>
#include <avtDatabaseMetaData.h>
#include <avtLogicalSelection.h>
#include <avtParallel.h>
#include <avtSpatialBoxSelection.h>

#include <DebugStream.h>
#include <InvalidVariableException.h>

#include <snprintf.h>

#ifdef PARALLEL
#include <mpi.h>
#endif

using     std::map;
using     std::string;
using     std::vector;

// ****************************************************************************
//  Method: avtViSUSFileFormat::SetupRectilinearCoordinates
//
//  Purpose: Sets up the coordinate arrays for the rectilinear mesh
//
//  Programmer: Mark C. Miller 
//  Creation:   Sunday, September 26, 2004 
//
// ****************************************************************************
void
avtViSUSFileFormat::SetupRectilinearCoordinates()
{
    if (haveSetupCoordinates)
        return;

    //
    // The ViSUS file format does not yet support rectilinear meshes.
    // So, we just populate these rectilinear coordinate arrays with
    // uniform coordinates. When ViSUS is enhanced to support rectilinear
    // meshes, this code should be replaced with code that populates these
    // arrays from data in the ViSUS file.
    //
    for (int i = 0; i < dataDimension; i++)
    {
        globalNodeCoords[i] = new double[globalZoneCount[i]+1];
        double stepSize = (globalMax[i] - globalMin[i]) / globalZoneCount[i];
        for (int j = 0; j < globalZoneCount[i]+1; j++)
            globalNodeCoords[i][j] = globalMin[i] + j * stepSize;;
    }

    haveSetupCoordinates = true;
}

// ****************************************************************************
//  Method: avtViSUS constructor
//
//  Programmer: mcmiller -- generated by xml2avt
//  Creation:   Tue Sep 14 19:55:29 PST 2004
//
//  Modifications:
//
//    Mark C. Miller, Tue Aug 16 13:56:55 PDT 2005
//    Added initialization of min/maxTimeIndex and haveOpenedFile
//
// ****************************************************************************

avtViSUSFileFormat::avtViSUSFileFormat(const char *filename)
    : avtMTSDFileFormat(&filename, 1)
{
    // idxFile = ???; whats a value representing "not set"
    fieldSampleSize = 0;
    fileName = filename;
    useGetData3D = false;
    for (int i = 0; i < 3; i++)
        globalNodeCoords[i] = 0;
    haveSetupCoordinates = false;
    ignoreDataSelections = false;
    haveOpenedFile = false;
    minTimeIndex = -1;
    maxTimeIndex = -1;
}

avtViSUSFileFormat::~avtViSUSFileFormat()
{
    if (fieldSampleSize)
        delete [] fieldSampleSize;
    for (int i = 0; i < 3; i++)
    {
        if (globalNodeCoords[i] != 0)
            delete [] globalNodeCoords[i];
    }
}

// ****************************************************************************
//  Method: avtViSUSFileFormat::FreeUpResources
//
//  Purpose:
//      When VisIt is done focusing on a particular timestep, it asks that
//      timestep to free up any resources (memory, file descriptors) that
//      it has associated with it.  This method is the mechanism for doing
//      that.
//
//  Programmer: mcmiller -- generated by xml2avt
//  Creation:   Tue Sep 14 19:55:29 PST 2004
//
//  Modifications:
//    Mark C. Miller, Tue Aug 16 13:56:55 PDT 2005
//    Added call to close IDX file
// ****************************************************************************

void
avtViSUSFileFormat::FreeUpResources(void)
{
    if (haveOpenedFile)
    {
        IDX_close(idxFile);
        haveOpenedFile = false;
    }
}

// ****************************************************************************
//  Method: avtViSUSFileFormat::CanCacheVariable
//
//  Purpose:
//      To truly exercise the ViSUS file format, we can't have VisIt caching
//      chunks of mesh and variables above the plugin.
//      
//  Programmer: Mark C. Miller 
//  Creation:   September 20, 2004 
//
// ****************************************************************************

bool
avtViSUSFileFormat::CanCacheVariable(const char *var)
{
    return false;
}


// ****************************************************************************
//  Method: avtViSUSFileFormat::RegisterDataSelections
//
//  Purpose:
//      The ViSUS format can exploit some data selections. So, we implement,
//      this method here. 
//      
//  Programmer: Mark C. Miller 
//  Creation:   September 20, 2004 
//
// ****************************************************************************
void
avtViSUSFileFormat::RegisterDataSelections(
    const vector<avtDataSelection_p> &sels,
    vector<bool> *selectionsApplied)
{
    selList     = sels;
    selsApplied = selectionsApplied;
}

// ****************************************************************************
//  Method: avtViSUSFileFormat::GetFile
//
//  Programmer: Mark C. Miller 
//  Creation:   September 27, 2004 
//
// ****************************************************************************
void
avtViSUSFileFormat::GetFile()
{
    if (haveOpenedFile == false)
    {
        char tmpName[1024];
        strncpy(tmpName, fileName.c_str(), sizeof(tmpName));
        idxFile = IDX_open(tmpName);
        haveOpenedFile = true;
    }
}

// ****************************************************************************
//  Method: avtViSUSFileFormat::GetTimeInfo
//
//  Programmer: Mark C. Miller 
//  Creation:   September 27, 2004 
//
// ****************************************************************************
void
avtViSUSFileFormat::GetTimeInfo()
{
    GetFile();

    // get start and stop time indices
    if (minTimeIndex == -1 || maxTimeIndex == -1)
        IDX_get_time_rank_min_max(idxFile, &minTimeIndex, &maxTimeIndex);

    if (cycleVals.size() == 0 || timeVals.size() == 0)
    {
        for (int t = minTimeIndex; t <= maxTimeIndex; t++)
        {
            double timeVal;
            IDX_get_time(idxFile, t, &timeVal);
            timeVals.push_back(timeVal);
            cycleVals.push_back(t);
        }
    }
}

// ****************************************************************************
//  Method: avtViSUSFileFormat::GetCycles
//
//  Programmer: Mark C. Miller 
//  Creation:   September 27, 2004 
//
//  Modifications:
//    Mark C. Miller, Tue Aug 16 13:56:55 PDT 2005
//    Added call to GetTimeInfo
// ****************************************************************************
void
avtViSUSFileFormat::GetCycles(vector<int> &cycles)
{
    GetTimeInfo();
    cycles = cycleVals;
}

// ****************************************************************************
//  Method: avtViSUSFileFormat::GetTimes
//
//  Programmer: Mark C. Miller 
//  Creation:   September 27, 2004 
//
//  Modifications:
//    Mark C. Miller, Tue Aug 16 13:56:55 PDT 2005
//    Added call to GetTimeInfo
// ****************************************************************************
void
avtViSUSFileFormat::GetTimes(vector<double> &times)
{
    GetTimeInfo();
    times = timeVals;
}

// ****************************************************************************
//  Method: avtViSUSFileFormat::GetTimes
//
//  Programmer: Mark C. Miller 
//  Creation:   September 27, 2004 
//
//  Modifications:
//    Mark C. Miller, Tue Aug 16 13:56:55 PDT 2005
//    Added call to GetTimeInfo
// ****************************************************************************
int
avtViSUSFileFormat::GetNTimesteps(void)
{
    GetTimeInfo();
    return maxTimeIndex - minTimeIndex + 1;
}

// ****************************************************************************
//  Method: avtViSUSFileFormat::PopulateDatabaseMetaData
//
//  Purpose:
//      This database meta-data object is like a table of contents for the
//      file.  By populating it, you are telling the rest of VisIt what
//      information it can request from you.
//
//  Programmer: mcmiller -- generated by xml2avt
//  Creation:   Tue Sep 14 19:55:29 PST 2004
//
//  Modifications:
//
//    Mark C. Miller, Tue May 17 18:48:38 PDT 2005
//    Added timeState arg to satisfy new interface
// 
//    Mark C. Miller, Tue Aug 16 13:56:55 PDT 2005
//    Added calls to GetFile/GetTimeInfo
//    Added call to IDX_get_time_rank_min_max 
//
//    Mark C. Miller, Mon Jan 22 22:09:01 PST 2007
//    Changed MPI_COMM_WORLD to VISIT_MPI_COMM
//
//    Mark C. Miller, Tue Apr 29 23:33:55 PDT 2008
//    Eliminated getenv call.
// ****************************************************************************

void
avtViSUSFileFormat::PopulateDatabaseMetaData(avtDatabaseMetaData *md,
    int timeState)
{

    procNum = 0;
    procCount = 1;
#ifdef PARALLEL
    MPI_Comm_rank(VISIT_MPI_COMM, &procNum);
    MPI_Comm_size(VISIT_MPI_COMM, &procCount);
#endif

    ignoreDataSelections = false; 

    GetFile();

    GetTimeInfo();

    dataDimension = IDX_get_data_dimension(idxFile);

    // get node-centered min/max indices along each axis
    IDX_get_bounding_box(idxFile, &globalMin[0], &globalMax[0],
                                  &globalMin[1], &globalMax[1],
                                  &globalMin[2], &globalMax[2]);

    IDX_get_origin_3D(idxFile, &origin3D[0], &origin3D[1], &origin3D[2]);
    IDX_get_grid_spacing_3D(idxFile,
        &gridSpacing3D[0], &gridSpacing3D[1], &gridSpacing3D[2]);

    double exts[6];
    exts[0] = (double) globalMin[0];
    exts[1] = (double) globalMax[0];
    exts[2] = (double) globalMin[1];
    exts[3] = (double) globalMax[1];
    exts[4] = (double) globalMin[2];
    exts[5] = (double) globalMax[2];

    AddMeshToMetaData(md, "mesh", AVT_RECTILINEAR_MESH, exts, 1, 0,
        dataDimension, dataDimension);
    md->SetFormatCanDoDomainDecomposition(true);

    numFields = IDX_get_num_fields(idxFile);

    fieldSampleSize = new int[numFields];
    for (int i = 0; i < numFields; i++)
    {
        fieldSampleSize[i] = IDX_get_field_sample_size(idxFile, i);

        char tmpStr[32];
        char *fieldName = 0;
        IDX_get_field_name(idxFile, i, &fieldName);
        if (fieldName == 0 || fieldName[0] == 0)
        {
            SNPRINTF(tmpStr, sizeof(tmpStr), "var_%02d", i);
        }
        else
        {
            strcpy(tmpStr, fieldName);
        }
        fieldMap[tmpStr] = i;
        AddScalarVarToMetaData(md, tmpStr, "mesh", AVT_ZONECENT);
    }

    for (int i = 0; i < dataDimension; i++)
        globalZoneCount[i] = globalMax[i] - globalMin[i];

    globalZoneTotal = 1;
    for (int i = 0; i < dataDimension; i++)
        globalZoneTotal *= globalZoneCount[i];

}

// ****************************************************************************
//  Method: avtViSUSFileFormat::SetupDomainAndZoneIndexing
//
//  Purpose:
//      Processes all the data selections and determines which zones to read.
//      
//  Programmer: Mark C. Miller 
//  Creation:   September 20, 2004 
//
//  Modifications:
//    Removed restriction on power-of-2 index select
//
//    Mark C. Miller, Tue Aug 15 15:28:11 PDT 2006
//    Moved DomainDecomp functions to avtDatabase
//
//    Eric Brugger, Fri Jul 22 14:23:17 PDT 2011
//    I corrected some compilation errors.
//
// ****************************************************************************
void
avtViSUSFileFormat::SetupDomainAndZoneIndexing(int *outputZoneCounts,
    int *inputStepSize, int *inputStartZone,
    int *domainCounts, int *domainIndices)
{
    SetupRectilinearCoordinates();

    int validStrides[32];
    for (int i = 0; i < 32; i++)
        validStrides[i] = 1 << i;

    int firstInputZone[3];
    int lastInputZone[3];
    int stepInputZones[3];
    for (int i = 0; i < 3; i++)
    {
        firstInputZone[i] = 0;
        lastInputZone[i]  = globalZoneCount[i]-1;  
        stepInputZones[i] = 1;
    }

    //
    // Loop over all selections, reducing each to a logical selection
    // and then composing together arriving at a single, logical
    // selection for whatever we can service here
    //
    // Note the internal logical selection is ZONAL based where as the
    // logical selection from the IndexSelect operator is NODAL based.
    //
    avtLogicalSelection composedSel;
    for (int i = 0; i < selList.size(); i++)
    {
        if (ignoreDataSelections)
        {
            (*selsApplied)[i] = false;
        }
        else if (string(selList[i]->GetType()) == "Logical Data Selection")
        {
            avtLogicalSelection *sel = (avtLogicalSelection *) *(selList[i]);

            int mins[3], maxs[3], strides[3];

            sel->GetStarts(mins);
            sel->GetStops(maxs);
            sel->GetStrides(strides);

            // The avtLogicalSelection from IndexSelect is nodal based
            // internally the composed selection is zonal so substract
            // 1 when appropriate.
            for (int j = 0; j < 3; j++)
            {
              // Do not subtract if the max is zero as it will result
              // in -1 which means maximum.

              // Note with IndexSelect is possible to have the min and
              // max be equal (i.e. a slice in a volume). ViSUS is
              // zonal and does not allow a slice so check for that
              // case.
              if( maxs[i] > 0 && mins[i] != maxs[i] )
                maxs[i] -= 1;
            }

            avtLogicalSelection newSel;
            newSel.SetStarts(mins);
            newSel.SetStops(maxs);
            newSel.SetStrides(strides);

            // overrwrite method-scope arrays with the new indexing
            composedSel.Compose(newSel);
            (*selsApplied)[i] = true;
        }
        else if (string(selList[i]->GetType()) == "Spatial Box Data Selection")
        {
            avtSpatialBoxSelection *sel = (avtSpatialBoxSelection *) *(selList[i]);

            double mins[3], maxs[3];
            sel->GetMins(mins);
            sel->GetMaxs(maxs);

            int nodeCounts[3];
            for (int j = 0; j < 3; j++)
                nodeCounts[j] = globalZoneCount[j]+1;
            sel->GetLogicalBounds(nodeCounts, globalNodeCoords,
                     firstInputZone, lastInputZone);
            for (int j = 0; j < 3; j++)
            {
                if (lastInputZone[j] <= firstInputZone[j])
                    lastInputZone[j] = firstInputZone[j];
                stepInputZones[j] = 1;
            }

            avtLogicalSelection newSel;
            newSel.SetStarts(firstInputZone);
            newSel.SetStops(lastInputZone);

            composedSel.Compose(newSel);
            (*selsApplied)[i] = true;
        }
        else
        {
            // indicate we won't handle this selection
            (*selsApplied)[i] = false;
        }
    }

    //
    // Compute combined starts, stops strides
    //
    {
        int starts[3], stops[3], strides[3];
        composedSel.GetStarts(starts);
        composedSel.GetStrides(strides);
        composedSel.GetStops(stops);

        // scheme and do some sanity checks
        for (int j = 0; j < 3; j++)
        {
            debug5 << "for dim " << j << " starts=" << starts[j] << ", stops=" << stops[j] << ", strides=" << strides[j] << endl;
            firstInputZone[j] = starts[j];
            if (stops[j] != -1)
              lastInputZone[j] = stops[j];
            stepInputZones[j] = strides[j];

            if (stepInputZones[j] >= globalZoneCount[j])
                stepInputZones[j] = globalZoneCount[j]-1;

            if (firstInputZone[j] >= globalZoneCount[j])
                firstInputZone[j] = globalZoneCount[j]-1;

            if (lastInputZone[j] < firstInputZone[j])
                lastInputZone[j] = firstInputZone[j];
        }
    }

    //
    // compute number of zones along each axis and the total in the output
    //
    int numOutputZones[3];
    int totalOutputZones = 1;
    for (int i = 0; i < 3; i++)
    {
        numOutputZones[i] = (lastInputZone[i] - firstInputZone[i] + 1) / stepInputZones[i];
        if ((lastInputZone[i] - firstInputZone[i] + 1) % stepInputZones[i] > 0)
            numOutputZones[i]++;
        totalOutputZones *= numOutputZones[i];
    }

    int procsToUse = procCount;
#if 0
    //
    // Avoid load-balancing to the point of becomming communication
    // bound. If the count of zones per proc falls below an arbitrary
    // threshold, have fewer processors participate and let the others
    // return an empty mesh. We achieve this by setting procsToUse
    // below procCount
    //
    if (totalOutputZones / procCount < minZonesPerProc)
        procsToUse = totalOutputZones / minZonesPerProc + 1;
#endif

    //
    // Determine a logical domain decomposition of the set of output zones 
    //
    int domCount[3]; 
    avtDatabase::ComputeRectilinearDecomposition(dataDimension, procCount,
        numOutputZones[0], numOutputZones[1], numOutputZones[2],
        &domCount[0], &domCount[1], &domCount[2]);

    if (procNum == 0)
    {
        debug1 << "Decomposition: " << domCount[0] << ", " << domCount[1] << ", " << domCount[2] << endl;
    }

    //
    // Determine this processor's logical domain indices
    //
    int domLogicalCoords[3];
    avtDatabase::ComputeDomainLogicalCoords(dataDimension, domCount, procNum,
        domLogicalCoords);

    debug1 << "Processor " << procNum << " domain logical coords: "
         << domLogicalCoords[0] << ", " << domLogicalCoords[1] << ", " << domLogicalCoords[2] << endl;

    for (int i = 0; i < 3; i++)
    {
        outputZoneCounts[i] = numOutputZones[i];
        domainCounts[i]     = domCount[i];
        domainIndices[i]    = domLogicalCoords[i];
        inputStartZone[i]   = firstInputZone[i];
        inputStepSize[i]    = stepInputZones[i];
    }

}

// ****************************************************************************
//  Method: avtViSUSFileFormat::GetMesh
//
//  Purpose:
//      Gets the mesh associated with this file.  The mesh is returned as a
//      derived type of vtkDataSet (ie vtkRectilinearGrid, vtkStructuredGrid,
//      vtkUnstructuredGrid, etc).
//
//  Arguments:
//      meshname    The name of the mesh of interest.  This can be ignored if
//                  there is only one mesh.
//
//  Programmer: mcmiller -- generated by xml2avt
//  Creation:   Tue Sep 14 19:55:29 PST 2004
//
//  Moficiations:
//
//    Mark C. Miller, Tue Aug 15 15:28:11 PDT 2006
//    Moved Domain decomp. functions to avtDatabase
// ****************************************************************************

vtkDataSet *
avtViSUSFileFormat::GetMesh(int, const char *meshname)
{

    vtkDataSet *retval = 0;


    int numOutputZones[3], startInputZones[3], stepInputZones[3];
    int domCount[3], domLogicalCoords[3];
    SetupDomainAndZoneIndexing(numOutputZones,
                               stepInputZones, startInputZones,
                               domCount, domLogicalCoords);

    //
    // Now, create this processor's chunk of mesh
    //
    vtkFloatArray  *coords[3];
    int nodeCount[3], zoneStart[3], zoneCount[3];
    for (int i = 0 ; i < 3 ; i++)
    {
        //
        // compute the bounds, in terms of output zone numbers,
        // of this processor's domain.
        //
        avtDatabase::ComputeDomainBounds(numOutputZones[i], domCount[i], domLogicalCoords[i],
            &zoneStart[i], &zoneCount[i]);

        nodeCount[i] = zoneCount[i] + 1;

        // Default number of components for an array is 1.
        coords[i] = vtkFloatArray::New();
        coords[i]->SetNumberOfTuples(nodeCount[i]);
        for (int j = 0; j < nodeCount[i]; j++)
        {
            int outputNodeIndex = zoneStart[i] + j;
            int inputNodeIndex = startInputZones[i] + outputNodeIndex * stepInputZones[i];
            if (inputNodeIndex >= globalZoneCount[i])
                inputNodeIndex = globalZoneCount[i];
            coords[i]->SetComponent(j, 0, globalNodeCoords[i][inputNodeIndex]);
        }
    }

    debug1 << "Processor " << procNum << " bounds: "
         << zoneStart[0] << ", " << zoneStart[1] << ", " << zoneStart[2] << "   "
         << zoneCount[0] << ", " << zoneCount[1] << ", " << zoneCount[2] << endl; 

    vtkRectilinearGrid  *rGrid = vtkRectilinearGrid::New();
    rGrid->SetDimensions(nodeCount);
    rGrid->SetXCoordinates(coords[0]);
    coords[0]->Delete();
    rGrid->SetYCoordinates(coords[1]);
    coords[1]->Delete();
    rGrid->SetZCoordinates(coords[2]);
    coords[2]->Delete();

    retval = rGrid;

    return retval;

}

// ****************************************************************************
//  Method: avtViSUSFileFormat::GetVar
//
//  Purpose:
//      Gets a scalar variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      varname    The name of the variable requested.
//
//  Programmer: mcmiller -- generated by xml2avt
//  Creation:   Tue Sep 14 19:55:29 PST 2004
//
//  Modifications:
//
//    Mark C. Miller, Mon Feb  7 19:50:42 PST 2005
//    Removed extraneous cerr statement
//
//    Mark C. Miller, Tue Aug 16 13:56:55 PDT 2005
//    Added logic to support different data types
//
// ****************************************************************************

vtkDataArray *
avtViSUSFileFormat::GetVar(int ts, const char *varname)
{

    int numOutputZones[3], stepInputZones[3], startInputZones[3];
    int domCount[3], domLogicalCoords[3];
    SetupDomainAndZoneIndexing(numOutputZones,
                               stepInputZones, startInputZones,
                               domCount, domLogicalCoords);

    if (varname == 0)
    {
        EXCEPTION1(InvalidVariableException, "<NULL>"); 
    }

    if (fieldMap.find(varname) == fieldMap.end())
    {
        EXCEPTION1(InvalidVariableException, varname);
    }

    int fieldIdx = fieldMap[varname];

    int validStrides[32];
    for (int i = 0; i < 32; i++)
        validStrides[i] = 1 << i;

    int pow2[3] = {0,0,0};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            if (stepInputZones[i] == validStrides[j])
                pow2[i] = j;
        }
    }

    int nzones = 1;
    int actualMin[3], actualMax[3], delta[3], num[3];
    int zoneStart[3], zoneCount[3];
    for (int i = 0; i < 3; i++)
    {
        //
        // compute the bounds, in terms of output zone numbers,
        // of this processor's domain.
        //
        avtDatabase::ComputeDomainBounds(numOutputZones[i], domCount[i], domLogicalCoords[i],
            &zoneStart[i], &zoneCount[i]);

        actualMin[i] = startInputZones[i]+zoneStart[i]*stepInputZones[i];
        actualMax[i] = startInputZones[i]+zoneStart[i]*stepInputZones[i]+zoneCount[i]*stepInputZones[i]-1;
        delta[i] = stepInputZones[i];
        debug5 << "for dim " << i << ", min=" << actualMin[i] << ", max=" << actualMax[i] << endl;
        nzones *= zoneCount[i];
    }

    //
    // Guess data type from IDX's field_sample_size 
    //
    int dataType;
    switch (fieldSampleSize[fieldIdx])
    {
        case 1: // assume unsigned char
            dataType = VTK_UNSIGNED_CHAR;
            break;
        case 2: // assume unsigned short
            dataType = VTK_UNSIGNED_SHORT;
            break;
        case 4: // assume float
            dataType = VTK_FLOAT;
            break;
        case 8: // assume double
            dataType = VTK_DOUBLE;
            break;
        default:
        {
            char tmpMsg[1024];
            SNPRINTF(tmpMsg, sizeof(tmpMsg), "Unrecognized field sample size of %d for "
                "variable \"%s\"", fieldSampleSize[fieldIdx], varname);
            EXCEPTION1(InvalidVariableException, tmpMsg);
        }
    }

    vtkDataArray *retval = vtkDataArray::CreateDataArray(dataType);
    retval->SetNumberOfComponents(1);
    retval->SetNumberOfTuples(nzones);
    void *dbuf = retval->GetVoidPointer(0);

    IDX_get_data_3D(dbuf, idxFile, ts, fieldIdx,
        actualMin[0], actualMax[0], actualMin[1], actualMax[1], actualMin[2], actualMax[2],
        delta[0], delta[1], delta[2], nzones);

    return retval;

}
