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

#include "QvisCracksClipperWindow.h"

#include <CracksClipperAttributes.h>
#include <ViewerProxy.h>

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QvisVariableButton.h>

#include <stdio.h>
#include <string>

using std::string;

// ****************************************************************************
// Method: QvisCracksClipperWindow::QvisCracksClipperWindow
//
// Purpose: 
//   Constructor
//
// Programmer: xml2window
// Creation:   Mon Aug 22 09:10:02 PDT 2005
//
// Modifications:
//   
// ****************************************************************************

QvisCracksClipperWindow::QvisCracksClipperWindow(const int type,
                         CracksClipperAttributes *subj,
                         const QString &caption,
                         const QString &shortName,
                         QvisNotepadArea *notepad)
    : QvisOperatorWindow(type,subj, caption, shortName, notepad)
{
    atts = subj;
}


// ****************************************************************************
// Method: QvisCracksClipperWindow::~QvisCracksClipperWindow
//
// Purpose: 
//   Destructor
//
// Programmer: xml2window
// Creation:   Mon Aug 22 09:10:02 PDT 2005
//
// Modifications:
//   
// ****************************************************************************

QvisCracksClipperWindow::~QvisCracksClipperWindow()
{
}


// ****************************************************************************
//  Method: QvisCracksClipperWindow::CreateWindowContents
//
//  Purpose: 
//    Creates the widgets for the window.
//
//  Programmer: xml2window
//  Creation:   Mon Aug 22 09:10:02 PDT 2005
//
//  Modifications:
//    Cyrus Harrison, Tue Aug 19 08:24:17 PDT 2008
//    Qt4 Port.
//
//    Kathleen Bonnell, Wed Sep 29 08:59:10 PDT 2010
//    Remove 'Density variable' and 'calculate density' toggle.
//
// ****************************************************************************

void
QvisCracksClipperWindow::CreateWindowContents()
{
    QGridLayout *mainLayout = new QGridLayout();
    topLayout->addLayout(mainLayout);

    // Show Crack 1
    showCrack1 = new QCheckBox(tr("Show Crack 1"), central);
    connect(showCrack1, SIGNAL(toggled(bool)),
        this, SLOT(showCrack1Changed(bool)));
    mainLayout->addWidget(showCrack1, 0, 0);

    // Crack 1 Variable
    mainLayout->addWidget(new QLabel(tr("Crack 1 Variable"), central), 1, 0);
    crack1Var = new QvisVariableButton(true, true, true,
                                       QvisVariableButton::Vectors, central);
    connect(crack1Var, SIGNAL(activated(const QString &)),
        this, SLOT(crack1VarChanged(const QString &)));
    mainLayout->addWidget(crack1Var, 1, 1, 1, 3);

    // Show Crack 2
    showCrack2 = new QCheckBox(tr("Show Crack 2"), central);
    connect(showCrack2, SIGNAL(toggled(bool)),
        this, SLOT(showCrack2Changed(bool)));
    mainLayout->addWidget(showCrack2, 2, 0);

    // Crack 2 Variable
    mainLayout->addWidget(new QLabel(tr("Crack 2 Variable"), central), 3, 0);
    crack2Var = new QvisVariableButton(true, true, true,
        QvisVariableButton::Vectors, central);
    connect(crack2Var, SIGNAL(activated(const QString &)),
        this, SLOT(crack2VarChanged(const QString &)));
    mainLayout->addWidget(crack2Var, 3, 1, 1, 3);

    // Show Crack 3
    showCrack3 = new QCheckBox(tr("Show Crack 3"), central);
    connect(showCrack3, SIGNAL(toggled(bool)),
        this, SLOT(showCrack3Changed(bool)));
    mainLayout->addWidget(showCrack3, 4, 0);

    // Crack 3 Variable
    mainLayout->addWidget(new QLabel(tr("Crack 3 Variable"), central), 5, 0);
    crack3Var = new QvisVariableButton(true, true, true,
        QvisVariableButton::Vectors, central);
    connect(crack3Var, SIGNAL(activated(const QString &)),
        this, SLOT(crack3VarChanged(const QString &)));
    mainLayout->addWidget(crack3Var, 5, 1, 1, 3);

    // Strain Variable
    mainLayout->addWidget(new QLabel(tr("Strain Variable"), central), 6, 0);
    strainVar = new QvisVariableButton(true, true, true,
                              QvisVariableButton::SymmetricTensors, central);
    connect(strainVar, SIGNAL(activated(const QString &)),
        this, SLOT(strainVarChanged(const QString &)));
    mainLayout->addWidget(strainVar, 6, 1, 1, 3);

    // Input Mass Variable 
    inMassVarLabel = new QLabel(tr("Input Mass Variable"), central);
    mainLayout->addWidget(inMassVarLabel, 8, 0);
    inMassVar = new QvisVariableButton(true, true, true,
                                       QvisVariableButton::Scalars, central);
    connect(inMassVar, SIGNAL(activated(const QString &)),
        this, SLOT(inMassVarChanged(const QString &)));
    mainLayout->addWidget(inMassVar, 8, 1, 1, 3);

}


// ****************************************************************************
//  Method: QvisCracksClipperWindow::UpdateWindow
//
//  Purpose: 
//    Updates the widgets in the window when the subject changes.
//
//  Programmer: xml2window
//  Creation:   Mon Aug 22 09:10:02 PDT 2005
//
//  Modifications:
//    Kathleen Bonnell, Mon May  7 15:48:42 PDT 2007
//    Added calculateDensity, inMarVar, outDenVar.
//   
//    Kathleen Bonnell, Tue Jul 1 15:10:55 PDT 2008 
//    Removed unreferenced variable.
//   
//    Kathleen Bonnell, Wed Sep 29 08:59:10 PDT 2010
//    Remove 'Density variable' and 'calculate density' toggle.
//
// ****************************************************************************

void
QvisCracksClipperWindow::UpdateWindow(bool doAll)
{
    QString temp;

    for(int i = 0; i < atts->NumAttributes(); ++i)
    {
        if(!doAll)
        {
            if(!atts->IsSelected(i))
            {
                continue;
            }
        }
        switch(i)
        {
          case CracksClipperAttributes::ID_crack1Var:
            temp = atts->GetCrack1Var().c_str();
            crack1Var->setText(temp);
            break;
          case CracksClipperAttributes::ID_crack2Var: //crack2Var
            temp = atts->GetCrack2Var().c_str();
            crack2Var->setText(temp);
            break;
          case CracksClipperAttributes::ID_crack3Var: //crack3Var
            temp = atts->GetCrack3Var().c_str();
            crack3Var->setText(temp);
            break;
          case CracksClipperAttributes::ID_strainVar: //strainVar
            temp = atts->GetStrainVar().c_str();
            strainVar->setText(temp);
            break;
          case CracksClipperAttributes::ID_showCrack1: //showCrack1
            showCrack1->setChecked(atts->GetShowCrack1());
            break;
          case CracksClipperAttributes::ID_showCrack2: //showCrack2
            showCrack2->setChecked(atts->GetShowCrack2());
            break;
          case CracksClipperAttributes::ID_showCrack3: //showCrack3
            showCrack3->setChecked(atts->GetShowCrack3());
            break;
          case CracksClipperAttributes::ID_inMassVar: //inMassVar
            temp = atts->GetInMassVar().c_str();
            inMassVar->setText(temp);
            break;
        }
    }
}


// ****************************************************************************
// Method: QvisCracksClipperWindow::GetCurrentValues
//
// Purpose: 
//   Gets values from certain widgets and stores them in the subject.
//
// Programmer: xml2window
// Creation:   Mon Aug 22 09:10:02 PDT 2005
//
// Modifications:
//   Kathleen Bonnell, Tue Jul 1 15:10:55 PDT 2008 
//   Removed unreferenced variables.
//   
// ****************************************************************************

void
QvisCracksClipperWindow::GetCurrentValues(int which_widget)
{
    bool doAll = (which_widget == -1);

    // Do crack1Var
    if(which_widget == 0 || doAll)
    {
        // Nothing for crack1Var
    }

    // Do crack2Var
    if(which_widget == 1 || doAll)
    {
        // Nothing for crack2Var
    }

    // Do crack3Var
    if(which_widget == 2 || doAll)
    {
        // Nothing for crack3Var
    }

    // Do strainVar
    if(which_widget == 3 || doAll)
    {
        // Nothing for strainVar
    }

    // Do showCrack1
    if(which_widget == 4 || doAll)
    {
        // Nothing for showCrack1
    }

    // Do showCrack2
    if(which_widget == 5 || doAll)
    {
        // Nothing for showCrack2
    }

    // Do showCrack3
    if(which_widget == 6 || doAll)
    {
        // Nothing for showCrack3
    }
}


//
// Qt Slot functions
//


void
QvisCracksClipperWindow::crack1VarChanged(const QString &var)
{
    atts->SetCrack1Var(var.toStdString());
    SetUpdate(false);
    Apply();
}


void
QvisCracksClipperWindow::crack2VarChanged(const QString &var)
{
    atts->SetCrack2Var(var.toStdString());
    SetUpdate(false);
    Apply();
}


void
QvisCracksClipperWindow::crack3VarChanged(const QString &var)
{
    atts->SetCrack3Var(var.toStdString());
    SetUpdate(false);
    Apply();
}


void
QvisCracksClipperWindow::strainVarChanged(const QString &var)
{
    atts->SetStrainVar(var.toStdString());
    SetUpdate(false);
    Apply();
}


void
QvisCracksClipperWindow::showCrack1Changed(bool val)
{
    atts->SetShowCrack1(val);
    Apply();
}


void
QvisCracksClipperWindow::showCrack2Changed(bool val)
{
    atts->SetShowCrack2(val);
    Apply();
}


void
QvisCracksClipperWindow::showCrack3Changed(bool val)
{
    atts->SetShowCrack3(val);
    Apply();
}


void
QvisCracksClipperWindow::inMassVarChanged(const QString &var)
{
    atts->SetInMassVar(var.toStdString());
    SetUpdate(false);
    Apply();
}

