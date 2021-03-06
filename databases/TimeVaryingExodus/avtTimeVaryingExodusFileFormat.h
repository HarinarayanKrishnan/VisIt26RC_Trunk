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

// ************************************************************************* //
//                       avtTimeVaryingExodusFileFormat.h                    //
// ************************************************************************* //

#ifndef AVT_TIME_VARYING_EXODUS_FILE_FORMAT_H
#define AVT_TIME_VARYING_EXODUS_FILE_FORMAT_H

#include <avtSTSDFileFormat.h>


class     vtkExodusReader;


// ****************************************************************************
//  Class: avtTimeVaryingExodusFileFormat
//
//  Purpose:
//      An time-varying-exodus file format reader.
//
//  Programmer: Hank Childs
//  Creation:   February 15, 2002
//
//  Modifications:
//    Kathleen Bonnell, Wed Mar  6 08:20:17 PST 2002 
//    vtkScalars and vtkVectors have been deprecated in VTK 4.0, 
//    use vtkDataArray instead.
//
//    Hank Childs, Wed Jun  4 12:02:32 PDT 2008
//    No longer use a cache.
//
// ****************************************************************************

class avtTimeVaryingExodusFileFormat : public avtSTSDFileFormat
{
  public:
                                avtTimeVaryingExodusFileFormat(const char *);
    virtual                    ~avtTimeVaryingExodusFileFormat();

    virtual void                FreeUpResources(void);
    const char                 *GetType(void) { return "Exodus File Format"; };

    virtual vtkDataSet         *GetMesh(const char *);
    virtual vtkDataArray       *GetVar(const char *);
    virtual vtkDataArray       *GetVectorVar(const char *);

    virtual void                PopulateDatabaseMetaData(avtDatabaseMetaData*);
    virtual bool                PerformsMaterialSelection(void) {return true;};

  protected:
    vtkExodusReader            *reader;
    int                         numBlocks;
    bool                        readInDataset;
    std::vector<bool>           validBlock;
    std::vector<int>            blockId;
    std::vector<std::string>    pointVars;
    std::vector<std::string>    cellVars;

    vtkExodusReader            *GetReader(void);
    bool                        GetBlockInformation(int &);
    void                        LoadVariable(vtkExodusReader *, const char *);
    vtkDataSet                 *ForceRead(const char *);
    void                        ReadInDataset(void);
};


#endif

