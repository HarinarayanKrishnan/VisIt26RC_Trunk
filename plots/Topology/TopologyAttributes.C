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

#include <TopologyAttributes.h>
#include <DataNode.h>

// ****************************************************************************
// Method: TopologyAttributes::TopologyAttributes
//
// Purpose: 
//   Init utility for the TopologyAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void TopologyAttributes::Init()
{
    lineWidth = 2;
    lineStyle = 0;
    multiColor.AddColors(ColorAttribute(255,0,0));
    multiColor.AddColors(ColorAttribute(0,255,0));
    multiColor.AddColors(ColorAttribute(0,0,255));
    multiColor.AddColors(ColorAttribute(0,255,255));
    minOpacity = 1;
    minPlateauOpacity = 1;
    maxPlateauOpacity = 1;
    maxOpacity = 1;
    tolerance = 1e-06;
    hitpercent = 0;

    TopologyAttributes::SelectAll();
}

// ****************************************************************************
// Method: TopologyAttributes::TopologyAttributes
//
// Purpose: 
//   Copy utility for the TopologyAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void TopologyAttributes::Copy(const TopologyAttributes &obj)
{
    lineWidth = obj.lineWidth;
    lineStyle = obj.lineStyle;
    multiColor = obj.multiColor;
    minOpacity = obj.minOpacity;
    minPlateauOpacity = obj.minPlateauOpacity;
    maxPlateauOpacity = obj.maxPlateauOpacity;
    maxOpacity = obj.maxOpacity;
    tolerance = obj.tolerance;
    hitpercent = obj.hitpercent;

    TopologyAttributes::SelectAll();
}

// Type map format string
const char *TopologyAttributes::TypeMapFormatString = TOPOLOGYATTRIBUTES_TMFS;
const AttributeGroup::private_tmfs_t TopologyAttributes::TmfsStruct = {TOPOLOGYATTRIBUTES_TMFS};


// ****************************************************************************
// Method: TopologyAttributes::TopologyAttributes
//
// Purpose: 
//   Default constructor for the TopologyAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

TopologyAttributes::TopologyAttributes() : 
    AttributeSubject(TopologyAttributes::TypeMapFormatString)
{
    TopologyAttributes::Init();
}

// ****************************************************************************
// Method: TopologyAttributes::TopologyAttributes
//
// Purpose: 
//   Constructor for the derived classes of TopologyAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

TopologyAttributes::TopologyAttributes(private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    TopologyAttributes::Init();
}

// ****************************************************************************
// Method: TopologyAttributes::TopologyAttributes
//
// Purpose: 
//   Copy constructor for the TopologyAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

TopologyAttributes::TopologyAttributes(const TopologyAttributes &obj) : 
    AttributeSubject(TopologyAttributes::TypeMapFormatString)
{
    TopologyAttributes::Copy(obj);
}

// ****************************************************************************
// Method: TopologyAttributes::TopologyAttributes
//
// Purpose: 
//   Copy constructor for derived classes of the TopologyAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

TopologyAttributes::TopologyAttributes(const TopologyAttributes &obj, private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    TopologyAttributes::Copy(obj);
}

// ****************************************************************************
// Method: TopologyAttributes::~TopologyAttributes
//
// Purpose: 
//   Destructor for the TopologyAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

TopologyAttributes::~TopologyAttributes()
{
    // nothing here
}

// ****************************************************************************
// Method: TopologyAttributes::operator = 
//
// Purpose: 
//   Assignment operator for the TopologyAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

TopologyAttributes& 
TopologyAttributes::operator = (const TopologyAttributes &obj)
{
    if (this == &obj) return *this;

    TopologyAttributes::Copy(obj);

    return *this;
}

// ****************************************************************************
// Method: TopologyAttributes::operator == 
//
// Purpose: 
//   Comparison operator == for the TopologyAttributes class.
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
TopologyAttributes::operator == (const TopologyAttributes &obj) const
{
    // Create the return value
    return ((lineWidth == obj.lineWidth) &&
            (lineStyle == obj.lineStyle) &&
            (multiColor == obj.multiColor) &&
            (minOpacity == obj.minOpacity) &&
            (minPlateauOpacity == obj.minPlateauOpacity) &&
            (maxPlateauOpacity == obj.maxPlateauOpacity) &&
            (maxOpacity == obj.maxOpacity) &&
            (tolerance == obj.tolerance) &&
            (hitpercent == obj.hitpercent));
}

// ****************************************************************************
// Method: TopologyAttributes::operator != 
//
// Purpose: 
//   Comparison operator != for the TopologyAttributes class.
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
TopologyAttributes::operator != (const TopologyAttributes &obj) const
{
    return !(this->operator == (obj));
}

// ****************************************************************************
// Method: TopologyAttributes::TypeName
//
// Purpose: 
//   Type name method for the TopologyAttributes class.
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
TopologyAttributes::TypeName() const
{
    return "TopologyAttributes";
}

// ****************************************************************************
// Method: TopologyAttributes::CopyAttributes
//
// Purpose: 
//   CopyAttributes method for the TopologyAttributes class.
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
TopologyAttributes::CopyAttributes(const AttributeGroup *atts)
{
    if(TypeName() != atts->TypeName())
        return false;

    // Call assignment operator.
    const TopologyAttributes *tmp = (const TopologyAttributes *)atts;
    *this = *tmp;

    return true;
}

// ****************************************************************************
// Method: TopologyAttributes::CreateCompatible
//
// Purpose: 
//   CreateCompatible method for the TopologyAttributes class.
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
TopologyAttributes::CreateCompatible(const std::string &tname) const
{
    AttributeSubject *retval = 0;
    if(TypeName() == tname)
        retval = new TopologyAttributes(*this);
    // Other cases could go here too. 

    return retval;
}

// ****************************************************************************
// Method: TopologyAttributes::NewInstance
//
// Purpose: 
//   NewInstance method for the TopologyAttributes class.
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
TopologyAttributes::NewInstance(bool copy) const
{
    AttributeSubject *retval = 0;
    if(copy)
        retval = new TopologyAttributes(*this);
    else
        retval = new TopologyAttributes;

    return retval;
}

// ****************************************************************************
// Method: TopologyAttributes::SelectAll
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
TopologyAttributes::SelectAll()
{
    Select(ID_lineWidth,         (void *)&lineWidth);
    Select(ID_lineStyle,         (void *)&lineStyle);
    Select(ID_multiColor,        (void *)&multiColor);
    Select(ID_minOpacity,        (void *)&minOpacity);
    Select(ID_minPlateauOpacity, (void *)&minPlateauOpacity);
    Select(ID_maxPlateauOpacity, (void *)&maxPlateauOpacity);
    Select(ID_maxOpacity,        (void *)&maxOpacity);
    Select(ID_tolerance,         (void *)&tolerance);
    Select(ID_hitpercent,        (void *)&hitpercent);
}

///////////////////////////////////////////////////////////////////////////////
// Persistence methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: TopologyAttributes::CreateNode
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
TopologyAttributes::CreateNode(DataNode *parentNode, bool completeSave, bool forceAdd)
{
    if(parentNode == 0)
        return false;

    TopologyAttributes defaultObject;
    bool addToParent = false;
    // Create a node for TopologyAttributes.
    DataNode *node = new DataNode("TopologyAttributes");

    if(completeSave || !FieldsEqual(ID_lineWidth, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("lineWidth", lineWidth));
    }

    if(completeSave || !FieldsEqual(ID_lineStyle, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("lineStyle", lineStyle));
    }

    if(completeSave || !FieldsEqual(ID_multiColor, &defaultObject))
    {
        DataNode *multiColorNode = new DataNode("multiColor");
        if(multiColor.CreateNode(multiColorNode, completeSave, false))
        {
            addToParent = true;
            node->AddNode(multiColorNode);
        }
        else
            delete multiColorNode;
    }

    if(completeSave || !FieldsEqual(ID_minOpacity, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("minOpacity", minOpacity));
    }

    if(completeSave || !FieldsEqual(ID_minPlateauOpacity, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("minPlateauOpacity", minPlateauOpacity));
    }

    if(completeSave || !FieldsEqual(ID_maxPlateauOpacity, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("maxPlateauOpacity", maxPlateauOpacity));
    }

    if(completeSave || !FieldsEqual(ID_maxOpacity, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("maxOpacity", maxOpacity));
    }

    if(completeSave || !FieldsEqual(ID_tolerance, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("tolerance", tolerance));
    }

    if(completeSave || !FieldsEqual(ID_hitpercent, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("hitpercent", hitpercent));
    }


    // Add the node to the parent node.
    if(addToParent || forceAdd)
        parentNode->AddNode(node);
    else
        delete node;

    return (addToParent || forceAdd);
}

// ****************************************************************************
// Method: TopologyAttributes::SetFromNode
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
TopologyAttributes::SetFromNode(DataNode *parentNode)
{
    if(parentNode == 0)
        return;

    DataNode *searchNode = parentNode->GetNode("TopologyAttributes");
    if(searchNode == 0)
        return;

    DataNode *node;
    if((node = searchNode->GetNode("lineWidth")) != 0)
        SetLineWidth(node->AsInt());
    if((node = searchNode->GetNode("lineStyle")) != 0)
        SetLineStyle(node->AsInt());
    if((node = searchNode->GetNode("multiColor")) != 0)
        multiColor.SetFromNode(node);
    if((node = searchNode->GetNode("minOpacity")) != 0)
        SetMinOpacity(node->AsDouble());
    if((node = searchNode->GetNode("minPlateauOpacity")) != 0)
        SetMinPlateauOpacity(node->AsDouble());
    if((node = searchNode->GetNode("maxPlateauOpacity")) != 0)
        SetMaxPlateauOpacity(node->AsDouble());
    if((node = searchNode->GetNode("maxOpacity")) != 0)
        SetMaxOpacity(node->AsDouble());
    if((node = searchNode->GetNode("tolerance")) != 0)
        SetTolerance(node->AsDouble());
    if((node = searchNode->GetNode("hitpercent")) != 0)
        SetHitpercent(node->AsDouble());
}

///////////////////////////////////////////////////////////////////////////////
// Set property methods
///////////////////////////////////////////////////////////////////////////////

void
TopologyAttributes::SetLineWidth(int lineWidth_)
{
    lineWidth = lineWidth_;
    Select(ID_lineWidth, (void *)&lineWidth);
}

void
TopologyAttributes::SetLineStyle(int lineStyle_)
{
    lineStyle = lineStyle_;
    Select(ID_lineStyle, (void *)&lineStyle);
}

void
TopologyAttributes::SetMultiColor(const ColorAttributeList &multiColor_)
{
    multiColor = multiColor_;
    Select(ID_multiColor, (void *)&multiColor);
}

void
TopologyAttributes::SetMinOpacity(double minOpacity_)
{
    minOpacity = minOpacity_;
    Select(ID_minOpacity, (void *)&minOpacity);
}

void
TopologyAttributes::SetMinPlateauOpacity(double minPlateauOpacity_)
{
    minPlateauOpacity = minPlateauOpacity_;
    Select(ID_minPlateauOpacity, (void *)&minPlateauOpacity);
}

void
TopologyAttributes::SetMaxPlateauOpacity(double maxPlateauOpacity_)
{
    maxPlateauOpacity = maxPlateauOpacity_;
    Select(ID_maxPlateauOpacity, (void *)&maxPlateauOpacity);
}

void
TopologyAttributes::SetMaxOpacity(double maxOpacity_)
{
    maxOpacity = maxOpacity_;
    Select(ID_maxOpacity, (void *)&maxOpacity);
}

void
TopologyAttributes::SetTolerance(double tolerance_)
{
    tolerance = tolerance_;
    Select(ID_tolerance, (void *)&tolerance);
}

void
TopologyAttributes::SetHitpercent(double hitpercent_)
{
    hitpercent = hitpercent_;
    Select(ID_hitpercent, (void *)&hitpercent);
}

///////////////////////////////////////////////////////////////////////////////
// Get property methods
///////////////////////////////////////////////////////////////////////////////

int
TopologyAttributes::GetLineWidth() const
{
    return lineWidth;
}

int
TopologyAttributes::GetLineStyle() const
{
    return lineStyle;
}

const ColorAttributeList &
TopologyAttributes::GetMultiColor() const
{
    return multiColor;
}

ColorAttributeList &
TopologyAttributes::GetMultiColor()
{
    return multiColor;
}

double
TopologyAttributes::GetMinOpacity() const
{
    return minOpacity;
}

double
TopologyAttributes::GetMinPlateauOpacity() const
{
    return minPlateauOpacity;
}

double
TopologyAttributes::GetMaxPlateauOpacity() const
{
    return maxPlateauOpacity;
}

double
TopologyAttributes::GetMaxOpacity() const
{
    return maxOpacity;
}

double
TopologyAttributes::GetTolerance() const
{
    return tolerance;
}

double
TopologyAttributes::GetHitpercent() const
{
    return hitpercent;
}

///////////////////////////////////////////////////////////////////////////////
// Select property methods
///////////////////////////////////////////////////////////////////////////////

void
TopologyAttributes::SelectMultiColor()
{
    Select(ID_multiColor, (void *)&multiColor);
}

///////////////////////////////////////////////////////////////////////////////
// Keyframing methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: TopologyAttributes::GetFieldName
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
TopologyAttributes::GetFieldName(int index) const
{
    switch (index)
    {
    case ID_lineWidth:         return "lineWidth";
    case ID_lineStyle:         return "lineStyle";
    case ID_multiColor:        return "multiColor";
    case ID_minOpacity:        return "minOpacity";
    case ID_minPlateauOpacity: return "minPlateauOpacity";
    case ID_maxPlateauOpacity: return "maxPlateauOpacity";
    case ID_maxOpacity:        return "maxOpacity";
    case ID_tolerance:         return "tolerance";
    case ID_hitpercent:        return "hitpercent";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: TopologyAttributes::GetFieldType
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
TopologyAttributes::GetFieldType(int index) const
{
    switch (index)
    {
    case ID_lineWidth:         return FieldType_linewidth;
    case ID_lineStyle:         return FieldType_linestyle;
    case ID_multiColor:        return FieldType_att;
    case ID_minOpacity:        return FieldType_opacity;
    case ID_minPlateauOpacity: return FieldType_opacity;
    case ID_maxPlateauOpacity: return FieldType_opacity;
    case ID_maxOpacity:        return FieldType_opacity;
    case ID_tolerance:         return FieldType_double;
    case ID_hitpercent:        return FieldType_double;
    default:  return FieldType_unknown;
    }
}

// ****************************************************************************
// Method: TopologyAttributes::GetFieldTypeName
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
TopologyAttributes::GetFieldTypeName(int index) const
{
    switch (index)
    {
    case ID_lineWidth:         return "linewidth";
    case ID_lineStyle:         return "linestyle";
    case ID_multiColor:        return "att";
    case ID_minOpacity:        return "opacity";
    case ID_minPlateauOpacity: return "opacity";
    case ID_maxPlateauOpacity: return "opacity";
    case ID_maxOpacity:        return "opacity";
    case ID_tolerance:         return "double";
    case ID_hitpercent:        return "double";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: TopologyAttributes::FieldsEqual
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
TopologyAttributes::FieldsEqual(int index_, const AttributeGroup *rhs) const
{
    const TopologyAttributes &obj = *((const TopologyAttributes*)rhs);
    bool retval = false;
    switch (index_)
    {
    case ID_lineWidth:
        {  // new scope
        retval = (lineWidth == obj.lineWidth);
        }
        break;
    case ID_lineStyle:
        {  // new scope
        retval = (lineStyle == obj.lineStyle);
        }
        break;
    case ID_multiColor:
        {  // new scope
        retval = (multiColor == obj.multiColor);
        }
        break;
    case ID_minOpacity:
        {  // new scope
        retval = (minOpacity == obj.minOpacity);
        }
        break;
    case ID_minPlateauOpacity:
        {  // new scope
        retval = (minPlateauOpacity == obj.minPlateauOpacity);
        }
        break;
    case ID_maxPlateauOpacity:
        {  // new scope
        retval = (maxPlateauOpacity == obj.maxPlateauOpacity);
        }
        break;
    case ID_maxOpacity:
        {  // new scope
        retval = (maxOpacity == obj.maxOpacity);
        }
        break;
    case ID_tolerance:
        {  // new scope
        retval = (tolerance == obj.tolerance);
        }
        break;
    case ID_hitpercent:
        {  // new scope
        retval = (hitpercent == obj.hitpercent);
        }
        break;
    default: retval = false;
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// User-defined methods.
///////////////////////////////////////////////////////////////////////////////

bool
TopologyAttributes::ChangesRequireRecalculation(const TopologyAttributes &obj)
{
    return ((tolerance != obj.tolerance) ||
            (hitpercent != obj.hitpercent));
}

