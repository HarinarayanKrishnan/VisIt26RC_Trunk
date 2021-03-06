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
#
#   Tom Fogal, Wed Aug  4 18:54:25 MDT 2010
#   Add multires operator.
#
#   Hank Childs, Sat Aug  7 03:39:33 PDT 2010
#   Add DataBinning operator.
#
#   Hank Childs, Fri Sep 24 08:16:10 PDT 2010
#   Add SurfaceNormal, Flux, and ConnectedComponents operators.
#
#   Brad Whitlock, Wed Jun 22 15:25:18 PDT 2011
#   I added the Extrude operator.
#
#   Marc Durant, Tue Sep 6 2011
#   Added the FFT operator
#
#   Hank Childs, Mon Oct 31 09:38:27 PDT 2011
#   Add ToroidalPoloidalProjection on behalf of Hari Krishnan.
#
#   Cyrus Harrison, Fri Apr  6 10:03:51 PDT 2012
#   Renamed Ftle to FTLE.
#
#   Cyrus Harrison, Tue Sep 25 12:08:29 PDT 2012
#   Added CartographicProjection (from Jean Favre)
#
#   Kathleen Biagas, Thu Oct 11 11:48:27 PDT 2012
#   Make CartographicProjection dependent upon existence of GEOVIS vtk kit.
#
#****************************************************************************/
IF(NOT WIN32)
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VISIT_PLUGIN_DIR}/operators)
ELSE(NOT WIN32)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VISIT_PLUGIN_DIR}/operators)
ENDIF(NOT WIN32)

SET(REQUIRED_OPERATOR_PLUGINS
AMRStitchCell
BoundaryOp
Box
Clip
Cone
ConnectedComponents
CoordSwap
CracksClipper
CreateBonds
Cylinder
DataBinning
Decimate
Delaunay
DeferExpression
Displace
DualMesh
Edge
Elevate
ExternalSurface
Extrude
FFT
FiveFoldTetSubdivision
Flux
FTLE
IndexSelect
InverseGhostZone
Isosurface
Isovolume
Lagrangian
Lineout
LineSampler
Merge
ModelFit
MultiresControl
OnionPeel
PDF
PersistentParticles
Project
Reflect
Replicate
Resample
Revolve
Slice
Smooth
SphereSlice
Stagger
SurfaceNormal
ThreeSlice
Threshold
ToroidalPoloidalProjection
Transform
TriangulateRegularPoints
Tube
ZoneDump
)

LIST(FIND VTK_KITS "GEOVIS" ADD_CARTOGRAPHIC)
IF(ADD_CARTOGRAPHIC GREATER "-1")
    LIST(APPEND REQUIRED_OPERATOR_PLUGINS CartographicProjection)
ENDIF()

SET(OPTIONAL_OPERATOR_PLUGINS
Context
ConnCompReduce
MetricThreshold
RemoveCells
SubdivideQuads
SurfCompPrep
)

SET(MINIMAL_OPERATOR_PLUGINS
Lineout
Slice
Threshold
)

# Select the plugins to build
IF(VISIT_BUILD_ALL_PLUGINS)
    SET(BUILD_PLUGINS ${REQUIRED_OPERATOR_PLUGINS} ${OPTIONAL_OPERATOR_PLUGINS})
ELSE(VISIT_BUILD_ALL_PLUGINS)
    IF(VISIT_BUILD_MINIMAL_PLUGINS)
        SET(BUILD_PLUGINS ${MINIMAL_OPERATOR_PLUGINS})
    ELSE(VISIT_BUILD_MINIMAL_PLUGINS)
        SET(BUILD_PLUGINS ${REQUIRED_OPERATOR_PLUGINS})
    ENDIF(VISIT_BUILD_MINIMAL_PLUGINS)
ENDIF(VISIT_BUILD_ALL_PLUGINS)

IF(HAVE_LIB_R)
  LIST(APPEND BUILD_PLUGINS ExtremeValueAnalysis)
  LIST(APPEND BUILD_PLUGINS PeaksOverThreshold)
  LIST(APPEND BUILD_PLUGINS ModelBasedClustering)
ENDIF(HAVE_LIB_R)

# Add the plugin directories
FOREACH(operator ${BUILD_PLUGINS})
    ADD_SUBDIRECTORY(${operator})
ENDFOREACH(operator)

# If we're building statically, make various main programs depend on plugins being built
IF(VISIT_STATIC)
    # Write a file containing the list of plugins such that we can include it into C++ code.
    FOREACH(operator ${BUILD_PLUGINS})
        SET(operatorCode "${operatorCode}PLUGIN_VERB(${operator})\n")
    ENDFOREACH(operator)
    FILE(WRITE ${VISIT_BINARY_DIR}/include/enabled_operators.h "${operatorCode}")

    IF(NOT VISIT_SERVER_COMPONENTS_ONLY AND NOT VISIT_ENGINE_ONLY)
        CREATE_PLUGIN_DEPENDENCIES(gui_exe "I" "Operator" ${BUILD_PLUGINS})
        CREATE_PLUGIN_DEPENDENCIES(gui_exe "G" "Operator" ${BUILD_PLUGINS})

        CREATE_PLUGIN_DEPENDENCIES(viewer_exe "I" "Operator" ${BUILD_PLUGINS})
        CREATE_PLUGIN_DEPENDENCIES(viewer_exe "V" "Operator" ${BUILD_PLUGINS})

        IF(VISIT_PYTHON_SCRIPTING)
            CREATE_PLUGIN_DEPENDENCIES(cli "I" "Operator" ${BUILD_PLUGINS})
            CREATE_PLUGIN_DEPENDENCIES(cli "S" "Operator" ${BUILD_PLUGINS})
        ENDIF(VISIT_PYTHON_SCRIPTING)
    ENDIF(NOT VISIT_SERVER_COMPONENTS_ONLY AND NOT VISIT_ENGINE_ONLY)

    CREATE_PLUGIN_DEPENDENCIES(engine_ser_exe "I" "Operator" ${BUILD_PLUGINS})
    CREATE_PLUGIN_DEPENDENCIES(engine_ser_exe "E" "Operator_ser" ${BUILD_PLUGINS})

    IF(VISIT_PARALLEL)
        CREATE_PLUGIN_DEPENDENCIES(engine_par_exe "I" "Operator" ${BUILD_PLUGINS})
        CREATE_PLUGIN_DEPENDENCIES(engine_par_exe "E" "Operator_par" ${BUILD_PLUGINS})
    ENDIF(VISIT_PARALLEL)
ENDIF(VISIT_STATIC)
