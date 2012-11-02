// ***************************************************************************
//
// Copyright (c) 2000 - 2012, Lawrence Livermore National Security, LLC
// Produced at the Lawrence Livermore National Laboratory
// LLNL-CODE-442911
// All rights reserved.
//
// This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
// full copyright notice is contained in the file COPYRIGHT located at the root
// of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
//
// Redistribution  and  use  in  source  and  binary  forms,  with  or  without
// modification, are permitted provided that the following conditions are met:
//
//  - Redistributions of  source code must  retain the above  copyright notice,
//    this list of conditions and the disclaimer below.
//  - Redistributions in binary form must reproduce the above copyright notice,
//    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
//    documentation and/or other materials provided with the distribution.
//  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
//    be used to endorse or promote products derived from this software without
//    specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
// ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
// LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
// DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
// SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
// CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
// LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
// OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ***************************************************************************

package llnl.visit.operators;

import llnl.visit.AttributeSubject;
import llnl.visit.CommunicationBuffer;
import llnl.visit.Plugin;
import java.lang.Integer;
import java.util.Vector;

// ****************************************************************************
// Class: AMRStitchCellAttributes
//
// Purpose:
//    Attributes for Stitch Cell Operator
//
// Notes:      Autogenerated by xml2java.
//
// Programmer: xml2java
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

public class AMRStitchCellAttributes extends AttributeSubject implements Plugin
{
    private static int AMRStitchCellAttributes_numAdditionalAtts = 6;

    // Enum values
    public final static int CREATETYPE_DUALGRIDANDSTITCHCELLS = 0;
    public final static int CREATETYPE_DUALGRID = 1;
    public final static int CREATETYPE_STITCHCELLS = 2;


    public AMRStitchCellAttributes()
    {
        super(AMRStitchCellAttributes_numAdditionalAtts);

            AddCaseNo = true;
        OnlyProcessListedDomains = false;
        Domains = new Vector();
        OnlyProcessLevel = false;
        Level = 0;
    }

    public AMRStitchCellAttributes(int nMoreFields)
    {
        super(AMRStitchCellAttributes_numAdditionalAtts + nMoreFields);

            AddCaseNo = true;
        OnlyProcessListedDomains = false;
        Domains = new Vector();
        OnlyProcessLevel = false;
        Level = 0;
    }

    public AMRStitchCellAttributes(AMRStitchCellAttributes obj)
    {
        super(AMRStitchCellAttributes_numAdditionalAtts);

        int i;

        CreateCellsOfType = obj.CreateCellsOfType;
        AddCaseNo = obj.AddCaseNo;
        OnlyProcessListedDomains = obj.OnlyProcessListedDomains;
        Domains = new Vector();
        for(i = 0; i < obj.Domains.size(); ++i)
        {
            Integer iv = (Integer)obj.Domains.elementAt(i);
            Domains.addElement(new Integer(iv.intValue()));
        }
        OnlyProcessLevel = obj.OnlyProcessLevel;
        Level = obj.Level;

        SelectAll();
    }

    public int Offset()
    {
        return super.Offset() + super.GetNumAdditionalAttributes();
    }

    public int GetNumAdditionalAttributes()
    {
        return AMRStitchCellAttributes_numAdditionalAtts;
    }

    public boolean equals(AMRStitchCellAttributes obj)
    {
        int i;

        // Compare the elements in the Domains vector.
        boolean Domains_equal = (obj.Domains.size() == Domains.size());
        for(i = 0; (i < Domains.size()) && Domains_equal; ++i)
        {
            // Make references to Integer from Object.
            Integer Domains1 = (Integer)Domains.elementAt(i);
            Integer Domains2 = (Integer)obj.Domains.elementAt(i);
            Domains_equal = Domains1.equals(Domains2);
        }
        // Create the return value
        return ((CreateCellsOfType == obj.CreateCellsOfType) &&
                (AddCaseNo == obj.AddCaseNo) &&
                (OnlyProcessListedDomains == obj.OnlyProcessListedDomains) &&
                Domains_equal &&
                (OnlyProcessLevel == obj.OnlyProcessLevel) &&
                (Level == obj.Level));
    }

    public String GetName() { return "AMRStitchCell"; }
    public String GetVersion() { return "1.0"; }

    // Property setting methods
    public void SetCreateCellsOfType(int CreateCellsOfType_)
    {
        CreateCellsOfType = CreateCellsOfType_;
        Select(0);
    }

    public void SetAddCaseNo(boolean AddCaseNo_)
    {
        AddCaseNo = AddCaseNo_;
        Select(1);
    }

    public void SetOnlyProcessListedDomains(boolean OnlyProcessListedDomains_)
    {
        OnlyProcessListedDomains = OnlyProcessListedDomains_;
        Select(2);
    }

    public void SetDomains(Vector Domains_)
    {
        Domains = Domains_;
        Select(3);
    }

    public void SetOnlyProcessLevel(boolean OnlyProcessLevel_)
    {
        OnlyProcessLevel = OnlyProcessLevel_;
        Select(4);
    }

    public void SetLevel(int Level_)
    {
        Level = Level_;
        Select(5);
    }

    // Property getting methods
    public int     GetCreateCellsOfType() { return CreateCellsOfType; }
    public boolean GetAddCaseNo() { return AddCaseNo; }
    public boolean GetOnlyProcessListedDomains() { return OnlyProcessListedDomains; }
    public Vector  GetDomains() { return Domains; }
    public boolean GetOnlyProcessLevel() { return OnlyProcessLevel; }
    public int     GetLevel() { return Level; }

    // Write and read methods.
    public void WriteAtts(CommunicationBuffer buf)
    {
        if(WriteSelect(0, buf))
            buf.WriteInt(CreateCellsOfType);
        if(WriteSelect(1, buf))
            buf.WriteBool(AddCaseNo);
        if(WriteSelect(2, buf))
            buf.WriteBool(OnlyProcessListedDomains);
        if(WriteSelect(3, buf))
            buf.WriteIntVector(Domains);
        if(WriteSelect(4, buf))
            buf.WriteBool(OnlyProcessLevel);
        if(WriteSelect(5, buf))
            buf.WriteInt(Level);
    }

    public void ReadAtts(int index, CommunicationBuffer buf)
    {
        switch(index)
        {
        case 0:
            SetCreateCellsOfType(buf.ReadInt());
            break;
        case 1:
            SetAddCaseNo(buf.ReadBool());
            break;
        case 2:
            SetOnlyProcessListedDomains(buf.ReadBool());
            break;
        case 3:
            SetDomains(buf.ReadIntVector());
            break;
        case 4:
            SetOnlyProcessLevel(buf.ReadBool());
            break;
        case 5:
            SetLevel(buf.ReadInt());
            break;
        }
    }

    public String toString(String indent)
    {
        String str = new String();
        str = str + indent + "CreateCellsOfType = ";
        if(CreateCellsOfType == CREATETYPE_DUALGRIDANDSTITCHCELLS)
            str = str + "CREATETYPE_DUALGRIDANDSTITCHCELLS";
        if(CreateCellsOfType == CREATETYPE_DUALGRID)
            str = str + "CREATETYPE_DUALGRID";
        if(CreateCellsOfType == CREATETYPE_STITCHCELLS)
            str = str + "CREATETYPE_STITCHCELLS";
        str = str + "\n";
        str = str + boolToString("AddCaseNo", AddCaseNo, indent) + "\n";
        str = str + boolToString("OnlyProcessListedDomains", OnlyProcessListedDomains, indent) + "\n";
        str = str + intVectorToString("Domains", Domains, indent) + "\n";
        str = str + boolToString("OnlyProcessLevel", OnlyProcessLevel, indent) + "\n";
        str = str + intToString("Level", Level, indent) + "\n";
        return str;
    }


    // Attributes
    private int     CreateCellsOfType;
    private boolean AddCaseNo;
    private boolean OnlyProcessListedDomains;
    private Vector  Domains; // vector of Integer objects
    private boolean OnlyProcessLevel;
    private int     Level;
}
