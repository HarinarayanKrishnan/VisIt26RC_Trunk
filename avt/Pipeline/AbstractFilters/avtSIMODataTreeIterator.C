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
//                           avtSIMODataTreeIterator.C                           //
// ************************************************************************* //

#include <avtSIMODataTreeIterator.h>

#include <vtkDataSet.h>

#include <avtCommonDataFunctions.h>
#include <avtDataTree.h>
#include <avtExtents.h>

#include <IncompatibleDomainListsException.h>
#include <DebugStream.h>

#include <string>


// ****************************************************************************
//  Method: avtSIMODataTreeIterator constructor
//
//  Programmer: Hank Childs
//  Creation:   June 20, 2001
//
// ****************************************************************************

avtSIMODataTreeIterator::avtSIMODataTreeIterator()
{
    currentNode = 0;
    totalNodes  = 0;
    overrideOriginalSpatialExtents = false;
    overrideOriginalDataExtents    = false;
    originalSpatialExtents = NULL;
    originalDataExtents    = NULL;
}


// ****************************************************************************
//  Method: avtSIMODataTreeIterator destructor
//
//  Programmer: Hank Childs
//  Creation:   November 6, 2001
//
// ****************************************************************************

avtSIMODataTreeIterator::~avtSIMODataTreeIterator()
{
    if (originalSpatialExtents != NULL)
    {
        delete originalSpatialExtents;
        originalSpatialExtents = NULL;
    }
    if (originalDataExtents != NULL)
    {
        delete originalDataExtents;
        originalDataExtents = NULL;
    }
}


// ****************************************************************************
//  Method: avtSIMODataTreeIterator::Execute
//
//  Purpose:
//      Defines the pure virtual function execute.  This does the work of
//      taking the input dataset and disabling the upstream pipeline to it.
//      It then sets the output and calls the pure virtual function
//      ExecuteDataTree.
//
//  Programmer: Hank Childs
//  Creation:   July 24, 2000
//
//  Modifications:
//
//    Hank Childs, Mon Jun  4 13:46:00 PDT 2001
//    Stop using domain's argument since everything should be driven by the
//    input data object anyway.  Re-wrote function and blew away previous
//    comments.
//
//    Hank Childs, Wed Jun 20 18:11:15 PDT 2001
//    Add support for updating progress.
//
//    Hank Childs, Wed Oct 24 14:21:18 PDT 2001
//    Moved PreExecute and PostExecute calls to avtFilter.
//
//    Eric Brugger, Mon Nov  5 13:38:23 PST 2001
//    Modified to always compile the timing code.
//
//    Hank Childs, Mon Feb 28 10:39:28 PST 2005
//    Moved timings code into base class avtFilter.
//
//    Hank Childs, Wed May  9 16:57:25 PDT 2007
//    Make sure that output tree is not NULL, since that causes heartache.
//
//    Hank Childs, Tue Apr 10 17:26:52 PDT 2012
//    Check for NULL input trees.
//
// ****************************************************************************

void
avtSIMODataTreeIterator::Execute(void)
{
    //
    // This will walk through the data domains in a data tree.
    //
    avtDataTree_p tree    = GetInputDataTree();
    avtDataTree_p newTree;
    if (*tree != NULL)
    {
        totalNodes = tree->GetNumberOfLeaves();
        newTree = Execute(tree);
    }
    else
    {
        // This can happen when a filter serves up an empty data tree.
        // It can also happen when we claim memory from intermediate
        // data objects and then go back to execute them.
        debug1 << "Unusual situation: NULL input tree to SIMO iterator.  Likely "
               << "that an exception occurred previously." << endl;
    }

    if (*newTree == NULL)
    {
        //
        // Lots of code assumes that the root tree is non-NULL.  Put a dummy
        // tree in its place.
        //
        newTree = new avtDataTree();
    }

    SetOutputDataTree(newTree);
}


// ****************************************************************************
//  Method: avtSIMODataTreeIterator::Execute
//
//  Purpose:
//    A recursive Execute method.  Walks down the tree and calls the virtual 
//    function ExecuteDataTree on the leaves.
//
//  Arguments:
//      inDT    The tree to traverse. 
//      dom     The domain number that the tree represents. 
//
//  Programmer:  Kathleen Bonnell 
//  Creation:    April 12, 2001 
//
//  Modifications:
//
//    Hank Childs, Tue Jun 19 12:36:02 PDT 2001
//    Handle NULL data trees better.
//
//    Kathleen Bonnell, Wed Sep 19 13:45:33 PDT 2001 
//    Retrieve and pass on the label associated with a dataset. 
//
//    Hank Childs, Tue Nov  6 11:49:18 PST 2001
//    Added support for overriding extents.
//
//    Hank Childs, Sat Jan  5 10:31:48 PST 2002
//    Fix memory leak.
//
//    Kathleen Bonnell, Tue May 16 09:41:46 PDT 2006 
//    Removed call to SetSource(NULL), with new vtk pipeline, it also removes
//    necessary information from the dataset. 
//
// ****************************************************************************

avtDataTree_p
avtSIMODataTreeIterator::Execute(avtDataTree_p inDT)
{
    CheckAbort();

    if (*inDT == NULL)
    {
        return NULL;
    }

    int nc = inDT->GetNChildren();

    if (nc <= 0 && !inDT->HasData())
    {
        return NULL;
    }

    if ( nc == 0 )
    {
        //
        // there is only one dataset to process
        //
        vtkDataSet *in_ds = inDT->GetDataRepresentation().GetDataVTK();
        int dom = inDT->GetDataRepresentation().GetDomain();
        std::string label = inDT->GetDataRepresentation().GetLabel();
  
        //
        // Setting the source to NULL for the input will break the 
        // pipeline.
        //
        // NO LONGER A GOOD IDEA
        //in_ds->SetSource(NULL);

        //
        // We own the returned dataset because you cannot delete it if
        // it only has one reference and you want to return it.
        //
        avtDataTree_p rv = ExecuteDataTree(in_ds, dom, label);
        UpdateExtents(rv);
        currentNode++;
        UpdateProgress(currentNode, totalNodes);
        return rv;
    }
    else
    {
        //
        // there is more than one input dataset to process
        // and we need an output datatree for each
        //
        avtDataTree_p *outDT = new avtDataTree_p[nc];
        for (int j = 0; j < nc; j++)
        {
            if (inDT->ChildIsPresent(j))
            {
                outDT[j] = Execute(inDT->GetChild(j));
            }
            else
            {
                outDT[j] = NULL;
            }
        }
        avtDataTree_p rv = new avtDataTree(nc, outDT);
        delete [] outDT;
        return (rv);
    }
}


// ****************************************************************************
//  Method: avtSIMODataTreeIterator::UpdateExtents
//
//  Purpose:
//      Updates the extents that we are overriding with the current dataset.
//
//  Arguments:
//      tree    The data tree to merge into the current extents.
//
//  Programmer: Hank Childs
//  Creation:   November 6, 2001
//
//  Modifications:
//    Kathleen Bonnell, Thu Mar 11 10:14:20 PST 2004
//    DataExtents now always have a size of 1 (num components = 2).
//
//    Jeremy Meredith, Tue Feb 20 11:04:44 PST 2007
//    Add support for transformed rectilinear grids.
//
// ****************************************************************************

void
avtSIMODataTreeIterator::UpdateExtents(avtDataTree_p tree)
{
    if (*tree == NULL)
    {
        return;
    }

    if (overrideOriginalSpatialExtents)
    {
        avtDataAttributes &outAtts = GetOutput()->GetInfo().GetAttributes();
        if (originalSpatialExtents == NULL)
        {
            avtDataAttributes &atts = GetInput()->GetInfo().GetAttributes();
            avtExtents        *exts = atts.GetThisProcsOriginalSpatialExtents();
            originalSpatialExtents = new avtExtents(exts->GetDimension());
        }
        double bounds[6];
        bool gotBounds = false;
        struct {double *se; const double *xform;} info = {bounds,NULL};
        tree->Traverse(CGetSpatialExtents, (void *)&info, gotBounds);
        if (gotBounds)
        {
            originalSpatialExtents->Merge(bounds);
            avtExtents *exts = outAtts.GetThisProcsOriginalSpatialExtents();
            *exts = *originalSpatialExtents;
        }
        outAtts.GetOriginalSpatialExtents()->Clear();
    }

    if (overrideOriginalDataExtents)
    {
        avtDataAttributes &outAtts = GetOutput()->GetInfo().GetAttributes();
        if (originalDataExtents == NULL)
        {
            originalDataExtents = new avtExtents(1);
        }
        double range[2];  // who has more than 25 vars?
        bool gotBounds = false;
        tree->Traverse(CGetDataExtents, (void *) range, gotBounds);
        if (gotBounds)
        {
            originalDataExtents->Merge(range);
            avtExtents *exts = outAtts.GetThisProcsOriginalDataExtents();
            *exts = *originalDataExtents;
        }
        outAtts.GetOriginalDataExtents()->Clear();
    }
}


