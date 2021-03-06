c-----------------------------------------------------------------------------
c
c Copyright (c) 2000 - 2012, Lawrence Livermore National Security, LLC
c Produced at the Lawrence Livermore National Laboratory
c LLNL-CODE-442911
c All rights reserved.
c
c This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
c full copyright notice is contained in the file COPYRIGHT located at the root
c of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
c
c Redistribution  and  use  in  source  and  binary  forms,  with  or  without
c modification, are permitted provided that the following conditions are met:
c
c  - Redistributions of  source code must  retain the above  copyright notice,
c    this list of conditions and the disclaimer below.
c  - Redistributions in binary form must reproduce the above copyright notice,
c    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
c    documentation and/or other materials provided with the distribution.
c  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
c    be used to endorse or promote products derived from this software without
c    specific prior written permission.
c
c THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
c AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
c IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
c ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
c LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
c DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
c DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
c SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
c CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
c LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
c OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
c DAMAGE.
c
c-----------------------------------------------------------------------------

      program main
      implicit none
      include "silo.inc"
      integer dbfile, ierr

c Create the Silo file
      ierr = dbcreate("fvectorvar.silo", 15, DB_CLOBBER, DB_LOCAL, 
     .                "Vector quadvars in 3D", 21, DB_HDF5, dbfile)
      if(dbfile.eq.-1) then
          write (6,*) 'Could not create Silo file!\n'
          goto 10000
      endif
c Add other Silo calls here.
      call write_curv3d(dbfile)
      call write_zonecent_quadvar(dbfile)
      call write_nodecent_quadvar(dbfile)
      call write_defvars(dbfile)
c Close the Silo file.
      ierr = dbclose(dbfile)
10000 stop
      end

      subroutine write_curv3d(dbfile)
      implicit none
      integer dbfile
      include "silo.inc"
      integer err, ierr, dims(3), ndims, ZX,ZY,ZZ,NX,NY,NZ
      parameter (NX = 4)
      parameter (NY = 3)
      parameter (NZ = 2)
      parameter (ZX = NX-1)
      parameter (ZY = NY-1)
      parameter (ZZ = NZ-1)
      real x(NX,NY,NZ), y(NX,NY,NZ), z(NX,NY,NZ)
      data x/0.,1.,2.,3., 0.,1.,2.,3., 0.,1.,2.,3.,
     . 0.,1.,2.,3., 0.,1.,2.,3., 0.,1.,2.,3./
      data y/0.5,0.,0.,0.5, 1.,1.,1.,1., 1.5,2.,2.,1.5,
     . 0.5,0.,0.,0.5, 1.,1.,1.,1., 1.5,2.,2.,1.5/
      data z/0.,0.,0.,0., 0.,0.,0.,0., 0.,0.,0.,0,
     . 1.,1.,1.,1., 1.,1.,1.,1., 1.,1.,1.,1./
      ndims = 3
      data dims/4, 3, 2/
      err = dbputqm (dbfile, "quadmesh", 8, "xc", 2, 
     . "yc", 2, "zc", 2, x, y, z, dims, ndims, 
     . DB_FLOAT, DB_NONCOLLINEAR, DB_F77NULL, ierr)
      end

      subroutine write_zonecent_quadvar(dbfile)
      implicit none
      integer dbfile
      include "silo.inc"
      integer err, ierr, dims(3), ndims,i,j,k,index,ZX,ZY,ZZ,NX,NY,NZ
      parameter (NX = 4)
      parameter (NY = 3)
      parameter (NZ = 2)
      parameter (ZX = NX-1)
      parameter (ZY = NY-1)
      parameter (ZZ = NZ-1)
      integer    comp0(ZX,ZY,ZZ), comp1(ZX,ZY,ZZ), comp2(ZX,ZY,ZZ)
      data dims/ZX,ZY,ZZ/
      index = 0
      do 10020 k=1,ZZ
      do 10010 j=1,ZY
      do 10000 i=1,ZX
          comp0(i,j,k) = index
          comp1(i,j,k) = index
          comp2(i,j,k) = index
          index = index + 1
10000 continue
10010 continue
10020 continue
      ndims = 3
      err = dbputqv1(dbfile, "zonal_comp0", 11, "quadmesh", 8, comp0, 
     . dims, ndims, DB_F77NULL, 0, DB_INT, DB_ZONECENT, DB_F77NULL, 
     . ierr)
      err = dbputqv1(dbfile, "zonal_comp1", 11, "quadmesh", 8, comp1,
     . dims, ndims, DB_F77NULL, 0, DB_INT, DB_ZONECENT, DB_F77NULL,
     . ierr)
      err = dbputqv1(dbfile, "zonal_comp2", 11, "quadmesh", 8, comp2,
     . dims, ndims, DB_F77NULL, 0, DB_INT, DB_ZONECENT, DB_F77NULL,
     . ierr)
      end

      subroutine write_nodecent_quadvar(dbfile)
      implicit none
      integer dbfile
      include "silo.inc"
      integer err, ierr, dims(3), ndims,i,j,k,index,ZX,ZY,ZZ,NX,NY,NZ
      parameter (NX = 4)
      parameter (NY = 3)
      parameter (NZ = 2)
      parameter (ZX = NX-1)
      parameter (ZY = NY-1)
      parameter (ZZ = NZ-1)
      real    comp0(NX,NY,NZ), comp1(NX,NY,NZ), comp2(NX,NY,NZ)
      data dims/NX,NY,NZ/
      index = 0
      do 20020 k=1,NZ
      do 20010 j=1,NY
      do 20000 i=1,NX
          comp0(i,j,k) = float(index)
          comp1(i,j,k) = float(index)
          comp2(i,j,k) = float(index)
          index = index + 1
20000 continue
20010 continue
20020 continue
      ndims = 3
      err = dbputqv1(dbfile, "nodal_comp0", 11, "quadmesh", 8, comp0,
     . dims, ndims, DB_F77NULL, 0, DB_FLOAT, DB_NODECENT, DB_F77NULL,
     . ierr)
      err = dbputqv1(dbfile, "nodal_comp1", 11, "quadmesh", 8, comp1,
     . dims, ndims, DB_F77NULL, 0, DB_FLOAT, DB_NODECENT, DB_F77NULL,
     . ierr)
      err = dbputqv1(dbfile, "nodal_comp2", 11, "quadmesh", 8, comp2,
     . dims, ndims, DB_F77NULL, 0, DB_FLOAT, DB_NODECENT, DB_F77NULL,
     . ierr)
      end

      subroutine write_defvars(dbfile)
            implicit none
      integer dbfile
      include "silo.inc"
      integer     err, ierr, types(2), lnames(2), ldefs(2), oldlen
      integer     optlists(2)
c Initialize some 20 character length strings
      character*40 names(2) /'zonalvec                                ',
     .                       'nodalvec                                '/
      character*40 defs(2)  /'{zonal_comp0,zonal_comp1,zonal_comp2}   ',
     .                       '{nodal_comp0,nodal_comp1,nodal_comp2}   '/
c Store the length of each string
      data lnames/8, 8/
      data ldefs/37, 37/
      data types/DB_VARTYPE_VECTOR, DB_VARTYPE_VECTOR/
c Store NULL in the option lists for dbputdefvars
      data optlists/DB_F77NULL, DB_F77NULL/
c Set the maximum string length to 40 since that's how long our strings are
      oldlen = dbget2dstrlen()
      err = dbset2dstrlen(40)
c Write out the expressions
      err = dbputdefvars(dbfile, "defvars", 7, 2, names, lnames, types,
     . defs, ldefs, optlists, ierr)
c Restore the previous value for maximum string length
      err = dbset2dstrlen(oldlen) 
      end
