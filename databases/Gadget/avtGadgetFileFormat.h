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
//                            avtGadgetFileFormat.h                           //
// ************************************************************************* //

#ifndef AVT_Gadget_FILE_FORMAT_H
#define AVT_Gadget_FILE_FORMAT_H

#include <avtSTSDFileFormat.h>
#define int4bytes int


// ****************************************************************************
//  Class: avtGadgetFileFormat
//
//  Purpose:
//      Reads in Gadget files as a plugin to VisIt.
//
//  Programmer: rwb -- generated by xml2avt
//  Creation:   Wed Oct 22 12:47:04 PDT 2008
//
// ****************************************************************************

class avtGadgetFileFormat : public avtSTSDFileFormat
{
  public:
                       avtGadgetFileFormat(const char *filename);
    virtual           ~avtGadgetFileFormat() {;};
    virtual const char    *GetType(void)   { return "Gadget"; };
    virtual void           FreeUpResources(void); 

    virtual vtkDataSet    *GetMesh(const char *);
    virtual vtkDataArray  *GetVar(const char *);
    virtual vtkDataArray  *GetVectorVar(const char *);
    virtual bool ReturnsValidCycle() const { return true; }
    virtual int GetCycle(void);
    virtual bool ReturnsValidTime() const { return true; }
    virtual double GetTime(void);
    virtual int GetCycleFromFilename(const char *f) const
    {
      return GuessCycle(f);
    }
  protected:
    // DATA MEMBERS
    FILE *fd;
    int4bytes blksize,swap;
    unsigned long ntot;
    double masstab[6],redshift,time;
    const char *fname;

    size_t my_fread(void *ptr, size_t size, size_t nmemb, FILE * stream);
    void swap_Nbyte(char *data,int n,int m);
    int get_block_names(FILE *fd, char **labels, int *vflag, int *numblocks);
    int find_block(FILE *fd,const char *label);
    int read_gadget_float3(float *data,const char *label,FILE *fd);
    int read_gadget_float(float *data,const char *label,FILE *fd);
    int read_gadget_head(int *npart,double *massarr,double *time,double *redshift,FILE *fd);
    virtual void PopulateDatabaseMetaData(avtDatabaseMetaData *);
};


#endif
