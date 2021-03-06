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

#include <GlobalLineoutAttributes.h>
#include <DataNode.h>

//
// Enum conversion methods for GlobalLineoutAttributes::CurveOptions
//

static const char *CurveOptions_strings[] = {
"UpdateCurve", "CreateCurve"};

std::string
GlobalLineoutAttributes::CurveOptions_ToString(GlobalLineoutAttributes::CurveOptions t)
{
    int index = int(t);
    if(index < 0 || index >= 2) index = 0;
    return CurveOptions_strings[index];
}

std::string
GlobalLineoutAttributes::CurveOptions_ToString(int t)
{
    int index = (t < 0 || t >= 2) ? 0 : t;
    return CurveOptions_strings[index];
}

bool
GlobalLineoutAttributes::CurveOptions_FromString(const std::string &s, GlobalLineoutAttributes::CurveOptions &val)
{
    val = GlobalLineoutAttributes::UpdateCurve;
    for(int i = 0; i < 2; ++i)
    {
        if(s == CurveOptions_strings[i])
        {
            val = (CurveOptions)i;
            return true;
        }
    }
    return false;
}

//
// Enum conversion methods for GlobalLineoutAttributes::ColorOptions
//

static const char *ColorOptions_strings[] = {
"RepeatColor", "CreateColor"};

std::string
GlobalLineoutAttributes::ColorOptions_ToString(GlobalLineoutAttributes::ColorOptions t)
{
    int index = int(t);
    if(index < 0 || index >= 2) index = 0;
    return ColorOptions_strings[index];
}

std::string
GlobalLineoutAttributes::ColorOptions_ToString(int t)
{
    int index = (t < 0 || t >= 2) ? 0 : t;
    return ColorOptions_strings[index];
}

bool
GlobalLineoutAttributes::ColorOptions_FromString(const std::string &s, GlobalLineoutAttributes::ColorOptions &val)
{
    val = GlobalLineoutAttributes::RepeatColor;
    for(int i = 0; i < 2; ++i)
    {
        if(s == ColorOptions_strings[i])
        {
            val = (ColorOptions)i;
            return true;
        }
    }
    return false;
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::GlobalLineoutAttributes
//
// Purpose: 
//   Init utility for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void GlobalLineoutAttributes::Init()
{
    Dynamic = false;
    createWindow = true;
    windowId = 2;
    samplingOn = false;
    numSamples = 50;
    createReflineLabels = false;
    curveOption = UpdateCurve;
    colorOption = RepeatColor;
    freezeInTime = false;

    GlobalLineoutAttributes::SelectAll();
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::GlobalLineoutAttributes
//
// Purpose: 
//   Copy utility for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void GlobalLineoutAttributes::Copy(const GlobalLineoutAttributes &obj)
{
    Dynamic = obj.Dynamic;
    createWindow = obj.createWindow;
    windowId = obj.windowId;
    samplingOn = obj.samplingOn;
    numSamples = obj.numSamples;
    createReflineLabels = obj.createReflineLabels;
    curveOption = obj.curveOption;
    colorOption = obj.colorOption;
    freezeInTime = obj.freezeInTime;

    GlobalLineoutAttributes::SelectAll();
}

// Type map format string
const char *GlobalLineoutAttributes::TypeMapFormatString = GLOBALLINEOUTATTRIBUTES_TMFS;
const AttributeGroup::private_tmfs_t GlobalLineoutAttributes::TmfsStruct = {GLOBALLINEOUTATTRIBUTES_TMFS};


// ****************************************************************************
// Method: GlobalLineoutAttributes::GlobalLineoutAttributes
//
// Purpose: 
//   Default constructor for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

GlobalLineoutAttributes::GlobalLineoutAttributes() : 
    AttributeSubject(GlobalLineoutAttributes::TypeMapFormatString)
{
    GlobalLineoutAttributes::Init();
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::GlobalLineoutAttributes
//
// Purpose: 
//   Constructor for the derived classes of GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

GlobalLineoutAttributes::GlobalLineoutAttributes(private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    GlobalLineoutAttributes::Init();
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::GlobalLineoutAttributes
//
// Purpose: 
//   Copy constructor for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

GlobalLineoutAttributes::GlobalLineoutAttributes(const GlobalLineoutAttributes &obj) : 
    AttributeSubject(GlobalLineoutAttributes::TypeMapFormatString)
{
    GlobalLineoutAttributes::Copy(obj);
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::GlobalLineoutAttributes
//
// Purpose: 
//   Copy constructor for derived classes of the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

GlobalLineoutAttributes::GlobalLineoutAttributes(const GlobalLineoutAttributes &obj, private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    GlobalLineoutAttributes::Copy(obj);
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::~GlobalLineoutAttributes
//
// Purpose: 
//   Destructor for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

GlobalLineoutAttributes::~GlobalLineoutAttributes()
{
    // nothing here
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::operator = 
//
// Purpose: 
//   Assignment operator for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

GlobalLineoutAttributes& 
GlobalLineoutAttributes::operator = (const GlobalLineoutAttributes &obj)
{
    if (this == &obj) return *this;

    GlobalLineoutAttributes::Copy(obj);

    return *this;
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::operator == 
//
// Purpose: 
//   Comparison operator == for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
GlobalLineoutAttributes::operator == (const GlobalLineoutAttributes &obj) const
{
    // Create the return value
    return ((Dynamic == obj.Dynamic) &&
            (createWindow == obj.createWindow) &&
            (windowId == obj.windowId) &&
            (samplingOn == obj.samplingOn) &&
            (numSamples == obj.numSamples) &&
            (createReflineLabels == obj.createReflineLabels) &&
            (curveOption == obj.curveOption) &&
            (colorOption == obj.colorOption) &&
            (freezeInTime == obj.freezeInTime));
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::operator != 
//
// Purpose: 
//   Comparison operator != for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
GlobalLineoutAttributes::operator != (const GlobalLineoutAttributes &obj) const
{
    return !(this->operator == (obj));
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::TypeName
//
// Purpose: 
//   Type name method for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

const std::string
GlobalLineoutAttributes::TypeName() const
{
    return "GlobalLineoutAttributes";
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::CopyAttributes
//
// Purpose: 
//   CopyAttributes method for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
GlobalLineoutAttributes::CopyAttributes(const AttributeGroup *atts)
{
    if(TypeName() != atts->TypeName())
        return false;

    // Call assignment operator.
    const GlobalLineoutAttributes *tmp = (const GlobalLineoutAttributes *)atts;
    *this = *tmp;

    return true;
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::CreateCompatible
//
// Purpose: 
//   CreateCompatible method for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeSubject *
GlobalLineoutAttributes::CreateCompatible(const std::string &tname) const
{
    AttributeSubject *retval = 0;
    if(TypeName() == tname)
        retval = new GlobalLineoutAttributes(*this);
    // Other cases could go here too. 

    return retval;
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::NewInstance
//
// Purpose: 
//   NewInstance method for the GlobalLineoutAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeSubject *
GlobalLineoutAttributes::NewInstance(bool copy) const
{
    AttributeSubject *retval = 0;
    if(copy)
        retval = new GlobalLineoutAttributes(*this);
    else
        retval = new GlobalLineoutAttributes;

    return retval;
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::SelectAll
//
// Purpose: 
//   Selects all attributes.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void
GlobalLineoutAttributes::SelectAll()
{
    Select(ID_Dynamic,             (void *)&Dynamic);
    Select(ID_createWindow,        (void *)&createWindow);
    Select(ID_windowId,            (void *)&windowId);
    Select(ID_samplingOn,          (void *)&samplingOn);
    Select(ID_numSamples,          (void *)&numSamples);
    Select(ID_createReflineLabels, (void *)&createReflineLabels);
    Select(ID_curveOption,         (void *)&curveOption);
    Select(ID_colorOption,         (void *)&colorOption);
    Select(ID_freezeInTime,        (void *)&freezeInTime);
}

///////////////////////////////////////////////////////////////////////////////
// Set property methods
///////////////////////////////////////////////////////////////////////////////

void
GlobalLineoutAttributes::SetDynamic(bool Dynamic_)
{
    Dynamic = Dynamic_;
    Select(ID_Dynamic, (void *)&Dynamic);
}

void
GlobalLineoutAttributes::SetCreateWindow(bool createWindow_)
{
    createWindow = createWindow_;
    Select(ID_createWindow, (void *)&createWindow);
}

void
GlobalLineoutAttributes::SetWindowId(int windowId_)
{
    windowId = windowId_;
    Select(ID_windowId, (void *)&windowId);
}

void
GlobalLineoutAttributes::SetSamplingOn(bool samplingOn_)
{
    samplingOn = samplingOn_;
    Select(ID_samplingOn, (void *)&samplingOn);
}

void
GlobalLineoutAttributes::SetNumSamples(int numSamples_)
{
    numSamples = numSamples_;
    Select(ID_numSamples, (void *)&numSamples);
}

void
GlobalLineoutAttributes::SetCreateReflineLabels(bool createReflineLabels_)
{
    createReflineLabels = createReflineLabels_;
    Select(ID_createReflineLabels, (void *)&createReflineLabels);
}

void
GlobalLineoutAttributes::SetCurveOption(GlobalLineoutAttributes::CurveOptions curveOption_)
{
    curveOption = curveOption_;
    Select(ID_curveOption, (void *)&curveOption);
}

void
GlobalLineoutAttributes::SetColorOption(GlobalLineoutAttributes::ColorOptions colorOption_)
{
    colorOption = colorOption_;
    Select(ID_colorOption, (void *)&colorOption);
}

void
GlobalLineoutAttributes::SetFreezeInTime(bool freezeInTime_)
{
    freezeInTime = freezeInTime_;
    Select(ID_freezeInTime, (void *)&freezeInTime);
}

///////////////////////////////////////////////////////////////////////////////
// Get property methods
///////////////////////////////////////////////////////////////////////////////

bool
GlobalLineoutAttributes::GetDynamic() const
{
    return Dynamic;
}

bool
GlobalLineoutAttributes::GetCreateWindow() const
{
    return createWindow;
}

int
GlobalLineoutAttributes::GetWindowId() const
{
    return windowId;
}

bool
GlobalLineoutAttributes::GetSamplingOn() const
{
    return samplingOn;
}

int
GlobalLineoutAttributes::GetNumSamples() const
{
    return numSamples;
}

bool
GlobalLineoutAttributes::GetCreateReflineLabels() const
{
    return createReflineLabels;
}

GlobalLineoutAttributes::CurveOptions
GlobalLineoutAttributes::GetCurveOption() const
{
    return CurveOptions(curveOption);
}

GlobalLineoutAttributes::ColorOptions
GlobalLineoutAttributes::GetColorOption() const
{
    return ColorOptions(colorOption);
}

bool
GlobalLineoutAttributes::GetFreezeInTime() const
{
    return freezeInTime;
}

///////////////////////////////////////////////////////////////////////////////
// Keyframing methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: GlobalLineoutAttributes::GetFieldName
//
// Purpose: 
//   This method returns the name of a field given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

std::string
GlobalLineoutAttributes::GetFieldName(int index) const
{
    switch (index)
    {
    case ID_Dynamic:             return "Dynamic";
    case ID_createWindow:        return "createWindow";
    case ID_windowId:            return "windowId";
    case ID_samplingOn:          return "samplingOn";
    case ID_numSamples:          return "numSamples";
    case ID_createReflineLabels: return "createReflineLabels";
    case ID_curveOption:         return "curveOption";
    case ID_colorOption:         return "colorOption";
    case ID_freezeInTime:        return "freezeInTime";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::GetFieldType
//
// Purpose: 
//   This method returns the type of a field given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeGroup::FieldType
GlobalLineoutAttributes::GetFieldType(int index) const
{
    switch (index)
    {
    case ID_Dynamic:             return FieldType_bool;
    case ID_createWindow:        return FieldType_bool;
    case ID_windowId:            return FieldType_int;
    case ID_samplingOn:          return FieldType_bool;
    case ID_numSamples:          return FieldType_int;
    case ID_createReflineLabels: return FieldType_bool;
    case ID_curveOption:         return FieldType_enum;
    case ID_colorOption:         return FieldType_enum;
    case ID_freezeInTime:        return FieldType_bool;
    default:  return FieldType_unknown;
    }
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::GetFieldTypeName
//
// Purpose: 
//   This method returns the name of a field type given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

std::string
GlobalLineoutAttributes::GetFieldTypeName(int index) const
{
    switch (index)
    {
    case ID_Dynamic:             return "bool";
    case ID_createWindow:        return "bool";
    case ID_windowId:            return "int";
    case ID_samplingOn:          return "bool";
    case ID_numSamples:          return "int";
    case ID_createReflineLabels: return "bool";
    case ID_curveOption:         return "enum";
    case ID_colorOption:         return "enum";
    case ID_freezeInTime:        return "bool";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: GlobalLineoutAttributes::FieldsEqual
//
// Purpose: 
//   This method compares two fields and return true if they are equal.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
GlobalLineoutAttributes::FieldsEqual(int index_, const AttributeGroup *rhs) const
{
    const GlobalLineoutAttributes &obj = *((const GlobalLineoutAttributes*)rhs);
    bool retval = false;
    switch (index_)
    {
    case ID_Dynamic:
        {  // new scope
        retval = (Dynamic == obj.Dynamic);
        }
        break;
    case ID_createWindow:
        {  // new scope
        retval = (createWindow == obj.createWindow);
        }
        break;
    case ID_windowId:
        {  // new scope
        retval = (windowId == obj.windowId);
        }
        break;
    case ID_samplingOn:
        {  // new scope
        retval = (samplingOn == obj.samplingOn);
        }
        break;
    case ID_numSamples:
        {  // new scope
        retval = (numSamples == obj.numSamples);
        }
        break;
    case ID_createReflineLabels:
        {  // new scope
        retval = (createReflineLabels == obj.createReflineLabels);
        }
        break;
    case ID_curveOption:
        {  // new scope
        retval = (curveOption == obj.curveOption);
        }
        break;
    case ID_colorOption:
        {  // new scope
        retval = (colorOption == obj.colorOption);
        }
        break;
    case ID_freezeInTime:
        {  // new scope
        retval = (freezeInTime == obj.freezeInTime);
        }
        break;
    default: retval = false;
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// User-defined methods.
///////////////////////////////////////////////////////////////////////////////

