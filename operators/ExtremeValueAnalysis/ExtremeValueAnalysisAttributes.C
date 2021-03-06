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

#include <ExtremeValueAnalysisAttributes.h>
#include <DataNode.h>

//
// Enum conversion methods for ExtremeValueAnalysisAttributes::AggregationType
//

static const char *AggregationType_strings[] = {
"ANNUAL", "MONTHLY", "SEASONAL"
};

std::string
ExtremeValueAnalysisAttributes::AggregationType_ToString(ExtremeValueAnalysisAttributes::AggregationType t)
{
    int index = int(t);
    if(index < 0 || index >= 3) index = 0;
    return AggregationType_strings[index];
}

std::string
ExtremeValueAnalysisAttributes::AggregationType_ToString(int t)
{
    int index = (t < 0 || t >= 3) ? 0 : t;
    return AggregationType_strings[index];
}

bool
ExtremeValueAnalysisAttributes::AggregationType_FromString(const std::string &s, ExtremeValueAnalysisAttributes::AggregationType &val)
{
    val = ExtremeValueAnalysisAttributes::ANNUAL;
    for(int i = 0; i < 3; ++i)
    {
        if(s == AggregationType_strings[i])
        {
            val = (AggregationType)i;
            return true;
        }
    }
    return false;
}

//
// Enum conversion methods for ExtremeValueAnalysisAttributes::MonthType
//

static const char *MonthType_strings[] = {
"January", "February", "March", 
"April", "May", "June", 
"July", "August", "September", 
"October", "November", "December"
};

std::string
ExtremeValueAnalysisAttributes::MonthType_ToString(ExtremeValueAnalysisAttributes::MonthType t)
{
    int index = int(t);
    if(index < 0 || index >= 12) index = 0;
    return MonthType_strings[index];
}

std::string
ExtremeValueAnalysisAttributes::MonthType_ToString(int t)
{
    int index = (t < 0 || t >= 12) ? 0 : t;
    return MonthType_strings[index];
}

bool
ExtremeValueAnalysisAttributes::MonthType_FromString(const std::string &s, ExtremeValueAnalysisAttributes::MonthType &val)
{
    val = ExtremeValueAnalysisAttributes::January;
    for(int i = 0; i < 12; ++i)
    {
        if(s == MonthType_strings[i])
        {
            val = (MonthType)i;
            return true;
        }
    }
    return false;
}

//
// Enum conversion methods for ExtremeValueAnalysisAttributes::SeasonType
//

static const char *SeasonType_strings[] = {
"WINTER", "SPRING", "SUMMER", 
"FALL"};

std::string
ExtremeValueAnalysisAttributes::SeasonType_ToString(ExtremeValueAnalysisAttributes::SeasonType t)
{
    int index = int(t);
    if(index < 0 || index >= 4) index = 0;
    return SeasonType_strings[index];
}

std::string
ExtremeValueAnalysisAttributes::SeasonType_ToString(int t)
{
    int index = (t < 0 || t >= 4) ? 0 : t;
    return SeasonType_strings[index];
}

bool
ExtremeValueAnalysisAttributes::SeasonType_FromString(const std::string &s, ExtremeValueAnalysisAttributes::SeasonType &val)
{
    val = ExtremeValueAnalysisAttributes::WINTER;
    for(int i = 0; i < 4; ++i)
    {
        if(s == SeasonType_strings[i])
        {
            val = (SeasonType)i;
            return true;
        }
    }
    return false;
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::ExtremeValueAnalysisAttributes
//
// Purpose: 
//   Init utility for the ExtremeValueAnalysisAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void ExtremeValueAnalysisAttributes::Init()
{
    aggregation = MONTHLY;
    displayMonth = January;
    displaySeason = WINTER;
    dumpData = false;
    dataScaling = 1;

    ExtremeValueAnalysisAttributes::SelectAll();
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::ExtremeValueAnalysisAttributes
//
// Purpose: 
//   Copy utility for the ExtremeValueAnalysisAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void ExtremeValueAnalysisAttributes::Copy(const ExtremeValueAnalysisAttributes &obj)
{
    aggregation = obj.aggregation;
    displayMonth = obj.displayMonth;
    displaySeason = obj.displaySeason;
    dumpData = obj.dumpData;
    dataScaling = obj.dataScaling;

    ExtremeValueAnalysisAttributes::SelectAll();
}

// Type map format string
const char *ExtremeValueAnalysisAttributes::TypeMapFormatString = EXTREMEVALUEANALYSISATTRIBUTES_TMFS;
const AttributeGroup::private_tmfs_t ExtremeValueAnalysisAttributes::TmfsStruct = {EXTREMEVALUEANALYSISATTRIBUTES_TMFS};


// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::ExtremeValueAnalysisAttributes
//
// Purpose: 
//   Default constructor for the ExtremeValueAnalysisAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExtremeValueAnalysisAttributes::ExtremeValueAnalysisAttributes() : 
    AttributeSubject(ExtremeValueAnalysisAttributes::TypeMapFormatString)
{
    ExtremeValueAnalysisAttributes::Init();
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::ExtremeValueAnalysisAttributes
//
// Purpose: 
//   Constructor for the derived classes of ExtremeValueAnalysisAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExtremeValueAnalysisAttributes::ExtremeValueAnalysisAttributes(private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    ExtremeValueAnalysisAttributes::Init();
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::ExtremeValueAnalysisAttributes
//
// Purpose: 
//   Copy constructor for the ExtremeValueAnalysisAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExtremeValueAnalysisAttributes::ExtremeValueAnalysisAttributes(const ExtremeValueAnalysisAttributes &obj) : 
    AttributeSubject(ExtremeValueAnalysisAttributes::TypeMapFormatString)
{
    ExtremeValueAnalysisAttributes::Copy(obj);
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::ExtremeValueAnalysisAttributes
//
// Purpose: 
//   Copy constructor for derived classes of the ExtremeValueAnalysisAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExtremeValueAnalysisAttributes::ExtremeValueAnalysisAttributes(const ExtremeValueAnalysisAttributes &obj, private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    ExtremeValueAnalysisAttributes::Copy(obj);
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::~ExtremeValueAnalysisAttributes
//
// Purpose: 
//   Destructor for the ExtremeValueAnalysisAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExtremeValueAnalysisAttributes::~ExtremeValueAnalysisAttributes()
{
    // nothing here
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::operator = 
//
// Purpose: 
//   Assignment operator for the ExtremeValueAnalysisAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExtremeValueAnalysisAttributes& 
ExtremeValueAnalysisAttributes::operator = (const ExtremeValueAnalysisAttributes &obj)
{
    if (this == &obj) return *this;

    ExtremeValueAnalysisAttributes::Copy(obj);

    return *this;
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::operator == 
//
// Purpose: 
//   Comparison operator == for the ExtremeValueAnalysisAttributes class.
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
ExtremeValueAnalysisAttributes::operator == (const ExtremeValueAnalysisAttributes &obj) const
{
    // Create the return value
    return ((aggregation == obj.aggregation) &&
            (displayMonth == obj.displayMonth) &&
            (displaySeason == obj.displaySeason) &&
            (dumpData == obj.dumpData) &&
            (dataScaling == obj.dataScaling));
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::operator != 
//
// Purpose: 
//   Comparison operator != for the ExtremeValueAnalysisAttributes class.
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
ExtremeValueAnalysisAttributes::operator != (const ExtremeValueAnalysisAttributes &obj) const
{
    return !(this->operator == (obj));
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::TypeName
//
// Purpose: 
//   Type name method for the ExtremeValueAnalysisAttributes class.
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
ExtremeValueAnalysisAttributes::TypeName() const
{
    return "ExtremeValueAnalysisAttributes";
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::CopyAttributes
//
// Purpose: 
//   CopyAttributes method for the ExtremeValueAnalysisAttributes class.
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
ExtremeValueAnalysisAttributes::CopyAttributes(const AttributeGroup *atts)
{
    if(TypeName() != atts->TypeName())
        return false;

    // Call assignment operator.
    const ExtremeValueAnalysisAttributes *tmp = (const ExtremeValueAnalysisAttributes *)atts;
    *this = *tmp;

    return true;
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::CreateCompatible
//
// Purpose: 
//   CreateCompatible method for the ExtremeValueAnalysisAttributes class.
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
ExtremeValueAnalysisAttributes::CreateCompatible(const std::string &tname) const
{
    AttributeSubject *retval = 0;
    if(TypeName() == tname)
        retval = new ExtremeValueAnalysisAttributes(*this);
    // Other cases could go here too. 

    return retval;
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::NewInstance
//
// Purpose: 
//   NewInstance method for the ExtremeValueAnalysisAttributes class.
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
ExtremeValueAnalysisAttributes::NewInstance(bool copy) const
{
    AttributeSubject *retval = 0;
    if(copy)
        retval = new ExtremeValueAnalysisAttributes(*this);
    else
        retval = new ExtremeValueAnalysisAttributes;

    return retval;
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::SelectAll
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
ExtremeValueAnalysisAttributes::SelectAll()
{
    Select(ID_aggregation,   (void *)&aggregation);
    Select(ID_displayMonth,  (void *)&displayMonth);
    Select(ID_displaySeason, (void *)&displaySeason);
    Select(ID_dumpData,      (void *)&dumpData);
    Select(ID_dataScaling,   (void *)&dataScaling);
}

///////////////////////////////////////////////////////////////////////////////
// Persistence methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::CreateNode
//
// Purpose: 
//   This method creates a DataNode representation of the object so it can be saved to a config file.
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
ExtremeValueAnalysisAttributes::CreateNode(DataNode *parentNode, bool completeSave, bool forceAdd)
{
    if(parentNode == 0)
        return false;

    ExtremeValueAnalysisAttributes defaultObject;
    bool addToParent = false;
    // Create a node for ExtremeValueAnalysisAttributes.
    DataNode *node = new DataNode("ExtremeValueAnalysisAttributes");

    if(completeSave || !FieldsEqual(ID_aggregation, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("aggregation", AggregationType_ToString(aggregation)));
    }

    if(completeSave || !FieldsEqual(ID_displayMonth, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("displayMonth", MonthType_ToString(displayMonth)));
    }

    if(completeSave || !FieldsEqual(ID_displaySeason, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("displaySeason", SeasonType_ToString(displaySeason)));
    }

    if(completeSave || !FieldsEqual(ID_dumpData, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("dumpData", dumpData));
    }

    if(completeSave || !FieldsEqual(ID_dataScaling, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("dataScaling", dataScaling));
    }


    // Add the node to the parent node.
    if(addToParent || forceAdd)
        parentNode->AddNode(node);
    else
        delete node;

    return (addToParent || forceAdd);
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::SetFromNode
//
// Purpose: 
//   This method sets attributes in this object from values in a DataNode representation of the object.
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
ExtremeValueAnalysisAttributes::SetFromNode(DataNode *parentNode)
{
    if(parentNode == 0)
        return;

    DataNode *searchNode = parentNode->GetNode("ExtremeValueAnalysisAttributes");
    if(searchNode == 0)
        return;

    DataNode *node;
    if((node = searchNode->GetNode("aggregation")) != 0)
    {
        // Allow enums to be int or string in the config file
        if(node->GetNodeType() == INT_NODE)
        {
            int ival = node->AsInt();
            if(ival >= 0 && ival < 3)
                SetAggregation(AggregationType(ival));
        }
        else if(node->GetNodeType() == STRING_NODE)
        {
            AggregationType value;
            if(AggregationType_FromString(node->AsString(), value))
                SetAggregation(value);
        }
    }
    if((node = searchNode->GetNode("displayMonth")) != 0)
    {
        // Allow enums to be int or string in the config file
        if(node->GetNodeType() == INT_NODE)
        {
            int ival = node->AsInt();
            if(ival >= 0 && ival < 12)
                SetDisplayMonth(MonthType(ival));
        }
        else if(node->GetNodeType() == STRING_NODE)
        {
            MonthType value;
            if(MonthType_FromString(node->AsString(), value))
                SetDisplayMonth(value);
        }
    }
    if((node = searchNode->GetNode("displaySeason")) != 0)
    {
        // Allow enums to be int or string in the config file
        if(node->GetNodeType() == INT_NODE)
        {
            int ival = node->AsInt();
            if(ival >= 0 && ival < 4)
                SetDisplaySeason(SeasonType(ival));
        }
        else if(node->GetNodeType() == STRING_NODE)
        {
            SeasonType value;
            if(SeasonType_FromString(node->AsString(), value))
                SetDisplaySeason(value);
        }
    }
    if((node = searchNode->GetNode("dumpData")) != 0)
        SetDumpData(node->AsBool());
    if((node = searchNode->GetNode("dataScaling")) != 0)
        SetDataScaling(node->AsDouble());
}

///////////////////////////////////////////////////////////////////////////////
// Set property methods
///////////////////////////////////////////////////////////////////////////////

void
ExtremeValueAnalysisAttributes::SetAggregation(ExtremeValueAnalysisAttributes::AggregationType aggregation_)
{
    aggregation = aggregation_;
    Select(ID_aggregation, (void *)&aggregation);
}

void
ExtremeValueAnalysisAttributes::SetDisplayMonth(ExtremeValueAnalysisAttributes::MonthType displayMonth_)
{
    displayMonth = displayMonth_;
    Select(ID_displayMonth, (void *)&displayMonth);
}

void
ExtremeValueAnalysisAttributes::SetDisplaySeason(ExtremeValueAnalysisAttributes::SeasonType displaySeason_)
{
    displaySeason = displaySeason_;
    Select(ID_displaySeason, (void *)&displaySeason);
}

void
ExtremeValueAnalysisAttributes::SetDumpData(bool dumpData_)
{
    dumpData = dumpData_;
    Select(ID_dumpData, (void *)&dumpData);
}

void
ExtremeValueAnalysisAttributes::SetDataScaling(double dataScaling_)
{
    dataScaling = dataScaling_;
    Select(ID_dataScaling, (void *)&dataScaling);
}

///////////////////////////////////////////////////////////////////////////////
// Get property methods
///////////////////////////////////////////////////////////////////////////////

ExtremeValueAnalysisAttributes::AggregationType
ExtremeValueAnalysisAttributes::GetAggregation() const
{
    return AggregationType(aggregation);
}

ExtremeValueAnalysisAttributes::MonthType
ExtremeValueAnalysisAttributes::GetDisplayMonth() const
{
    return MonthType(displayMonth);
}

ExtremeValueAnalysisAttributes::SeasonType
ExtremeValueAnalysisAttributes::GetDisplaySeason() const
{
    return SeasonType(displaySeason);
}

bool
ExtremeValueAnalysisAttributes::GetDumpData() const
{
    return dumpData;
}

double
ExtremeValueAnalysisAttributes::GetDataScaling() const
{
    return dataScaling;
}

///////////////////////////////////////////////////////////////////////////////
// Keyframing methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::GetFieldName
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
ExtremeValueAnalysisAttributes::GetFieldName(int index) const
{
    switch (index)
    {
    case ID_aggregation:   return "aggregation";
    case ID_displayMonth:  return "displayMonth";
    case ID_displaySeason: return "displaySeason";
    case ID_dumpData:      return "dumpData";
    case ID_dataScaling:   return "dataScaling";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::GetFieldType
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
ExtremeValueAnalysisAttributes::GetFieldType(int index) const
{
    switch (index)
    {
    case ID_aggregation:   return FieldType_enum;
    case ID_displayMonth:  return FieldType_enum;
    case ID_displaySeason: return FieldType_enum;
    case ID_dumpData:      return FieldType_bool;
    case ID_dataScaling:   return FieldType_double;
    default:  return FieldType_unknown;
    }
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::GetFieldTypeName
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
ExtremeValueAnalysisAttributes::GetFieldTypeName(int index) const
{
    switch (index)
    {
    case ID_aggregation:   return "enum";
    case ID_displayMonth:  return "enum";
    case ID_displaySeason: return "enum";
    case ID_dumpData:      return "bool";
    case ID_dataScaling:   return "double";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: ExtremeValueAnalysisAttributes::FieldsEqual
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
ExtremeValueAnalysisAttributes::FieldsEqual(int index_, const AttributeGroup *rhs) const
{
    const ExtremeValueAnalysisAttributes &obj = *((const ExtremeValueAnalysisAttributes*)rhs);
    bool retval = false;
    switch (index_)
    {
    case ID_aggregation:
        {  // new scope
        retval = (aggregation == obj.aggregation);
        }
        break;
    case ID_displayMonth:
        {  // new scope
        retval = (displayMonth == obj.displayMonth);
        }
        break;
    case ID_displaySeason:
        {  // new scope
        retval = (displaySeason == obj.displaySeason);
        }
        break;
    case ID_dumpData:
        {  // new scope
        retval = (dumpData == obj.dumpData);
        }
        break;
    case ID_dataScaling:
        {  // new scope
        retval = (dataScaling == obj.dataScaling);
        }
        break;
    default: retval = false;
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// User-defined methods.
///////////////////////////////////////////////////////////////////////////////

