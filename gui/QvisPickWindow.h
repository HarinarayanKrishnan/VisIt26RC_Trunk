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

#ifndef QVIS_PICK_WINDOW_H
#define QVIS_PICK_WINDOW_H
#include <gui_exports.h>
#include <QvisPostableWindowObserver.h>

#define MAX_PICK_TABS 25
#define MIN_PICK_TABS 8 

class QCheckBox;
class QComboBox;
class QLineEdit;
class QTextEdit;
class QSpinBox;
class QTabWidget;
class QTextEdit;

class QvisTimeQueryOptionsWidget;
class QvisVariableButton;
class PickAttributes;
class PlotList;

// ****************************************************************************
// Class: PickRecord
//
// Purpose:
//   This class is a container for information about a Pick. It  can later be
//   used to focus DDT on the domain contain that Pick.
//
// Notes:
//
// Programmer: Jonathan Byrd (Allinea Software)
// Creation:   December 18, 2011
//
// Modifications:
// ****************************************************************************
class PickRecord {
public:
    int domain;
    std::string simulation;

    PickRecord();
    PickRecord(int dom, std::string &sim);
    void reset();
};

// ****************************************************************************
// Class: QvisPickWindow
//
// Purpose:
//   This class is a postable window that watches pick 
//   attributes and always presents the current state.
//
// Notes:      
//
// Programmer: Kathleen Bonnell 
// Creation:   December 3, 2001 
//
// Modifications:
//   Brad Whitlock, Fri Feb 15 17:15:30 PST 2002
//   Changed the pages and infoLists arrays to be length MAX_PICK_TABS to
//   get around a weird memory problem where I could not seem to free those
//   arrays.
//
//   Kathleen Bonnell, Tue Mar 26 15:23:11 PST 2002  
//   Added ClearPages method. 
//
//   Kathleen Bonnell, Fri Nov 15 09:07:36 PST 2002  
//   Removed makeDefault and reset. 
//
//   Kathleen Bonnell, Fri Dec 27 14:09:40 PST 2002   
//   Added useNodeCoords checkbox.
//
//   Brad Whitlock, Wed Aug 27 08:35:44 PDT 2003
//   Added the autoShow flag and CreateNode and SetFromNode methods.
//
//   Brad Whitlock, Tue Sep 9 09:02:03 PDT 2003
//   I made it use QTextEdit instead of QListWidget.
//
//   Kathleen Bonnell, Wed Sep 10 08:02:02 PDT 2003 
//   Added the savePicks checkbox. Remove AddInformation, no longer necessary.
//
//   Kathleen Bonnell, Tue Nov 18 14:03:22 PST 2003 
//   Added logicalZone checkbox. 
//
//   Kathleen Bonnell, Wed Dec 17 15:19:46 PST 2003 
//   More widgets to support more user-settable PickAtts.
//
//   Kathleen Bonnell, Thu Apr  1 18:42:52 PST 2004 
//   Added timeCurveCheckBox. 
//
//   Kathleen Bonnell, Wed Jun  9 09:41:15 PDT 2004 
//   Added conciseOutputCheckBox, showMeshNameCheckBox, showTimestepCheckBox. 
//
//   Brad Whitlock, Fri Dec 10 09:47:30 PDT 2004
//   I added a pick variable button so it is a little easier to select
//   variables.
//
//   Kathleen Bonnell, Wed Dec 15 08:20:11 PST 2004
//   Add checkbox and slot for displayGlobalIds. 
//
//   Kathleen Bonnell, Tue Dec 28 16:23:43 PST 2004
//   Add checkbox and slot for displayPickLetter. 
//
//   Kathleen Bonnell, Mon Oct 31 10:39:28 PST 2005 
//   Added spinbox for userMaxPickTabs, and ResizeTabs method.
//
//   Ellen Tarwater, Fri May 18, 2007
//   Added saveCount and SavePickText to write pick text to a file.
//
//   Hank Childs, Thu Aug 30 14:15:11 PDT 2007
//   Added spreadsheetCheckBox.
//
//   Cyrus Harrison, Thu Sep 13 12:12:32 PDT 2007
//   Added floatFormatLineEdit and floatFormatProcessText to support
//   user settable floating point format string.
//
//   Kathleen Bonnell, Tue Nov 27 15:44:08 PST 2007 
//   Added preserveCoord combo box.
//
//   Kathleen Bonnell, Thu Nov 29 15:32:32 PST 2007 
//   Added defaultAutoShow, defaultSavePicks, defaultNumTabs, clearPicks(). 
//
//   Brad Whitlock, Wed Apr  9 11:29:38 PDT 2008
//   QString for caption, shortName.
//
//   Gunther H. Weber, Fri Aug 15 10:22:46 PDT 2008
//   Added buttons, methods and variables for repick with and without
//   bringing up a Spreadsheet plot.
//
//   Kathleen Bonnell, Thu Mar  3 08:09:10 PST 2011
//   Added timeCurveType combo box.
//
//   Kathleen Biagas, Fri Aug 26 11:10:16 PDT 2011
//   Added tabs for cleaning up window a bit.  Added timeQueryOptionsWidget.
//
//   Kathleen Biagas, Wed Jan 25 15:59:12 MST 2012
//   Added redoPickButton, timeOptsTabIndex.
//
//   Jonathan Byrd (Allinea Software), Sun Dec 18 2011
//   Added slot for 'Focus DDT on Pick' button clicks. Added pickRecords array
//
// ****************************************************************************

class GUI_API QvisPickWindow : public QvisPostableWindowObserver
{
    Q_OBJECT
public:
    QvisPickWindow( PickAttributes *subj,
                    const QString &caption = QString::null,
                    const QString &shortName = QString::null,
                    QvisNotepadArea *notepad = 0);
    virtual ~QvisPickWindow();

    virtual void CreateNode(DataNode *);
    virtual void SetFromNode(DataNode *, const int *borders);
    virtual void SubjectRemoved(Subject *TheRemovedSubject);
            void ConnectPlotList(PlotList *pl);

public slots:
    virtual void apply();
    virtual void makeDefault();
    virtual void reset();
    virtual void redoPick();
    virtual void restorePickAttributesAfterRepick();
signals:
    void initiateRedoPick(); 
    void initiateRestorePickAttributesAfterRepick();
protected:
    virtual void CreateWindowContents();

    void CreateDisplayOptionsTab();
    void CreateTimeOptionsTab();
    void CreateSpreadsheetOptionsTab();

    void UpdateWindow(bool doAll);
    void GetCurrentValues(int which_widget);
    void Apply(bool ignore = false);
private slots:
    void variableProcessText();
    void floatFormatProcessText();
    void displayIncElsToggled(bool val);
    void displayGlobalIdsToggled(bool val);
    void displayPickLetterToggled(bool val);
    void nodeIdToggled(bool val);
    void nodeDomLogToggled(bool val);
    void nodeBlockLogToggled(bool val);
    void nodePhysicalToggled(bool val);
    void zoneIdToggled(bool val);
    void zoneDomLogToggled(bool val);
    void zoneBlockLogToggled(bool val);
    void autoShowToggled(bool);
    void savePicksToggled(bool);
    void timeCurveToggled(bool);
    void spreadsheetToggled(bool);
    void conciseOutputToggled(bool);
    void showMeshNameToggled(bool);
    void showTimestepToggled(bool);
    void addPickVariable(const QString &);
    void savePickText();
    void preserveCoordActivated(int);
    void timeCurveTypeActivated(int);
    void clearPicks();
    void redoPickClicked();
    void redoPickWithSpreadsheetClicked();
    void optionsTabSelected(int);
    void focusPickInDDTClicked();
private:
    void UpdatePage(void);
    void ClearPages(void);
    void ResizeTabs(void);
    void UpdateAll(bool);
    void UpdateTimeOptions(void);

    bool                savePicks;
    bool                defaultSavePicks;
    bool                autoShow;
    bool                defaultAutoShow;
    int                 defaultNumTabs;
    int                 nextPage;
    QString             lastLetter;

    QTabWidget         *resultsTabWidget;
    QWidget            *pages[MAX_PICK_TABS];
    QTextEdit          *infoLists[MAX_PICK_TABS];
    PickRecord         *pickRecords[MAX_PICK_TABS];

    QCheckBox          *autoShowCheckBox;
    QCheckBox          *savePicksCheckBox;
    QSpinBox           *userMaxPickTabs;

    QvisVariableButton *varsButton;
    QLineEdit          *varsLineEdit;
    
    QLineEdit          *floatFormatLineEdit;

    QTabWidget         *optionsTabWidget;

    // Display Options Tab widgets
    QWidget            *pageDisplay;
    QCheckBox          *conciseOutputCheckBox;
    QCheckBox          *showMeshNameCheckBox;
    QCheckBox          *showTimestepCheckBox;

    QCheckBox          *displayIncEls;
    QCheckBox          *displayGlobalIds;
    QCheckBox          *displayPickLetter;

    QCheckBox          *nodeId;
    QCheckBox          *nodePhysical;
    QCheckBox          *nodeDomLog;
    QCheckBox          *nodeBlockLog;
    QCheckBox          *zoneId;
    QCheckBox          *zoneDomLog;
    QCheckBox          *zoneBlockLog;

    // Time Pick Tab widgets
    QWidget            *pageTime;
    QComboBox          *preserveCoord;
    QComboBox          *timeCurveType;
    QvisTimeQueryOptionsWidget *timeOpts;
    QPushButton        *redoPickButton;

    // Spreadsheet Tab widgets
    QWidget            *pageSpreadsheet;
    QCheckBox          *spreadsheetCheckBox;


    PickAttributes     *pickAtts;
    PlotList           *plotList;

    int                 saveCount;
    int                 activeOptionsTab;
    int                 timeOptsTabIndex;

    // Saving attributes for restoring them after re-doing a pick
    bool                createSpreadsheetSave;
    bool                displayPickLetterSave;
    bool                reusePickLetterSave;
};
#endif
