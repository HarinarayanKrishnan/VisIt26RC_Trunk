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
//                                  Zoom2D.C                                 //
// ************************************************************************* //

#include <Zoom2D.h>

#include <vtkActor2D.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty2D.h>
#include <VisWindow.h>
#include <VisWindowInteractorProxy.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkFieldData.h>
#include <vtkIntArray.h>

static void
Zoom2D_SetLineProperties(vtkPolyData *guideLines, 
    int drawn, int spaced, bool hbias)
{
    vtkDataArray *props = guideLines->GetFieldData()->GetArray("LineProperties");
    if(props != NULL)
    {
        props->SetTuple1(0, drawn);
        props->SetTuple1(1, spaced);
        props->SetTuple1(2, hbias?1.:0.);
    }
}

// ****************************************************************************
//  Method: Zoom2D constructor
//
//  Programmer: Hank Childs
//  Creation:   May 22, 2000 
//
//  Modifications:
//    Akira Haddox, Thu Jul  3 13:58:11 PDT 2003
//    Added initialization of guideLine data.
//
//    Brad Whitlock, Mon Mar 13 12:11:01 PDT 2006
//    Added different initialization for Apple so all lines can be drawn
//    at once.
//
//    Brad Whitlock, Fri Oct 14 16:26:20 PDT 2011
//    Create mapper via proxy.
//
//    Kathleen Biagas, Mon Jun 11 17:16:42 MST 2012
//    Have windows follow the APPLE path.
//
// ****************************************************************************

Zoom2D::Zoom2D(VisWindowInteractorProxy &v) : ZoomInteractor(v)
{
    ctrlOrShiftPushed = false;

    guideLines       = vtkPolyData::New();

    // Stash line properties into field data so we can pass them to our mapper.
    vtkIntArray *LineProperties = vtkIntArray::New();
    LineProperties->SetNumberOfTuples(3);
    LineProperties->SetName("LineProperties");
    guideLines->GetFieldData()->AddArray(LineProperties);
    LineProperties->Delete();
    Zoom2D_SetLineProperties(guideLines, 2, 3, false);

#if defined(__APPLE__) || defined(_WIN32)
    vtkPoints *pts = vtkPoints::New();
    pts->SetNumberOfPoints(12);
    guideLines->SetPoints(pts);
    pts->Delete();

    vtkCellArray *lines  = vtkCellArray::New();
    vtkIdType  ids[8][2] = {
       {0, 1}, {2,3}, {4,5}, {6,7}, {8,1}, {5,9}, {10,2}, {6,11}
    };
    for(int i = 0; i < 8; ++i)
        lines->InsertNextCell(2, ids[i]);
    guideLines->SetLines(lines);
    lines->Delete();
#else
    vtkPoints *pts = vtkPoints::New();
    pts->SetNumberOfPoints(2);
    guideLines->SetPoints(pts);
    pts->Delete();

    vtkCellArray *lines  = vtkCellArray::New();
    vtkIdType  ids[2] = { 0, 1 };
    lines->InsertNextCell(2, ids);
    guideLines->SetLines(lines);
    lines->Delete();
#endif
    guideLinesMapper = proxy.CreateXorGridMapper();
    guideLinesMapper->SetInput(guideLines);
//    guideLinesMapper->SetDots(2, 3);
    
    guideLinesActor  = vtkActor2D::New();
    guideLinesActor->SetMapper(guideLinesMapper);
    guideLinesActor->GetProperty()->SetColor(0., 0., 0.);
}


// ****************************************************************************
//  Method: Zoom2D destructor
//
//  Programmer: Akira Haddox
//  Creation:   July 3, 2003
//
// ****************************************************************************

Zoom2D::~Zoom2D()
{
    guideLines->Delete();
    guideLinesMapper->Delete();
    guideLinesActor->Delete();
}

// ****************************************************************************
//  Method: Zoom2D::OnTimer
//
//  Purpose:
//    Handles the timer event.  For Navigate2D, this means the user has
//    pressed a mouse key and that it is time to sample the mouse position
//    to see if the view should be panned or zoomed.
//
//  Programmer: Eric Brugger
//  Creation:   October 10, 2003
//
//  Modifications:
//    Kathleen Bonnell, Tue Jun  7 14:52:29 PDT 2011
//    Use current event position instead of Last, appears to work better with
//    new vtk.
//
// ****************************************************************************

void
Zoom2D::OnTimer(void)
{
    vtkRenderWindowInteractor *rwi = Interactor;

    int Pos[2];
    rwi->GetEventPosition(Pos);

    switch (State)
    {
      case VTKIS_PAN:
        PanCamera(Pos[0], Pos[1]);

        rwi->CreateTimer(VTKI_TIMER_UPDATE);
        break;

      case VTKIS_DOLLY:
        ZoomCamera(Pos[0], Pos[1]);

        rwi->CreateTimer(VTKI_TIMER_UPDATE);
        break;

      default:
        break;
    }
}


// ****************************************************************************
//  Method: Zoom2D::StartLeftButtonAction
//
//  Purpose:
//      Handles the left button being pushed down.  For Zoom2D, this means
//      a rubber band zoom mode.
//
//  Programmer: Hank Childs
//  Creation:   May 22, 2000
//
//  Modifications:
//
//    Hank Childs, Tue Feb 19 13:11:47 PST 2002
//    Update for the VTK magic to make the zoom work.
//
//    Hank Childs, Mon Mar 18 13:48:55 PST 2002
//    Renamed from OnLeftButtonDown.
//
//    Kathleen Bonnell, Fri Dec 13 16:41:12 PST 2002
//    Removed arguments to comply with vtk's new interactor interface.
//
// ****************************************************************************

void
Zoom2D::StartLeftButtonAction()
{
    //
    // If ctrl or shift is pushed, pan, otherwise rotate.  Save which one we
    // did so we can issue the proper "End.." statement when the button is
    // released.
    //
    if (Interactor->GetControlKey()|| Interactor->GetShiftKey())
    {
        StartPan();
        ctrlOrShiftPushed = true;
    }
    else
    {
        int x, y;
        Interactor->GetEventPosition(x, y);
        StartZoom();
        StartRubberBand(x, y);
        ctrlOrShiftPushed = false;
    }
}


// ****************************************************************************
//  Method: Zoom2D::EndLeftButtonAction
//
//  Purpose:
//      Handles the left button being pushed up.  For Zoom2D, this means
//      a rubber band zoom mode.
//
//  Programmer: Hank Childs
//  Creation:   May 22, 2000
//
//  Modifications:
//
//    Hank Childs, Mon Jun 26 17:09:25 PDT 2000
//    Removed arguments to EndRubberBand.
//
//    Hank Childs, Tue Feb 19 13:11:47 PST 2002
//    Update for the VTK magic to make the zoom work.
//
//    Hank Childs, Mon Mar 18 13:48:55 PST 2002
//    Renamed from OnLeftButtonDown.
//
//    Kathleen Bonnell, Fri Dec 13 16:41:12 PST 2002
//    Removed arguments to comply with vtk's new interactor interface.
//
//    Eric Brugger, Fri Nov 21 08:02:55 PST 2003
//    Added code to call the view callback.
//
// ****************************************************************************

void
Zoom2D::EndLeftButtonAction()
{
    if (Interactor->GetControlKey()|| Interactor->GetShiftKey())
    {
        EndPan();
    }
    else
    {
        EndRubberBand();
        ZoomCamera();
        EndZoom();
    }

    IssueViewCallback();
}


// ****************************************************************************
//  Method: Zoom2D::AbortLeftButtonAction
//
//  Purpose:
//      Handles an abort action for the left button.  This means giving up on
//      the rubber band.
//
//  Programmer: Hank Childs
//  Creation:   March 18, 2002
//
//  Modifications:
//    Kathleen Bonnell, Fri Dec 13 16:41:12 PST 2002
//    Removed arguments to comply with vtk's new interactor interface.
//
// ****************************************************************************

void
Zoom2D::AbortLeftButtonAction()
{
    EndRubberBand();
    EndZoom();
}


// ****************************************************************************
//  Method: Zoom2D::StartMiddleButtonAction
//
//  Purpose:
//      Handles the middle button being pushed down.  For Zoom2D, this 
//      means standard zooming.
//
//  Programmer: Hank Childs
//  Creation:   May 22, 2000
//
//  Modifications:
//
//    Hank Childs, Mon Mar 18 13:48:55 PST 2002
//    Renamed from OnMiddleButtonDown.
//
//    Kathleen Bonnell, Fri Dec 13 16:41:12 PST 2002
//    Removed arguments to comply with vtk's new interactor interface.
//
// ****************************************************************************

void
Zoom2D::StartMiddleButtonAction()
{
    StartZoom();
}


// ****************************************************************************
//  Method: Zoom2D::EndMiddleButtonAction
//
//  Purpose:
//      Handles the middle button being pushed up.  For Zoom2D, this means
//      standard panning.
//
//  Programmer: Hank Childs
//  Creation:   May 22, 2000
//
//  Modifications:
//
//    Hank Childs, Mon Mar 18 13:48:55 PST 2002
//    Renamed from OnMiddleButtonUp.
//
//    Kathleen Bonnell, Fri Dec 13 16:41:12 PST 2002
//    Removed arguments to comply with vtk's new interactor interface.
//
//    Eric Brugger, Fri Nov 21 08:02:55 PST 2003
//    Added code to call the view callback.
//
// ****************************************************************************

void
Zoom2D::EndMiddleButtonAction()
{
    EndZoom();
    IssueViewCallback();
}


// ****************************************************************************
//  Method: Zoom2D::OnMouseWheelForward()
//
//  Purpose:
//    Handles the mouse wheel turned backward.
//
//  Arguments:
//
//  Programmer: Gunther H. Weber
//  Creation:   August 07, 2007
//
//  Modifications:
//    Gunther H. Weber, Fri Sep 28 13:48:04 PDT 2007
//    Added missing StartZoom() / EndZoom()
//
//    Brad Whitlock, Fri Mar  2 14:19:45 PST 2012
//    I added a call to issue the view callback.
//
// ****************************************************************************

void
Zoom2D::OnMouseWheelForward()
{
    StartZoom();
    ZoomCamera2D(MotionFactor * 0.2 * this->MouseWheelMotionFactor);
    EndZoom();
    IssueViewCallback(true);
}

// ****************************************************************************
//  Method: Zoom2D::OnMouseWheelBackward()
//
//  Purpose:
//    Handles the mouse wheel turned forward.  
//
//  Arguments:
//
//  Programmer: Gunther H. Weber
//  Creation:   August 07, 2007
//
//  Modifications:
//    Gunther H. Weber, Fri Sep 28 13:48:04 PDT 2007
//    Added missing StartZoom() / EndZoom()
//
//    Brad Whitlock, Fri Mar  2 14:19:45 PST 2012
//    I added a call to issue the view callback.
//
// ****************************************************************************

void
Zoom2D::OnMouseWheelBackward()
{
    StartZoom();
    ZoomCamera2D(MotionFactor * -0.2 * this->MouseWheelMotionFactor);
    EndZoom();
    IssueViewCallback(true);
}

// ****************************************************************************
//  Method: Zoom2D::StartRubberBand
//
//  Purpose:
//      Calls parent's ZoomInteractor::StartRubberBand, and if control key is
//      down, calls DrawAllGuideLines.
//
//  Arguments:
//      x       The x-coordinate of the pointer in display coordinates when
//              the button was pressed.
//      y       The y-coordinate of the pointer in display coordinates when
//              the button was pressed.
//
//  Programmer: Akira Haddox 
//  Creation:   July 3, 2003
//
//  Modifications:
//    Kathleen Bonnell, Wed Aug 18 10:10:35 PDT 2004
//    control-key now determines zoom/unzoom, shouldDrawGuides 
//    (from InteractorAtts) determines whether guidelines should be drawn.
//
//    Brad Whitlock, Mon Mar 13 11:42:54 PDT 2006
//    I added code to set the color for the guidlines actor.
//
//    Kathleen Biagas, Mon Jun 11 17:16:42 MST 2012
//    Have windows follow the APPLE path.
//
// ****************************************************************************

void
Zoom2D::StartRubberBand(int x, int y)
{
    // Must call this first: It sets some variables that we use.
    ZoomInteractor::StartRubberBand(x, y);
    if (shouldDrawGuides)
    {
        // Set the actor's color.
        double fg[3];
        proxy.GetForegroundColor(fg);
        guideLinesActor->GetProperty()->SetColor(fg[0], fg[1], fg[2]);

        vtkRenderer *ren = proxy.GetBackground();
        ren->AddActor2D(guideLinesActor);
       
        lastGuideX = x;
        lastGuideY = y;
#if defined(__APPLE__) || defined(_WIN32)
        UpdateRubberBand(x,y,x,y,x,y);
#else
        DrawAllGuideLines(x, y, x, y);
#endif
    }
}


// ****************************************************************************
//  Method: Zoom2D::EndRubberBand
//
//  Purpose:
//      Removes the guideLines actor if it's in place, then call's parent's
//      ZoomInteractor::EndRubberBand.
//
//  Programmer: Akira Haddox
//  Creation:   July 3, 2003
// ****************************************************************************

void
Zoom2D::EndRubberBand()
{
    vtkRenderer *ren = proxy.GetBackground();
    ren->RemoveActor2D(guideLinesActor);

    ZoomInteractor::EndRubberBand();
}


// ****************************************************************************
//  Method: Zoom2D::UpdateRubberBand
//
//  Purpose:
//      Calls parent's ZoomInteractor::UpdateRubberBand to draw the actual
//      rubber band, and if control key is down, makes the call to draw
//      the guidelines.
//
//  Arguments:
//      aX      The x-coordinate of the anchor in display coordinates.
//      aY      The y-coordinate of the anchor in display coordinates.
//      lX      The x-coordinate of the last corner in display coordinates.
//      lY      The y-coordinate of the last corner in display coordinates.
//      nX      The x-coordinate of the new corner in display coordinates.
//      nY      The y-coordinate of the new corner in display coordinates.
//
//  Programmer: Akira Haddox 
//  Creation:   July 3, 2003
//
//  Modifications:
//    Kathleen Bonnell, Wed Aug 18 10:10:35 PDT 2004
//    control-key now determines zoom/unzoom, shouldDrawGuides 
//    (from InteractorAtts) determines whether guidelines should be drawn.
//
//    Brad Whitlock, Mon Mar 13 12:22:13 PDT 2006
//    Added code for Apple so all lines can be drawn at once.
//
//    Kathleen Biagas, Mon Jun 11 17:16:42 MST 2012
//    Have windows follow the APPLE path.
//
// ****************************************************************************

void
Zoom2D::UpdateRubberBand(int aX, int aY, int lX, int lY, int nX, int nY)
{
    if (nX == lX && nY == lY)
    {
        //
        // No update neccessary (from Zoom2D or ZoomInteractor)
        //
        return;
    }
    
    ZoomInteractor::UpdateRubberBand(aX, aY, lX, lY, nX, nY);

    // We have to keep track of the last x and y positions in this class,
    // since lX and lY and not always the last values used in a previous
    // call. The rubberBand doesn't need it exactly, but we do.
    if (shouldDrawGuides)
    {
#if defined(__APPLE__) || defined(_WIN32)
        int x0 = (aX < nX) ? aX : nX;
        int x1 = (aX > nX) ? aX : nX;
        int y0 = (aY < nY) ? aY : nY;
        int y1 = (aY > nY) ? aY : nY;
        int xMin = (int)(canvasDeviceMinX);
        int yMin = (int)(canvasDeviceMinY);
        int xMax = (int)(canvasDeviceMaxX);
        int yMax = (int)(canvasDeviceMaxY);
        vtkViewport *ren = proxy.GetBackground();
        vtkPoints *pts = guideLines->GetPoints();
        pts->SetPoint(0, (double) xMin, (double) y0, 0.);
        pts->SetPoint(1, (double) x0,   (double) y0, 0.);
        pts->SetPoint(2, (double) x1,   (double) y0, 0.);
        pts->SetPoint(3, (double) xMax, (double) y0, 0.);
        pts->SetPoint(4, (double) xMin, (double) y1, 0.);
        pts->SetPoint(5, (double) x0,   (double) y1, 0.);
        pts->SetPoint(6, (double) x1,   (double) y1, 0.);
        pts->SetPoint(7, (double) xMax, (double) y1, 0.);
        pts->SetPoint(8, (double) x0,   (double) yMin, 0.);
        pts->SetPoint(9, (double) x0,   (double) yMax, 0.);
        pts->SetPoint(10, (double) x1,  (double) yMin, 0.);
        pts->SetPoint(11, (double) x1,  (double) yMax, 0.);
        guideLinesMapper->RenderOverlay(ren, guideLinesActor);
#else
        UpdateGuideLines(aX, aY, lastGuideX, lastGuideY, nX, nY);
#endif
        lastGuideX = nX;
        lastGuideY = nY;
    }
}


// ****************************************************************************
//  Function: GetGuideSegment
//
//  Purpose:
//      Determines what pixels should be drawn based on the anchor location,
//      the location of the old pixel and the location of the new pixel.
//      It is identical to GetSegement used in ZoomInteractor for rubberBands
//      except that the one pixel adjustment is in the oppossite direstion, and
//      for the other point.
//
//  Note:       This is meant for extending or retracting a segment.  If
//              both dimensions vary for the new pixel, this should not be 
//              used.
//
//  Arguments:
//      a       The location of the anchor in one dimension in display coords.
//      l       The location of the last coord in one dim in display coords.
//      n       The location of the new coord in one dim in display coords.
//      outl    Where the last coord portion of the segment should be placed.
//      newl    Where the new coord portion of the segment should be placed.
//
//  Programmer: Akira Haddox 
//  Creation:   July 3, 2003
//
// ****************************************************************************

static inline void
GetGuideSegment(int a, int l, int n, int &outl, int &newl)
{
    outl = l;
    newl = n;
    if (abs(l-a) > abs(n-a))
    {
        //
        // We are moving towards the anchor.
        //
        int offset = (l < a ? 1 : -1);
        outl += offset;
    }
    else
    {
        //
        // We are moving away from the anchor.
        //
        int offset = (n < a ? 1 : -1);
        newl += offset;
    }
}


// ****************************************************************************
//  Method: Zoom2D::UpdateGuideLines
//
//  Purpose:
//      Updates the guide lines to new positions, with minimized flashing. 
//      This method only updates the guide lines, assuming that guide
//      lines already exist on the screen (to be erased if needed).
//      Uses same naming conventions as described in DrawGuideLines.
//
//  Arguments:
//      aX      The x-coordinate of the anchor in display coordinates.
//      aY      The y-coordinate of the anchor in display coordinates.
//      lX      The x-coordinate of the last corner in display coordinates.
//      lY      The y-coordinate of the last corner in display coordinates.
//      nX      The x-coordinate of the new corner in display coordinates.
//      nY      The y-coordinate of the new corner in display coordinates.
//
//  Programmer: Akira Haddox 
//  Creation:   July 3, 2003
//
// ****************************************************************************

void
Zoom2D::UpdateGuideLines(int aX, int aY, int lX, int lY, int nX, int nY)
{
    // Certain lines need to be completely erased and redrawn. Those we
    // draw using DrawGuideLines with this array. Lines which need to be
    // 'adjusted' (like how the rubberBand is adjusted in ZoomInteractor)
    // are done manually in this function.
    bool refresh[8];
    int i;
    for (i = 0; i < 8; ++i)
    {
        refresh[i] = false;
    }
    
    // If any lines will be or are down on a border, or we're moving over
    // a line, just erase and redraw. It's more work than it's worth
    // to make it work, and it doesn't happen often enough for the flashing
    // to be noticable.
    if (((nX-aX)*(lX-aX) <= 0) || ((nY-aY)*(lY-aY) <= 0))
    {
        for (i = 0; i < 8; ++i)
            refresh[i] = true;
    }
    else
    {
        bool cornerPartialRefresh = true;
        // If both coordinates have changed, the following refreshes: 
        // refresh: corner [h/v], shareX[h], shareY[v]
        if (nX != lX && nY != lY)
        {
            refresh[2] = refresh[3] = refresh[4] = refresh[7] = true;
            cornerPartialRefresh = false;
        }
        
        // If the x coordinate has changed, the following refreshes:
        // refresh: corner[v], shareY[v]
        // partials: shareY[h], corner[h]
        if(nX != lX)
        {
            refresh[3] = refresh[7] = true;
            
            // Partially refresh shareY[h]
//            guideLinesMapper->SetHorizontalBias(true);
            Zoom2D_SetLineProperties(guideLines, 2, 3, true);

            int fromX, toX;
            GetGuideSegment(aX, lX, nX, fromX, toX);
        
            DrawGuideLine(fromX, aY, toX, aY);

            // Partially refresh corner[h]
            if (cornerPartialRefresh)
                DrawGuideLine(fromX, nY, toX, nY);
        }
       
        // If the y coordinate has changed, the following refreshes:
        // refresh: corner[h], shareX[h]
        // partials: shareX[v], corner[v]
        if(nY != lY)
        {
            refresh[2] = refresh[4] = true;
            
            // Partially refresh shareX[v]
//            guideLinesMapper->SetHorizontalBias(false);
            Zoom2D_SetLineProperties(guideLines, 2, 3, false);
            int fromY, toY;
            GetGuideSegment(aY, lY, nY, fromY, toY);
            DrawGuideLine(aX, fromY, aX, toY);
            
            //Partially refresh corner[v]
            if (cornerPartialRefresh)
                DrawGuideLine(nX, fromY, nX, toY);
        }
    }
   
    // Completely refreshed lines
    // Draw the new lines
    DrawGuideLines(aX, aY, nX, nY, refresh);
    // Erase the old lines
    DrawGuideLines(aX, aY, lX, lY, refresh);
}


// ****************************************************************************
//  Method: Zoom2D::DrawAllGuideLines
//
//  Purpose:
//      Draw all the guide lines without considering what may already be
//      on the screen. Useful for initial drawing, and for erasing all
//      lines on the screen.
//
//  Arguments:
//      aX:     Anchor's x display coordinate
//      aY:     Anchor's y display coordinate
//      nX:     Oppossite corner's x display coordinate
//      nY:     Oppossite corner's y display coordinate
//
//  Programmer: Akira Haddox 
//  Creation:   July 3, 2003
//
// ****************************************************************************

void
Zoom2D::DrawAllGuideLines(int aX, int aY, int nX, int nY)
{
    const bool drawAll[8] = { true, true, true, true, true, true, true, true };
    DrawGuideLines(aX, aY, nX, nY, drawAll);
}


// ****************************************************************************
//  Method: Zoom2D::DrawGuideLines
//
//  Purpose:
//      Draws the guidelines requested. The four points are mentioned
//      as anchor (aX, aY), corner (nX, nY), shareX (aX, nY), shareY (nX, aY).
//      This function will not draw duplicate lines if they overlap.
//      All lines are drawn from the boundary to the box.
//
//  Arguments:
//      aX:     Anchor's x display coordinate
//      aY:     Anchor's y display coordinate
//      nX:     Opposite corner's x display coordinate
//      nY:     Opposite corner's y display coordinate
//      which:  Which lines to draw. Stored in pairs of the order of:
//              anchor, corner, shareX, shareY. Even values are horizontal
//              line requests, odd values are vertical requests.
//
//              which[0]: anchor horizontal     which[1]: anchor vertical
//              which[2]: corner horizontal     which[3]: corner vertical
//              which[4]: shareX horizontal     which[5]: shareX vertical
//              which[6]: shareY horizontal     which[7]: shareY vertical
//
//  Programmer: Akira Haddox 
//  Creation:   July 3, 2003
//
// ****************************************************************************

void
Zoom2D::DrawGuideLines(int aX, int aY, int nX, int nY, const bool which[8])
{
    int xMin = (int)(canvasDeviceMinX);
    int yMin = (int)(canvasDeviceMinY);
    int xMax = (int)(canvasDeviceMaxX);
    int yMax = (int)(canvasDeviceMaxY);

    // Existance tests for certain lines
    bool shareXHorizontal = (aY != nY) && which[4];
    bool shareXVertical = (aX != nX) && which[5];
    bool shareYHorizontal = (aY != nY) && which[6];
    bool shareYVertical = (aX != nX) && which[7];
    
    // If the anchor is to the left of the corner
    if (aX < nX)
    {
        // The anchor horizontal line goes to the left
        if (which[0])
            DrawGuideLine(xMin, aY, aX, aY);
        // The corner horizontal line goes to the right
        if (which[2])
            DrawGuideLine(xMax, nY, nX, nY);
        // The shareX horizontal line goes to the left
        if (shareXHorizontal)
            DrawGuideLine(xMin, nY, aX, nY);
        // The shareY horizontal line goes to the right
        if (shareYHorizontal)
            DrawGuideLine(xMax, aY, nX, aY);
    }
    else
    {
        // The anchor horizontal line goes to the right
        if (which[0])
            DrawGuideLine(xMax, aY, aX, aY);
        // The corner horizontal line goes to the left
        if (which[2])
            DrawGuideLine(xMin, nY, nX, nY);
        // The shareX horizontal line goes to the right
        if (shareXHorizontal)
            DrawGuideLine(xMax, nY, aX, nY);
        // The shareY horizontal line goes to the left
        if (shareYHorizontal)
            DrawGuideLine(xMin, aY, nX, aY);
    }

    // If the anchor is below the corner
    if (aY < nY)
    {
        // The anchor vertical line goes down
        if (which[1])
            DrawGuideLine(aX, yMin, aX, aY);
        // The corner vertical line goes up
        if (which[3])
            DrawGuideLine(nX, yMax, nX, nY);
        // The shareX vertical line goes up
        if (shareXVertical)
            DrawGuideLine(aX, yMax, aX, nY);
        // The shareY vertical line goes down
        if (shareYVertical)
            DrawGuideLine(nX, yMin, nX, aY);
    }
    else
    {
        // The anchor vertical line goes up
        if (which[1])
            DrawGuideLine(aX, yMax, aX, aY);
        // The corner vertical line goes down
        if (which[3])
            DrawGuideLine(nX, yMin, nX, nY);
        // The shareX vertical line goes down
        if (shareXVertical)
            DrawGuideLine(aX, yMin, aX, nY);
        // The shareY vertical line goes up
        if (shareYVertical)
            DrawGuideLine(nX, yMax, nX, aY);
    }
}

// ****************************************************************************
//  Method: Zoom2D::DrawGuideLine
//
//  Purpose:
//      Draws a guide line.
//
//  Arguments:
//      x1      The x-coordinate of the first endpoint in display coordinates.
//      y1      The y-coordinate of the first endpoint in display coordinates.
//      x2      The x-coordinate of the second endpoint in display coordinates.
//      y2      The y-coordinate of the second endpoint in display coordinates.
//
//  Programmer: Akira Haddox
//  Creation:   July 3, 2003
//
// ****************************************************************************

void
Zoom2D::DrawGuideLine(int x1, int y1, int x2, int y2)
{
    vtkViewport *ren = proxy.GetBackground();

    vtkPoints *pts = guideLines->GetPoints();
    pts->SetPoint(0, (double) x1, (double) y1, 0.);   
    pts->SetPoint(1, (double) x2, (double) y2, 0.);   
    guideLinesMapper->RenderOverlay(ren, guideLinesActor);
}


// ****************************************************************************
//  Method: Zoom2D::PanCamera
//
//  Purpose:
//    Handle panning the camera.
//
//  Programmer: Eric Brugger
//  Creation:   October 10, 2003
//
//  Modifications:
//    Eric Brugger, Thu May 26 12:35:49 PDT 2011
//    Remove an unnecessary render call.
//
// ****************************************************************************

void
Zoom2D::PanCamera(const int x, const int y)
{
    vtkRenderWindowInteractor *rwi = Interactor;

    if ((OldX != x) || (OldY != y))
    {
        //
        // Determine the size of the window.
        //
        int       size[2];

        rwi->GetSize(size);

        //
        // Get the current view information.
        //
        VisWindow *vw = proxy;

        double    viewport[4];
        double    pan[2];

        avtView2D newView2D = vw->GetView2D();

        newView2D.GetActualViewport(viewport, size[0], size[1]);
        
        pan[0] = (double)(x - OldX) /
                 ((viewport[1] - viewport[0]) * (double)(size[0])) *
                 (newView2D.window[1] - newView2D.window[0]);
        pan[1] = (double)(y - OldY) /
                 ((viewport[3] - viewport[2]) * (double)(size[1])) *
                 (newView2D.window[3] - newView2D.window[2]);

        newView2D.window[0] -= pan[0];
        newView2D.window[1] -= pan[0];
        newView2D.window[2] -= pan[1];
        newView2D.window[3] -= pan[1];

        OldX = x;
        OldY = y;

        vw->SetView2D(newView2D);
    }
}

// ****************************************************************************
//  Method: Zoom2D::ZoomCamera
//
//  Purpose:
//      Zooms the camera to be between the display coordinates specified by
//      start and end X and Y.
//
//  Programmer:  Eric Brugger
//  Creation:    March 26, 2002
//
//  Modifications:
//    Akira Haddox, Thu Jul  3 14:06:42 PDT 2003
//    Changed so no zooming occurs if zooming area is a line.
//    Also clamped zooming to the boundries (necessary since we round
//    outwards in our zooming, to always be able to reach the boundry).
//
//    Kathleen Bonnell, Mon Jul  7 15:59:11 PDT 2003 
//    Copy newView2D from current View2D to preserve fields which are
//    not being overwritten here. 
// 
//    Eric Brugger, Thu Oct  9 17:03:59 PDT 2003
//    Modified to handle full frame properly.
//
//    Kathleen Bonnell, Tue Aug  3 15:22:58 PDT 2004 
//    Modified to handle Un-Zoom mode (controlKey is pressed.). 
//
//    Eric Brugger, Thu May 26 12:32:21 PDT 2011
//    Remove an unnecessary render call.
//
//    Marc Durant, Mon Dec 19 14:40:00 MST 2011
//    Cancelling zoom action if mouse movement was too small.
//
// ****************************************************************************

void
Zoom2D::ZoomCamera(void)
{
    if (!SufficientDistanceMoved())
    {
        //
        // This is a point, line, or very, very small rectangle
        //
        return;
    }

    //
    // Figure out the lower left and upper right hand corners in
    // display space.
    //
    double leftX   = (double) (anchorX < lastX ? anchorX : lastX);
    double rightX  = (double) (anchorX > lastX ? anchorX : lastX);
    double bottomY = (double) (anchorY < lastY ? anchorY : lastY);
    double topY    = (double) (anchorY > lastY ? anchorY : lastY);
    double dummyZ  = 0.;

    //
    // Convert them to world coordinates.
    //
    vtkRenderer *canvas = proxy.GetCanvas();

    canvas->DisplayToNormalizedDisplay(leftX, topY);
    canvas->NormalizedDisplayToViewport(leftX, topY);
    canvas->ViewportToNormalizedViewport(leftX, topY);

    // Clamp the boundaries.
    if (leftX < 0.0)
        leftX = 0.0;
    if (topY > 1.0)
        topY = 1.0;

    canvas->NormalizedViewportToView(leftX, topY, dummyZ);
    canvas->ViewToWorld(leftX, topY, dummyZ);

    canvas->DisplayToNormalizedDisplay(rightX, bottomY);
    canvas->NormalizedDisplayToViewport(rightX, bottomY);
    canvas->ViewportToNormalizedViewport(rightX, bottomY);

    // Clamp the boundaries.
    if (rightX > 1.0)
        rightX = 1.0;
    if (bottomY < 0.0)
        bottomY = 0.0;

    canvas->NormalizedViewportToView(rightX, bottomY, dummyZ);
    canvas->ViewToWorld(rightX, bottomY, dummyZ);

    //
    // Set the new view window.
    //
    VisWindow *vw = proxy;

    double s = 1.;

    avtView2D newView2D = vw->GetView2D();

    //
    // Handle full frame mode.
    //
    if (newView2D.fullFrame)
    {
        int       size[2];

        vtkRenderWindowInteractor *rwi = Interactor;
        rwi->GetSize(size);

        s = newView2D.GetScaleFactor(size);
    }

    if (!controlKeyDown)
    {
        newView2D.window[0] = leftX;
        newView2D.window[1] = rightX;
        newView2D.window[2] = bottomY;
        newView2D.window[3] = topY;
    }
    else
    {
        double win1[4], win2[4], win3[4], win4[4];

        // window created by rubber band
        win1[0] = leftX;
        win1[1] = rightX;
        win1[2] = bottomY;
        win1[3] = topY;
        double win1_w = win1[1] - win1[0];
        double win1_h = win1[3] - win1[2];

        // the current window 
        win2[0] = newView2D.window[0];
        win2[1] = newView2D.window[1];
        win2[2] = newView2D.window[2];
        win2[3] = newView2D.window[3];
        double win2_w = win2[1] - win2[0];
        double win2_h = win2[3] - win2[2];

        double scaleX = win1_w / win2_w;
        double scaleY = win1_h / win2_h;

        if (scaleY < scaleX)
        {
            double midX = (win2[0] + win2[1]) / 2.;
            double halfw = (win2_h) * (win1_w / win1_h) / 2.;
            win3[0] = midX - halfw;
            win3[1] = midX + halfw;
            win3[2] = win2[2];
            win3[3] = win2[3];
        }
        else 
        {
            double midY = (win2[2] + win2[3]) /2.;
            double halfh = (win2_w) * (win1_h / win1_w) / 2.;
            win3[0] = win2[0];
            win3[1] = win2[1]; 
            win3[2] = midY - halfh;
            win3[3] = midY + halfh;
        }

        double win3_w = (win3[1] - win3[0]);
        double win3_h = (win3[3] - win3[2]);

        win4[0] = ((win1[0] - win2[0]) / win2_w) * win3_w + win3[0];
        win4[1] = ((win1[1] - win2[0]) / win2_w) * win3_w + win3[0];
        win4[2] = ((win1[2] - win2[2]) / win2_h) * win3_h + win3[2];
        win4[3] = ((win1[3] - win2[2]) / win2_h) * win3_h + win3[2];

        double win4_w = (win4[1] - win4[0]);
        double win4_h = (win4[3] - win4[2]);

        newView2D.window[0] = (win3[0] - win4[0]) * win3_w / win4_w + win3[0];
        newView2D.window[1] = (win3[1] - win4[0]) * win3_w / win4_w + win3[0];
        newView2D.window[2] = (win3[2] - win4[2]) * win3_h / win4_h + win3[2];
        newView2D.window[3] = (win3[3] - win4[2]) * win3_h / win4_h + win3[2];
    }

    // handle full-frame;
    newView2D.window[2] /= s;
    newView2D.window[3] /= s;

    vw->SetView2D(newView2D);
}


// ****************************************************************************
//  Method: Zoom2D::ZoomCamera
//
//  Purpose:
//    Handle zooming the camera.
//
//  Programmer: Eric Brugger
//  Creation:   April 12, 2002
//
//  Modifications:
//    Eric Brugger, Mon Nov 22 08:23:50 PST 2004
//    I moved the code for this routine into a method in the base class.
//
// ****************************************************************************

void
Zoom2D::ZoomCamera(const int x, const int y)
{
    ZoomCamera2D(x, y);
}
