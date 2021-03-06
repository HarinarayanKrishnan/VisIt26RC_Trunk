#*****************************************************************************
#
# Copyright (c) 2000 - 2012, Lawrence Livermore National Security, LLC
# Produced at the Lawrence Livermore National Laboratory
# LLNL-CODE-442911
# All rights reserved.
#
# This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
# full copyright notice is contained in the file COPYRIGHT located at the root
# of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
#
# Redistribution  and  use  in  source  and  binary  forms,  with  or  without
# modification, are permitted provided that the following conditions are met:
#
#  - Redistributions of  source code must  retain the above  copyright notice,
#    this list of conditions and the disclaimer below.
#  - Redistributions in binary form must reproduce the above copyright notice,
#    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
#    documentation and/or other materials provided with the distribution.
#  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
#    be used to endorse or promote products derived from this software without
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
# ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
# LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
# DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
# SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
# CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
# LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
# OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
# DAMAGE.
#
# Modifications:
#    Jeremy Meredith, Thu Jan 14 12:32:46 EST 2010
#    Added Rect and Hex back to the optional plugins.
#
#    Brad Whitlock, Wed Feb 10 10:02:53 PST 2010
#    I added the contributed Velodyne plugin to the build.
#
#    Hank Childs, Sat Feb 20 09:13:11 PST 2010
#    Added Vs plugin.  Plugin has been available for a while, but we weren't
#    generating a Makefile for it.
#
#    Eric Brugger, Wed Feb 24 09:27:11 PST 2010
#    I added the paraDIS and paraDIS_tecplot plugins to the build.
#
#    Eric Brugger, Wed Feb 24 17:46:10 PST 2010
#    I temporalily removed the paraDIS and paraDIS_tecplot plugins from
#    the build since they don't build.
#
#    Eric Brugger, Fri Feb 26 10:20:38 PST 2010
#    I added back the paraDIS and paraDIS_tecplot plugins to the build since
#    they now build.
#
#    Eric Brugger, Thu Mar  4 14:31:45 PST 2010
#    I deleted the PlasmaState plugin from the build since it no longer
#    exists.
#
#    Cyrus Harrison, Thu Mar 18 17:07:55 PDT 2010
#    Temporarily removed the paraDIS due to a build issue.
#
#    Cyrus Harrison, Mon Mar 22 09:01:32 PDT 2010
#    Added back paraDIS after build issue was resolved.
#
#    Brad Whitlock, Mon Apr 26 13:35:37 PST 2010
#    I made Xdmf use the Xdmf library. I renamed our old XDMF reader to 
#    VisItXdmf and I made it get used when the Xdmf reader that uses the
#    Xdmf library is not available.
#
#    Kathleen Bonnell, Wed May 5 12:04:22 PST 2010 
#    Enable VASP reader on windows. 
#
#    Tom Fogal, Wed Aug  4 15:14:56 MDT 2010
#    Build STAR database.
#
#    Tom Fogal, Tue Aug 10 17:35:23 MDT 2010
#    Don't build STAR on Windows.
#
#    Jeremy Meredith, Thu Aug 12 13:54:57 EDT 2010
#    Added PuReMD.
#
#    Mark C. Miller, Sun Aug 29 23:35:37 PDT 2010
#    Fixed support for DBIO-ONLY build.
#
#    Brad Whitlock, Fri Oct 22 15:06:17 PDT 2010
#    I added a GMV reader.
#
#    Eric Brugger, Fri Jan  7 13:35:31 PST 2011
#    I replaced the BOXLIB2D and BOXLIB3D variables with just BOXLIB.
#
#    Brad Whitlock, Wed Feb 23 11:52:22 PST 2011
#    Add VelodyneCLE "AMR" plugin from Corvid.
#
#    Eric Brugger, Wed Mar 30 11:58:31 PDT 2011
#    I added the lata database reader contributed by CEA.
#
#    Eric Brugger, Mon Apr  4 10:05:55 PDT 2011
#    I temporarily disabled the ITAPS readers since they aren't compiling.
#
#    Mark C. Miller, Mon Apr  4 23:56:43 PDT 2011
#    Re-enabled ITAPS after fixing config-site file
#
#    Brad Whitlock, Thu May  5 10:12:18 PDT 2011
#    Add CaleHDF5 by Rob Managan.
#
#    Oliver Ruebel, Thu May 12 11:30 PDT 2011
#    I added a CUBE reader
#
#    Hank Childs, Thu Aug  4 13:22:36 PDT 2011
#    Add VCell reader on behalf of Ed Boyce and UCHC team.
#
#    Brad Whitlock, Wed Aug 10 16:19:42 PDT 2011
#    Add volimage reader for WPP code team.
#
#    Brad Whitlock, Thu Oct 20 14:01:42 PDT 2011
#    Record that the H5Part plugin can also be dependent on FASTBIT being set
#    so we can statically link.
#
#    Hank Childs, Sun Feb  5 18:52:04 PST 2012
#    Add CarpetHDF5 reader from Christian Reisswig and Erik Schnetter.
#
#    Brad Whitlock, Fri Mar  2 12:04:25 PST 2012
#    Remove BOW reader.
#
#    Brad Whitlock, Fri May 11 15:02:31 PDT 2012
#    Add BATL reader.
#
#    Eric Brugger, Tue Aug 28 16:33:00 PDT 2012
#    Add WPPImage reader.
#
#    Mark C. Miller, Mon Oct  8 18:07:48 PDT 2012
#    Added unv file reader from Olivier Cessenat
#
#    Eric Brugger, Tue Mar 19 10:45:32 PDT 2013
#    Added code to only build the paraDIS reader if boost 1.36.0 or later
#    was installed on the system.
#
#****************************************************************************/

IF(NOT WIN32)
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VISIT_PLUGIN_DIR}/databases)
ELSE(NOT WIN32)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VISIT_PLUGIN_DIR}/databases)
ENDIF(NOT WIN32)

SET(THIRDPARTY_DEPENDENT_PLUGINS)

MACRO(THIRDPARTY_DEPENDENT_PLUGINS libname targets)
    SET(alltargets ${targets})
    FOREACH (X ${ARGN})
       SET(alltargets ${alltargets} ${X})
    ENDFOREACH (X ${ARGN})
    SET(libname2 "${libname}_FOUND")
    IF(${libname2})
        IF(VISIT_STATIC)
            SET(libdir "${libname}_LIBRARY_DIR")
            SET(libs "${libname}_LIB")
            IF(${libdir})
                SET(TMP_THIRDPARTY_IO_LIBRARY_DIR ${TMP_THIRDPARTY_IO_LIBRARY_DIR} ${${libdir}})
            ENDIF(${libdir})
            IF(${libs})
                FOREACH(lib ${${libs}})
                    LIST(FIND TMP_THIRDPARTY_IO_LIB ${lib} findVar)
                    IF(findVar EQUAL -1)
                        SET(TMP_THIRDPARTY_IO_LIB ${TMP_THIRDPARTY_IO_LIB} ${lib})
                    ENDIF(findVar EQUAL -1)
                ENDFOREACH(lib)
            ENDIF(${libs})
        ENDIF(VISIT_STATIC)
        # Add each target individually so we can check if it is already in the list. It might
        # be in the list already if it might depend on multiple libraries (H5PART, FASTBIT)
        FOREACH(target ${alltargets})
            LIST(FIND THIRDPARTY_DEPENDENT_PLUGINS ${target} findVar)
            IF(findVar EQUAL -1)
                SET(THIRDPARTY_DEPENDENT_PLUGINS ${THIRDPARTY_DEPENDENT_PLUGINS} ${target})
            ENDIF(findVar EQUAL -1)
        ENDFOREACH(target)
    ENDIF(${libname2})
ENDMACRO(THIRDPARTY_DEPENDENT_PLUGINS libname targets)

# These are the various targets that are optional since they depend on 3rd-
# party I/O libraries to be present. We pass the name of the 3rd party library
# to THIRDPARTY_DEPENDENT_PLUGINS and it checks whether there is a HAVE_XXXX
# symbol defined for that library. If so, the targets that use that 3rd party
# library are added to the mix.
UNSET(TMP_THIRDPARTY_IO_LIBRARY_DIR)
UNSET(TMP_THIRDPARTY_IO_LIB)
THIRDPARTY_DEPENDENT_PLUGINS(ADIOS    ADIOS)
THIRDPARTY_DEPENDENT_PLUGINS(ADVIO    Adventure)
THIRDPARTY_DEPENDENT_PLUGINS(BOXLIB   Boxlib2D Boxlib3D)
THIRDPARTY_DEPENDENT_PLUGINS(CCMIO    CCM)
THIRDPARTY_DEPENDENT_PLUGINS(CFITSIO  FITS)
THIRDPARTY_DEPENDENT_PLUGINS(CGNS     CGNS)
THIRDPARTY_DEPENDENT_PLUGINS(EXODUSII Exodus TimeVaryingExodus)
THIRDPARTY_DEPENDENT_PLUGINS(GDAL     GDAL)
THIRDPARTY_DEPENDENT_PLUGINS(HDF4     Cosmos ZeusMP)
IF(NOT XDMF_FOUND)
    SET(XDMF_READER VisItXdmf)
ENDIF(NOT XDMF_FOUND)
THIRDPARTY_DEPENDENT_PLUGINS(HDF5     
AMR
BATL
CaleHDF5
CarpetHDF5
Chombo
CosmosPP
Enzo
FLASH
Geqdsk
GTC
H5Nimrod
M3D
M3DC1
PFLOTRAN
Pixie
SAMRAI
SXRIS
Tetrad
UNIC
Velodyne
Vs
${XDMF_READER}
)
THIRDPARTY_DEPENDENT_PLUGINS(MDSPLUS  MDSplus)
THIRDPARTY_DEPENDENT_PLUGINS(FASTBIT  H5Part)
THIRDPARTY_DEPENDENT_PLUGINS(H5PART   H5Part)
THIRDPARTY_DEPENDENT_PLUGINS(MILI     Mili)
THIRDPARTY_DEPENDENT_PLUGINS(NETCDF   NETCDF S3D)
THIRDPARTY_DEPENDENT_PLUGINS(NETCDF_CXX  MFIXCDF)
IF (NOT WIN32)
    THIRDPARTY_DEPENDENT_PLUGINS(SILO     Cale KullLite PDB Silo SAMI Vista)
ELSE (NOT WIN32)
    THIRDPARTY_DEPENDENT_PLUGINS(SILO     Cale KullLite PDB Silo SAMI)
ENDIF (NOT WIN32)
THIRDPARTY_DEPENDENT_PLUGINS(VISUS    ViSUS)
THIRDPARTY_DEPENDENT_PLUGINS(XDMF     Xdmf)
IF (WIN32)
    THIRDPARTY_DEPENDENT_PLUGINS(ZLIB     VCellMTMD)
ENDIF (WIN32)

# Add ITAPS
IF(ITAPS_DIRS)
    SET(THIRDPARTY_DEPENDENT_PLUGINS ${THIRDPARTY_DEPENDENT_PLUGINS} ${ITAPS_DIRS})
    IF(VISIT_STATIC)
        FOREACH(impl ${ITAPS_DIRS})
            SET(itapslib "${impl}_LIB")
            IF(${itapslib})
                SET(TMP_THIRDPARTY_IO_LIB ${TMP_THIRDPARTY_IO_LIB} ${${itapslib}})
            ENDIF(${itapslib})
            SET(itapslibdir "${impl}_LIBRARY_DIR")
            IF(${itapslibdir})
                SET(TMP_THIRDPARTY_IO_LIBRARY_DIR ${TMP_THIRDPARTY_IO_LIBRARY_DIR} ${${itapslibdir}})
            ENDIF(${itapslibdir})
        ENDFOREACH(impl)
    ENDIF(VISIT_STATIC)
ENDIF(ITAPS_DIRS)

# Save the list of all library dirs and libs so the engine can use it
IF(VISIT_STATIC)
    SET(ALL_THIRDPARTY_IO_LIBRARY_DIR ${TMP_THIRDPARTY_IO_LIBRARY_DIR} CACHE INTERNAL "List of all I/O lib dirs")
    SET(ALL_THIRDPARTY_IO_LIB ${TMP_THIRDPARTY_IO_LIB} CACHE INTERNAL "List of all I/O libs")
ENDIF(VISIT_STATIC)

# These are our required plugins (no external dependencies)
SET(REQUIRED_DATABASE_PLUGINS
ANALYZE
ANSYS
AUXFile
AugDecomp
BOV
CEAucd
CMAT
CTRL
Cube
Curve2D
DDCMD
Dyna3D
EnSight
ExtrudedVol
Fluent
FT2
GGCM
GMV
GULP
Gadget
Image
LAMMPS
Lines
MFIX
MM5
MatrixMarket
Miranda
NASTRAN
Nek5000
OVERFLOW
OpenFOAM
PATRAN
PuReMD
PLOT2D
PLOT3D
PlainText
Point3D
ProteinDataBank
RAW
SAR
SAS
STL
Shapefile
SimV2
Spheral
TFT
TSurf
Tecplot
VASP
VTK
WPPImage
WavefrontOBJ
XSF
XYZ
Xmdv
lata
volimage
unv
)

IF (Boost_USABLE_PARADIS)
    LIST(APPEND REQUIRED_DATABASE_PLUGINS
        paraDIS
        paraDIS_tecplot
    )
ENDIF (Boost_USABLE_PARADIS)

IF (NOT WIN32)
    LIST(APPEND REQUIRED_DATABASE_PLUGINS 
        Claw
        STAR
        VCellMTMD
        Vis5D
        ZipWrapper
    )
    IF(NOT VISIT_STATIC AND NOT VISIT_DBIO_ONLY)
        LIST(APPEND REQUIRED_DATABASE_PLUGINS SimV1 SimV1Writer)
    ENDIF(NOT VISIT_STATIC AND NOT VISIT_DBIO_ONLY)
    SET(OPTIONAL_DATABASE_PLUGINS
        Hex
        Rect
        PICS_Tester
        VLI
    )
ENDIF (NOT WIN32)


SET(MINIMAL_DATABASE_PLUGINS
Curve2D
RAW
VTK
)

# Select the plugins to build
IF(VISIT_BUILD_ALL_PLUGINS)
    SET(BUILD_PLUGINS ${REQUIRED_DATABASE_PLUGINS} ${THIRDPARTY_DEPENDENT_PLUGINS} ${OPTIONAL_DATABASE_PLUGINS})
ELSE(VISIT_BUILD_ALL_PLUGINS)
    IF(VISIT_BUILD_MINIMAL_PLUGINS)
        SET(BUILD_PLUGINS ${MINIMAL_DATABASE_PLUGINS})
    ELSE(VISIT_BUILD_MINIMAL_PLUGINS)
        SET(BUILD_PLUGINS ${REQUIRED_DATABASE_PLUGINS} ${THIRDPARTY_DEPENDENT_PLUGINS})
    ENDIF(VISIT_BUILD_MINIMAL_PLUGINS)
ENDIF(VISIT_BUILD_ALL_PLUGINS)

# Add the plugin directories
FOREACH(db ${BUILD_PLUGINS})
    ADD_SUBDIRECTORY(${db})
ENDFOREACH(db)

# If we're building statically, make various main programs depend on plugins being built
IF(VISIT_STATIC)
    # Write a file containing the list of plugins such that we can include it into C++ code.
    FOREACH(db ${BUILD_PLUGINS})
        SET(dbCode "${dbCode}PLUGIN_VERB(${db})\n")
    ENDFOREACH(db)
    FILE(WRITE ${VISIT_BINARY_DIR}/include/enabled_databases.h "${dbCode}")

    IF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)
        CREATE_PLUGIN_DEPENDENCIES(mdserver "I" "Database" ${BUILD_PLUGINS})
        CREATE_PLUGIN_DEPENDENCIES(mdserver "M" "Database" ${BUILD_PLUGINS})
    ENDIF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)
    CREATE_PLUGIN_DEPENDENCIES(engine_ser_exe "I" "Database" ${BUILD_PLUGINS})
    CREATE_PLUGIN_DEPENDENCIES(engine_ser_exe "E" "Database_ser" ${BUILD_PLUGINS})
    IF(VISIT_PARALLEL)
        CREATE_PLUGIN_DEPENDENCIES(engine_par_exe "I" "Database" ${BUILD_PLUGINS})
        CREATE_PLUGIN_DEPENDENCIES(engine_par_exe "E" "Database_par" ${BUILD_PLUGINS})
    ENDIF(VISIT_PARALLEL)
ENDIF(VISIT_STATIC)
