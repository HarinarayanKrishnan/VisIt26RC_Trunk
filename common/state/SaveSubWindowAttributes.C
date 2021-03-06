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

#include <SaveSubWindowAttributes.h>
#include <DataNode.h>

// ****************************************************************************
// Method: SaveSubWindowAttributes::SaveSubWindowAttributes
//
// Purpose: 
//   Init utility for the SaveSubWindowAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void SaveSubWindowAttributes::Init()
{
    position[0] = 0;
    position[1] = 0;
    size[0] = 128;
    size[1] = 128;
    layer = 0;
    transparency = 0;
    omitWindow = false;

    SaveSubWindowAttributes::SelectAll();
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::SaveSubWindowAttributes
//
// Purpose: 
//   Copy utility for the SaveSubWindowAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void SaveSubWindowAttributes::Copy(const SaveSubWindowAttributes &obj)
{
    position[0] = obj.position[0];
    position[1] = obj.position[1];

    size[0] = obj.size[0];
    size[1] = obj.size[1];

    layer = obj.layer;
    transparency = obj.transparency;
    omitWindow = obj.omitWindow;

    SaveSubWindowAttributes::SelectAll();
}

// Type map format string
const char *SaveSubWindowAttributes::TypeMapFormatString = SAVESUBWINDOWATTRIBUTES_TMFS;
const AttributeGroup::private_tmfs_t SaveSubWindowAttributes::TmfsStruct = {SAVESUBWINDOWATTRIBUTES_TMFS};


// ****************************************************************************
// Method: SaveSubWindowAttributes::SaveSubWindowAttributes
//
// Purpose: 
//   Default constructor for the SaveSubWindowAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

SaveSubWindowAttributes::SaveSubWindowAttributes() : 
    AttributeSubject(SaveSubWindowAttributes::TypeMapFormatString)
{
    SaveSubWindowAttributes::Init();
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::SaveSubWindowAttributes
//
// Purpose: 
//   Constructor for the derived classes of SaveSubWindowAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

SaveSubWindowAttributes::SaveSubWindowAttributes(private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    SaveSubWindowAttributes::Init();
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::SaveSubWindowAttributes
//
// Purpose: 
//   Copy constructor for the SaveSubWindowAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

SaveSubWindowAttributes::SaveSubWindowAttributes(const SaveSubWindowAttributes &obj) : 
    AttributeSubject(SaveSubWindowAttributes::TypeMapFormatString)
{
    SaveSubWindowAttributes::Copy(obj);
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::SaveSubWindowAttributes
//
// Purpose: 
//   Copy constructor for derived classes of the SaveSubWindowAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

SaveSubWindowAttributes::SaveSubWindowAttributes(const SaveSubWindowAttributes &obj, private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    SaveSubWindowAttributes::Copy(obj);
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::~SaveSubWindowAttributes
//
// Purpose: 
//   Destructor for the SaveSubWindowAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

SaveSubWindowAttributes::~SaveSubWindowAttributes()
{
    // nothing here
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::operator = 
//
// Purpose: 
//   Assignment operator for the SaveSubWindowAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

SaveSubWindowAttributes& 
SaveSubWindowAttributes::operator = (const SaveSubWindowAttributes &obj)
{
    if (this == &obj) return *this;

    SaveSubWindowAttributes::Copy(obj);

    return *this;
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::operator == 
//
// Purpose: 
//   Comparison operator == for the SaveSubWindowAttributes class.
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
SaveSubWindowAttributes::operator == (const SaveSubWindowAttributes &obj) const
{
    // Compare the position arrays.
    bool position_equal = true;
    for(int i = 0; i < 2 && position_equal; ++i)
        position_equal = (position[i] == obj.position[i]);

    // Compare the size arrays.
    bool size_equal = true;
    for(int i = 0; i < 2 && size_equal; ++i)
        size_equal = (size[i] == obj.size[i]);

    // Create the return value
    return (position_equal &&
            size_equal &&
            (layer == obj.layer) &&
            (transparency == obj.transparency) &&
            (omitWindow == obj.omitWindow));
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::operator != 
//
// Purpose: 
//   Comparison operator != for the SaveSubWindowAttributes class.
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
SaveSubWindowAttributes::operator != (const SaveSubWindowAttributes &obj) const
{
    return !(this->operator == (obj));
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::TypeName
//
// Purpose: 
//   Type name method for the SaveSubWindowAttributes class.
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
SaveSubWindowAttributes::TypeName() const
{
    return "SaveSubWindowAttributes";
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::CopyAttributes
//
// Purpose: 
//   CopyAttributes method for the SaveSubWindowAttributes class.
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
SaveSubWindowAttributes::CopyAttributes(const AttributeGroup *atts)
{
    if(TypeName() != atts->TypeName())
        return false;

    // Call assignment operator.
    const SaveSubWindowAttributes *tmp = (const SaveSubWindowAttributes *)atts;
    *this = *tmp;

    return true;
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::CreateCompatible
//
// Purpose: 
//   CreateCompatible method for the SaveSubWindowAttributes class.
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
SaveSubWindowAttributes::CreateCompatible(const std::string &tname) const
{
    AttributeSubject *retval = 0;
    if(TypeName() == tname)
        retval = new SaveSubWindowAttributes(*this);
    // Other cases could go here too. 

    return retval;
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::NewInstance
//
// Purpose: 
//   NewInstance method for the SaveSubWindowAttributes class.
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
SaveSubWindowAttributes::NewInstance(bool copy) const
{
    AttributeSubject *retval = 0;
    if(copy)
        retval = new SaveSubWindowAttributes(*this);
    else
        retval = new SaveSubWindowAttributes;

    return retval;
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::SelectAll
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
SaveSubWindowAttributes::SelectAll()
{
    Select(ID_position,     (void *)position, 2);
    Select(ID_size,         (void *)size, 2);
    Select(ID_layer,        (void *)&layer);
    Select(ID_transparency, (void *)&transparency);
    Select(ID_omitWindow,   (void *)&omitWindow);
}

///////////////////////////////////////////////////////////////////////////////
// Persistence methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: SaveSubWindowAttributes::CreateNode
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
SaveSubWindowAttributes::CreateNode(DataNode *parentNode, bool completeSave, bool forceAdd)
{
    if(parentNode == 0)
        return false;

    SaveSubWindowAttributes defaultObject;
    bool addToParent = false;
    // Create a node for SaveSubWindowAttributes.
    DataNode *node = new DataNode("SaveSubWindowAttributes");

    if(completeSave || !FieldsEqual(ID_position, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("position", position, 2));
    }

    if(completeSave || !FieldsEqual(ID_size, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("size", size, 2));
    }

    if(completeSave || !FieldsEqual(ID_layer, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("layer", layer));
    }

    if(completeSave || !FieldsEqual(ID_transparency, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("transparency", transparency));
    }

    if(completeSave || !FieldsEqual(ID_omitWindow, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("omitWindow", omitWindow));
    }


    // Add the node to the parent node.
    if(addToParent || forceAdd)
        parentNode->AddNode(node);
    else
        delete node;

    return (addToParent || forceAdd);
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::SetFromNode
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
SaveSubWindowAttributes::SetFromNode(DataNode *parentNode)
{
    if(parentNode == 0)
        return;

    DataNode *searchNode = parentNode->GetNode("SaveSubWindowAttributes");
    if(searchNode == 0)
        return;

    DataNode *node;
    if((node = searchNode->GetNode("position")) != 0)
        SetPosition(node->AsIntArray());
    if((node = searchNode->GetNode("size")) != 0)
        SetSize(node->AsIntArray());
    if((node = searchNode->GetNode("layer")) != 0)
        SetLayer(node->AsInt());
    if((node = searchNode->GetNode("transparency")) != 0)
        SetTransparency(node->AsDouble());
    if((node = searchNode->GetNode("omitWindow")) != 0)
        SetOmitWindow(node->AsBool());
}

///////////////////////////////////////////////////////////////////////////////
// Set property methods
///////////////////////////////////////////////////////////////////////////////

void
SaveSubWindowAttributes::SetPosition(const int *position_)
{
    position[0] = position_[0];
    position[1] = position_[1];
    Select(ID_position, (void *)position, 2);
}

void
SaveSubWindowAttributes::SetSize(const int *size_)
{
    size[0] = size_[0];
    size[1] = size_[1];
    Select(ID_size, (void *)size, 2);
}

void
SaveSubWindowAttributes::SetLayer(int layer_)
{
    layer = layer_;
    Select(ID_layer, (void *)&layer);
}

void
SaveSubWindowAttributes::SetTransparency(double transparency_)
{
    transparency = transparency_;
    Select(ID_transparency, (void *)&transparency);
}

void
SaveSubWindowAttributes::SetOmitWindow(bool omitWindow_)
{
    omitWindow = omitWindow_;
    Select(ID_omitWindow, (void *)&omitWindow);
}

///////////////////////////////////////////////////////////////////////////////
// Get property methods
///////////////////////////////////////////////////////////////////////////////

const int *
SaveSubWindowAttributes::GetPosition() const
{
    return position;
}

int *
SaveSubWindowAttributes::GetPosition()
{
    return position;
}

const int *
SaveSubWindowAttributes::GetSize() const
{
    return size;
}

int *
SaveSubWindowAttributes::GetSize()
{
    return size;
}

int
SaveSubWindowAttributes::GetLayer() const
{
    return layer;
}

double
SaveSubWindowAttributes::GetTransparency() const
{
    return transparency;
}

bool
SaveSubWindowAttributes::GetOmitWindow() const
{
    return omitWindow;
}

///////////////////////////////////////////////////////////////////////////////
// Select property methods
///////////////////////////////////////////////////////////////////////////////

void
SaveSubWindowAttributes::SelectPosition()
{
    Select(ID_position, (void *)position, 2);
}

void
SaveSubWindowAttributes::SelectSize()
{
    Select(ID_size, (void *)size, 2);
}

///////////////////////////////////////////////////////////////////////////////
// Keyframing methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: SaveSubWindowAttributes::GetFieldName
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
SaveSubWindowAttributes::GetFieldName(int index) const
{
    switch (index)
    {
    case ID_position:     return "position";
    case ID_size:         return "size";
    case ID_layer:        return "layer";
    case ID_transparency: return "transparency";
    case ID_omitWindow:   return "omitWindow";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::GetFieldType
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
SaveSubWindowAttributes::GetFieldType(int index) const
{
    switch (index)
    {
    case ID_position:     return FieldType_intArray;
    case ID_size:         return FieldType_intArray;
    case ID_layer:        return FieldType_int;
    case ID_transparency: return FieldType_double;
    case ID_omitWindow:   return FieldType_bool;
    default:  return FieldType_unknown;
    }
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::GetFieldTypeName
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
SaveSubWindowAttributes::GetFieldTypeName(int index) const
{
    switch (index)
    {
    case ID_position:     return "intArray";
    case ID_size:         return "intArray";
    case ID_layer:        return "int";
    case ID_transparency: return "double";
    case ID_omitWindow:   return "bool";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: SaveSubWindowAttributes::FieldsEqual
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
SaveSubWindowAttributes::FieldsEqual(int index_, const AttributeGroup *rhs) const
{
    const SaveSubWindowAttributes &obj = *((const SaveSubWindowAttributes*)rhs);
    bool retval = false;
    switch (index_)
    {
    case ID_position:
        {  // new scope
        // Compare the position arrays.
        bool position_equal = true;
        for(int i = 0; i < 2 && position_equal; ++i)
            position_equal = (position[i] == obj.position[i]);

        retval = position_equal;
        }
        break;
    case ID_size:
        {  // new scope
        // Compare the size arrays.
        bool size_equal = true;
        for(int i = 0; i < 2 && size_equal; ++i)
            size_equal = (size[i] == obj.size[i]);

        retval = size_equal;
        }
        break;
    case ID_layer:
        {  // new scope
        retval = (layer == obj.layer);
        }
        break;
    case ID_transparency:
        {  // new scope
        retval = (transparency == obj.transparency);
        }
        break;
    case ID_omitWindow:
        {  // new scope
        retval = (omitWindow == obj.omitWindow);
        }
        break;
    default: retval = false;
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// User-defined methods.
///////////////////////////////////////////////////////////////////////////////

