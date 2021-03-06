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

#ifndef QVIS_QUERY_WINDOW_H
#define QVIS_QUERY_WINDOW_H
#include <gui_exports.h>
#include <QvisPostableWindowSimpleObserver.h>
#include <vectortypes.h>

// Forward declarations
class QueryAttributes;
class PickAttributes;
class PlotList;
class QButtonGroup;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QTextEdit;
class QPushButton;
class QueryList;
class QSplitter;
class QTabWidget;
class QvisVariableButton;
class QvisPythonFilterEditor;

// widgets
class QvisHohlraumFluxQueryWidget;
class QvisLineoutQueryWidget;
class QvisPickQueryWidget;
class QvisTimeQueryOptionsWidget;
class QvisXRayImageQueryWidget;

// ****************************************************************************
// Class: QvisQueryWindow
//
// Purpose:
//   This class displays the list of available queries and
//
// Notes:      
//
// Programmer: Brad Whitlock
// Creation:   Fri Sep 6 15:41:05 PST 2002
//
// Modifications:
//   Kathleen Bonnell, Mon Sep 30 14:38:33 PDT 2002 
//   Add queryAtts. 
//
//   Brad Whitlock, Thu Dec 26 17:46:18 PST 2002
//   I changed the interface to GetVars.
//
//   Brad Whitlock, Fri May 9 17:14:54 PST 2003
//   I added a button to clear the window.
//
//   Kathleen Bonnell, Tue Dec  2 08:39:46 PST 2003 
//   Added a button to specify whether the query should apply to the current
//   plot or the database. 
//
//   Kathleen Bonnell, Thu Apr  1 18:42:52 PST 2004
//   Added button in support of creating queries over time.
//
//   Kathleen Bonnell, Sat Sep  4 11:49:58 PDT 2004 
//   Added displayMode combo box, Removed unneeded argument from 
//   UpdateQueryList, changed arg in UpdateArgumentPanel from int to QString.
//   All to allow query lists to be sorted and grouped functionally.
//
//   Kathleen Bonnell, Wed Sep  8 10:06:16 PDT 2004 
//   Removed coordLabel, removed 'rep' arg from GetPoint.
//
//   Kathleen Bonnell, Wed Dec 15 17:16:17 PST 2004 
//   Added 'useGlobal' checkbox.
//
//   Kathleen Bonnell, Tue Jan 11 16:16:48 PST 2005 
//   Added slot 'useGlobalToggled'.
//
//   Hank Childs, Mon Jul 10 17:23:24 PDT 2006
//   Added GetFloatingPointNumber.
//
//   Ellen Tarwater, Tues May 15 2006
//   Added saveResultText and saveCount
//
//   Cyrus Harrison, Tue Sep 18 08:09:53 PDT 2007
//   Added floatFormatText
//
//   Brad Whitlock, Wed Apr  9 11:46:13 PDT 2008
//   QString for caption, shortName.
//
//   Kathleen Bonnell, Tue Jun 24 11:18:13 PDT 2008
//   Added QvisVariableButton for queries that need variables.
//
//   Cyrus Harrison, Wed Feb 17 12:31:16 PST 2010
//   Added new widgets for python query intergration.
//
//   Eric Brugger, Fri Jul  2 15:52:13 PDT 2010
//   Increased the number of text fields to support the x ray image query.
//
//   Kathleen Bonnell, Tue Mar  1 11:06:35 PST 2011
//   Add widget for plotOpts, which controls the type of plot for Time Picks.
//
//   Kathleen Biagas, Fri Jun 10 08:58:12 PDT 2011
//   Added QvisPickQueryWidget, QvisHohlraumFluxQueryWidget, 
//   QvisXRayImageQueryWidget, QvisLineoutWidget, QvisTimeQueryOptionsWidget, 
//   which provide controls for specific queries.
//
//   Kathleen Biagas, Wed Jan  9 13:45:16 PST 2013
//   Store currentFloatFormat so that the floatFormat will only be sent to
//   viewer when it has changed. (Reduces log messages).
//
// ****************************************************************************

class GUI_API QvisQueryWindow : public QvisPostableWindowSimpleObserver
{
    Q_OBJECT
public:
    QvisQueryWindow(const QString &caption = QString::null,
                    const QString &shortName = QString::null,
                    QvisNotepadArea *n = 0);
    virtual ~QvisQueryWindow();
    virtual void CreateWindowContents();
    virtual void SubjectRemoved(Subject *TheRemovedSubject);

    void ConnectQueryList(QueryList *q);
    void ConnectQueryAttributes(QueryAttributes *p);
    void ConnectPickAttributes(PickAttributes *p);
    void ConnectPlotList(PlotList *pl);
protected:
    virtual void UpdateWindow(bool doAll);
    virtual void CreateEntireWindow();
private:
    void UpdateQueryButton();
    void UpdateTimeQueryOptions();
    void UpdateQueryList();
    void UpdateResults(bool doAll);
    void UpdateArgumentPanel(const QString &);

    void Apply(bool ignore = false);
    bool GetNumber(int index, int *num);
    bool GetFloatingPointNumber(int index, double *num);
    bool GetVars(stringVector &vars);
private slots:
    void apply();
    void handleText();
    void selectQuery();
    void clearResultText();
    void displayModeChanged(int);
    void useGlobalToggled(bool);
    void dumpStepsToggled(bool);
    void saveResultText();
    void addVariable(const QString &);
    void addPyVariable(const QString &);
    void pyTemplateSelected(const QString &);

private:
    void                CreateStandardQueryWidget();
    void                CreatePythonQueryWidget();
    void                CreateResultsWidget();

    void                ExecuteStandardQuery();
    void                ExecutePythonQuery();

    QueryList           *queries;
    QueryAttributes     *queryAtts;
    PickAttributes      *pickAtts;
    PlotList            *plotList;

    QSplitter           *splitter;

    QTabWidget          *queryTabs;

    QWidget             *stdQueryWidget;
    QComboBox           *displayMode;
    QListWidget         *queryList;
    QPushButton         *queryButton;

    QGroupBox           *argPanel;
    QLabel              *labels[6];
    QLineEdit           *textFields[6];
    QCheckBox           *useGlobal, *dumpSteps;
    QvisVariableButton  *varsButton;
    QLineEdit           *varsLineEdit;

    QWidget             *resultsWidget;
    QLineEdit           *floatFormatText;
    QTextEdit           *resultText;

    QButtonGroup        *dataOpts;

    int                 saveCount;
    int                 queryVarTypes;
    std::string         currentFloatFormat;

    QWidget                *pyQueryWidget;
    QLabel                 *pyFilterEditLabel;
    QvisPythonFilterEditor *pyFilterEdit;
    QvisVariableButton     *pyVarsButton;
    QLineEdit              *pyVarsLineEdit;
    QPushButton            *pyQueryButton;

    QvisHohlraumFluxQueryWidget   *hohlraumFluxQueryWidget;
    QvisLineoutQueryWidget        *lineoutQueryWidget;
    QvisPickQueryWidget           *pickQueryWidget;
    QvisTimeQueryOptionsWidget    *timeQueryOptions;
    QvisXRayImageQueryWidget      *xRayImageQueryWidget;
};

#endif

