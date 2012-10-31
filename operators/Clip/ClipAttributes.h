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

#ifndef CLIPATTRIBUTES_H
#define CLIPATTRIBUTES_H
#include <string>
#include <AttributeSubject.h>

#include <PlaneAttributes.h>

// ****************************************************************************
// Class: ClipAttributes
//
// Purpose:
//    This class contains attributes for the clip operator.
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class ClipAttributes : public AttributeSubject
{
public:
    enum ClipStyle
    {
        Plane,
        Sphere
    };
    enum WhichClipPlane
    {
        None,
        Plane1,
        Plane2,
        Plane3
    };
    enum Quality
    {
        Fast,
        Accurate
    };

    // These constructors are for objects of this class
    ClipAttributes();
    ClipAttributes(const ClipAttributes &obj);
protected:
    // These constructors are for objects derived from this class
    ClipAttributes(private_tmfs_t tmfs);
    ClipAttributes(const ClipAttributes &obj, private_tmfs_t tmfs);
public:
    virtual ~ClipAttributes();

    virtual ClipAttributes& operator = (const ClipAttributes &obj);
    virtual bool operator == (const ClipAttributes &obj) const;
    virtual bool operator != (const ClipAttributes &obj) const;
private:
    void Init();
    void Copy(const ClipAttributes &obj);
public:

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectPlane1Origin();
    void SelectPlane2Origin();
    void SelectPlane3Origin();
    void SelectPlane1Normal();
    void SelectPlane2Normal();
    void SelectPlane3Normal();
    void SelectCenter();

    // Property setting methods
    void SetQuality(Quality quality_);
    void SetFuncType(ClipStyle funcType_);
    void SetPlane1Status(bool plane1Status_);
    void SetPlane2Status(bool plane2Status_);
    void SetPlane3Status(bool plane3Status_);
    void SetPlane1Origin(const double *plane1Origin_);
    void SetPlane2Origin(const double *plane2Origin_);
    void SetPlane3Origin(const double *plane3Origin_);
    void SetPlane1Normal(const double *plane1Normal_);
    void SetPlane2Normal(const double *plane2Normal_);
    void SetPlane3Normal(const double *plane3Normal_);
    void SetPlaneInverse(bool planeInverse_);
    void SetPlaneToolControlledClipPlane(WhichClipPlane planeToolControlledClipPlane_);
    void SetCenter(const double *center_);
    void SetRadius(double radius_);
    void SetSphereInverse(bool sphereInverse_);

    // Property getting methods
    Quality      GetQuality() const;
    ClipStyle    GetFuncType() const;
    bool         GetPlane1Status() const;
    bool         GetPlane2Status() const;
    bool         GetPlane3Status() const;
    const double *GetPlane1Origin() const;
          double *GetPlane1Origin();
    const double *GetPlane2Origin() const;
          double *GetPlane2Origin();
    const double *GetPlane3Origin() const;
          double *GetPlane3Origin();
    const double *GetPlane1Normal() const;
          double *GetPlane1Normal();
    const double *GetPlane2Normal() const;
          double *GetPlane2Normal();
    const double *GetPlane3Normal() const;
          double *GetPlane3Normal();
    bool         GetPlaneInverse() const;
    WhichClipPlane GetPlaneToolControlledClipPlane() const;
    const double *GetCenter() const;
          double *GetCenter();
    double       GetRadius() const;
    bool         GetSphereInverse() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);

    // Enum conversion functions
    static std::string ClipStyle_ToString(ClipStyle);
    static bool ClipStyle_FromString(const std::string &, ClipStyle &);
protected:
    static std::string ClipStyle_ToString(int);
public:
    static std::string WhichClipPlane_ToString(WhichClipPlane);
    static bool WhichClipPlane_FromString(const std::string &, WhichClipPlane &);
protected:
    static std::string WhichClipPlane_ToString(int);
public:
    static std::string Quality_ToString(Quality);
    static bool Quality_FromString(const std::string &, Quality &);
protected:
    static std::string Quality_ToString(int);
public:

    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

    // User-defined methods
    virtual bool EqualTo(const AttributeGroup *atts) const;

    // IDs that can be used to identify fields in case statements
    enum {
        ID_quality = 0,
        ID_funcType,
        ID_plane1Status,
        ID_plane2Status,
        ID_plane3Status,
        ID_plane1Origin,
        ID_plane2Origin,
        ID_plane3Origin,
        ID_plane1Normal,
        ID_plane2Normal,
        ID_plane3Normal,
        ID_planeInverse,
        ID_planeToolControlledClipPlane,
        ID_center,
        ID_radius,
        ID_sphereInverse,
        ID__LAST
    };

private:
    int    quality;
    int    funcType;
    bool   plane1Status;
    bool   plane2Status;
    bool   plane3Status;
    double plane1Origin[3];
    double plane2Origin[3];
    double plane3Origin[3];
    double plane1Normal[3];
    double plane2Normal[3];
    double plane3Normal[3];
    bool   planeInverse;
    int    planeToolControlledClipPlane;
    double center[3];
    double radius;
    bool   sphereInverse;

    // Static class format string for type map.
    static const char *TypeMapFormatString;
    static const private_tmfs_t TmfsStruct;
};
#define CLIPATTRIBUTES_TMFS "iibbbDDDDDDbiDdb"

#endif
