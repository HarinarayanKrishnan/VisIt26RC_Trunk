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

#include <AnimationActions.h>
#include <ViewerFileServer.h>
#include <ViewerPlotList.h>
#include <ViewerProperties.h>
#include <ViewerWindow.h>
#include <ViewerWindowManager.h>
#include <ViewerEngineManager.h>
#include <ViewerActionManager.h>
#include <DDTManager.h>
#include <avtSimulationInformation.h>
#include <avtDatabaseMetaData.h>

#include <DatabaseCorrelationList.h>
#include <DebugStream.h>

#include <QIcon>
#include <QPixmap>

// Include icons
#include <animationreversestep.xpm>
#include <animationreverseplayon.xpm>
#include <animationreverseplayoff.xpm>
#include <animationstopon.xpm>
#include <animationstopoff.xpm>
#include <animationplayon.xpm>
#include <animationplayoff.xpm>
#include <animationforwardstep.xpm>

///////////////////////////////////////////////////////////////////////////////

TimeSliderReverseStepAction::TimeSliderReverseStepAction(ViewerWindow *win) :
    ViewerAction(win)
{
    SetAllText(tr("Reverse step"));
    SetToolTip(tr("Step back one frame"));
    if (!GetViewerProperties()->GetNowin())
        SetIcon(QIcon(QPixmap(animationreversestep_xpm)));
}

void
TimeSliderReverseStepAction::Execute()
{
     windowMgr->PrevFrame(windowId);
}

bool
TimeSliderReverseStepAction::Enabled() const
{
    return window->GetPlotList()->HasActiveTimeSlider() &&
           window->GetPlotList()->GetNumPlots() > 0;
}

///////////////////////////////////////////////////////////////////////////////

AnimationReversePlayAction::AnimationReversePlayAction(ViewerWindow *win) :
    ViewerToggleAction(win)
{
    SetAllText(tr("Reverse play"));
    SetToolTip(tr("Play animation in reverse"));
    if (!GetViewerProperties()->GetNowin())
        SetIcons(QPixmap(animationreverseplayon_xpm), QPixmap(animationreverseplayoff_xpm));
}

void
AnimationReversePlayAction::Execute()
{
     windowMgr->ReversePlay(windowId);
}

bool
AnimationReversePlayAction::Enabled() const
{
    return window->GetPlotList()->HasActiveTimeSlider() &&
           window->GetPlotList()->GetNumPlots() > 0;
}

bool
AnimationReversePlayAction::Checked() const
{
    return (window->GetPlotList()->GetAnimationAttributes().GetAnimationMode() ==
            AnimationAttributes::ReversePlayMode);
}

///////////////////////////////////////////////////////////////////////////////

AnimationStopAction::AnimationStopAction(ViewerWindow *win) :
    ViewerToggleAction(win)
{
    SetAllText(tr("Stop"));
    SetToolTip(tr("Stop animation"));
    if (!GetViewerProperties()->GetNowin())
        SetIcons(QPixmap(animationstopon_xpm), QPixmap(animationstopoff_xpm));
}

void
AnimationStopAction::Execute()
{
    if (DDTManager::isDDTSim(window))
    {
        const EngineKey &key = window->GetPlotList()->GetEngineKey();
        const avtDatabaseMetaData *md = ViewerEngineManager::Instance()->GetSimulationMetaData(key);
        ViewerEngineManager::Instance()->SendSimulationCommand(key, DDTSIM_CMD_STOP, "");
    }
    else
        windowMgr->Stop(windowId);
}

bool 
AnimationStopAction::Enabled() const
{
    if (DDTManager::isDDTSim(window))
        return true;
    else
        return window->GetPlotList()->HasActiveTimeSlider() &&
            window->GetPlotList()->GetNumPlots() > 0;
}

bool
AnimationStopAction::Checked() const
{
    if (DDTManager::isDDTSim(window))
    {
        const EngineKey &key = window->GetPlotList()->GetEngineKey();
        const avtDatabaseMetaData *md = ViewerEngineManager::Instance()->GetSimulationMetaData(key);
        if (md && key.IsSimulation()){
            return md->GetSimInfo().GetMode()==avtSimulationInformation::Stopped;
        }
        return false;
    }
    else
        return (window->GetPlotList()->GetAnimationAttributes().GetAnimationMode() ==
                AnimationAttributes::StopMode);
}

///////////////////////////////////////////////////////////////////////////////

AnimationPlayAction::AnimationPlayAction(ViewerWindow *win) :
    ViewerToggleAction(win)
{
    SetAllText(tr("Play"));
    SetToolTip(tr("Play animation"));
    if (!GetViewerProperties()->GetNowin())
        SetIcons(QPixmap(animationplayon_xpm), QPixmap(animationplayoff_xpm));
}

void
AnimationPlayAction::Execute()
{
    if (DDTManager::isDDTSim(window))
    {
        const EngineKey &key = window->GetPlotList()->GetEngineKey();
        const avtDatabaseMetaData *md = ViewerEngineManager::Instance()->GetSimulationMetaData(key);
        if (md->GetSimInfo().GetMode()==avtSimulationInformation::Running)
            ViewerEngineManager::Instance()->SendSimulationCommand(key, DDTSIM_CMD_STOP, "");
        else
            ViewerEngineManager::Instance()->SendSimulationCommand(key, DDTSIM_CMD_PLAY, "");
    }
    else
        windowMgr->Play(windowId);
}

bool
AnimationPlayAction::Enabled() const
{
    if (DDTManager::isDDTSim(window))
        return true;
    else
        return window->GetPlotList()->HasActiveTimeSlider() &&
            window->GetPlotList()->GetNumPlots() > 0;
}

bool
AnimationPlayAction::Checked() const
{
    if (DDTManager::isDDTSim(window))
    {
        const EngineKey &key = window->GetPlotList()->GetEngineKey();
        const avtDatabaseMetaData *md = ViewerEngineManager::Instance()->GetSimulationMetaData(key);
        if (md && key.IsSimulation())
            return md->GetSimInfo().GetMode()==avtSimulationInformation::Running;
        return true;
    }
    else
        return window->GetPlotList()->GetAnimationAttributes().GetAnimationMode() ==
                AnimationAttributes::PlayMode;
}

///////////////////////////////////////////////////////////////////////////////

TimeSliderForwardStepAction::TimeSliderForwardStepAction(ViewerWindow *win) :
    ViewerAction(win)
{
    SetAllText(tr("Forward step"));
    SetToolTip(tr("Step forward one frame"));
    if (!GetViewerProperties()->GetNowin())
        SetIcon(QIcon(QPixmap(animationforwardstep_xpm)));
}

void
TimeSliderForwardStepAction::Execute()
{
     if (DDTManager::isDDTSim(window))
     {
         ViewerEngineManager::Instance()->SendSimulationCommand(
                     window->GetPlotList()->GetEngineKey(), DDTSIM_CMD_STEP, "");
     }
     else
         windowMgr->NextFrame(windowId);
}

bool
TimeSliderForwardStepAction::Enabled() const
{
    if (DDTManager::isDDTSim(window))
        return true;
    else
        return window->GetPlotList()->HasActiveTimeSlider() &&
            window->GetPlotList()->GetNumPlots() > 0;
}

///////////////////////////////////////////////////////////////////////////////

AnimationSetNFramesAction::AnimationSetNFramesAction(ViewerWindow *win) :
    ViewerAction(win)
{
    DisableVisual();
}

// ****************************************************************************
// Method: AnimationSetNFramesAction::Execute
//
// Purpose: 
//   Executes the set n frames action, which is around for keyframing.
//
// Programmer: Brad Whitlock
// Creation:   Tue Feb 3 16:19:53 PST 2004
//
// Modifications:
//   Brad Whitlock, Tue Feb 3 16:20:22 PST 2004
//   I made it update the keyframe attributes instead of the window info.
//
// ****************************************************************************

void
AnimationSetNFramesAction::Execute()
{
    window->GetPlotList()->SetNKeyframes(args.GetNFrames());
    windowMgr->UpdateKeyframeAttributes();
}

///////////////////////////////////////////////////////////////////////////////

SetTimeSliderStateAction::SetTimeSliderStateAction(ViewerWindow *win) :
    ViewerAction(win)
{
    DisableVisual();
}

void
SetTimeSliderStateAction::Execute()
{
    windowMgr->SetFrameIndex(args.GetStateNumber(), windowId);
}

///////////////////////////////////////////////////////////////////////////////

SetActiveTimeSliderAction::SetActiveTimeSliderAction(ViewerWindow *win) :
    ViewerAction(win)
{
    DisableVisual();
}

// ****************************************************************************
// Method: SetActiveTimeSliderAction::Execute
//
// Purpose: 
//   This method executes the SetActiveTimeSlider action.
//
// Programmer: Brad Whitlock
// Creation:   Wed Mar 31 09:50:32 PDT 2004
//
// Modifications:
//   Brad Whitlock, Wed Mar 31 09:50:37 PDT 2004
//   Added code to expand the time slider name in case it contains relative
//   paths, etc.
//
//   Brad Whitlock, Mon Aug 30 14:13:55 PST 2004
//   I added code to prevent the keyframing time slider from incorrectly being
//   expanded as a database name.
//
// ****************************************************************************

void
SetActiveTimeSliderAction::Execute()
{
    //
    // If we don't find a correlation for the desired time slider, expand the
    // time slider name and try and use the expanded time slider name.
    //
    ViewerFileServer *fs = ViewerFileServer::Instance();
    DatabaseCorrelationList *cL = fs->GetDatabaseCorrelationList();
    std::string tsName(args.GetDatabase());

    if(cL->FindCorrelation(tsName) == 0)
    {
        // If we're in keyframe mode, only expand the database name if the
        // name of the time slider is not the same as the keyframing time
        // slider.
        if(window->GetPlotList()->GetKeyframeMode() ? (tsName != KF_TIME_SLIDER) : true)
        {
            std::string host, db;
            fs->ExpandDatabaseName(tsName, host, db);
            debug3 << "The new time slider had to be expanded before we could "
                      "use it. It was called: "
                   << args.GetDatabase().c_str() << ". Now it is called: "
                   << tsName.c_str() << endl;
        }
    } 

    windowMgr->SetActiveTimeSlider(tsName, windowId);
}
