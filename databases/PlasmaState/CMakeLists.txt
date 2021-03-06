# DO NOT EDIT THIS FILE! THIS FILE IS AUTOMATICALLY GENERATED BY xml2cmake
PROJECT(PlasmaState)

INCLUDE(${VISIT_SOURCE_DIR}/CMake/PluginMacros.cmake)

SET(COMMON_SOURCES
PlasmaStatePluginInfo.C
PlasmaStateCommonPluginInfo.C
)

SET(LIBI_SOURCES 
PlasmaStatePluginInfo.C
)

SET(LIBM_SOURCES
PlasmaStateMDServerPluginInfo.C
${COMMON_SOURCES}
avtPlasmaStateFileFormat.C
)

SET(LIBE_SOURCES
PlasmaStateEnginePluginInfo.C
${COMMON_SOURCES}
avtPlasmaStateFileFormat.C
)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}
${NETCDF_INCLUDE_DIR}
${VISIT_COMMON_INCLUDES}
${VISIT_INCLUDE_DIR}/avt/DBAtts/MetaData
${VISIT_INCLUDE_DIR}/avt/DBAtts/SIL
${VISIT_INCLUDE_DIR}/avt/Database/Database
${VISIT_INCLUDE_DIR}/avt/Database/Formats
${VISIT_INCLUDE_DIR}/avt/Database/Ghost
${VISIT_INCLUDE_DIR}/avt/FileWriter
${VISIT_INCLUDE_DIR}/avt/Filters
${VISIT_INCLUDE_DIR}/avt/MIR/Base
${VISIT_INCLUDE_DIR}/avt/MIR/Tet
${VISIT_INCLUDE_DIR}/avt/MIR/Zoo
${VISIT_INCLUDE_DIR}/avt/Math
${VISIT_INCLUDE_DIR}/avt/Pipeline/AbstractFilters
${VISIT_INCLUDE_DIR}/avt/Pipeline/Data
${VISIT_INCLUDE_DIR}/avt/Pipeline/Pipeline
${VISIT_INCLUDE_DIR}/avt/Pipeline/Sinks
${VISIT_INCLUDE_DIR}/avt/Pipeline/Sources
${VISIT_INCLUDE_DIR}/avt/VisWindow/VisWindow
${VISIT_INCLUDE_DIR}/visit_vtk/full
${VISIT_INCLUDE_DIR}/visit_vtk/lightweight
${VTK_INCLUDE_DIRS} 
)

IF(WIN32)
  ADD_DEFINITIONS(-DDLL_NETCDF)
  ADD_DEFINITIONS(-DDLL_NETCDF)
ENDIF(WIN32)

LINK_DIRECTORIES(${VISIT_LIBRARY_DIR} ${VTK_LIBRARY_DIRS} ${NETCDF_LIBRARY_DIR} )

ADD_LIBRARY(IPlasmaStateDatabase ${LIBI_SOURCES})
TARGET_LINK_LIBRARIES(IPlasmaStateDatabase visitcommon)
SET(INSTALLTARGETS IPlasmaStateDatabase)

IF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)
    ADD_LIBRARY(MPlasmaStateDatabase ${LIBM_SOURCES}    )
    TARGET_LINK_LIBRARIES(MPlasmaStateDatabase visitcommon avtdbatts avtdatabase_ser ${NETCDF_CXX_LIB} ${NETCDF_LIB} )
    ADD_TARGET_DEFINITIONS(MPlasmaStateDatabase MDSERVER)
    SET(INSTALLTARGETS ${INSTALLTARGETS} MPlasmaStateDatabase)
ENDIF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)

ADD_LIBRARY(EPlasmaStateDatabase_ser ${LIBE_SOURCES})
TARGET_LINK_LIBRARIES(EPlasmaStateDatabase_ser visitcommon avtdatabase_ser avtpipeline_ser ${NETCDF_CXX_LIB} ${NETCDF_LIB} )
ADD_TARGET_DEFINITIONS(EPlasmaStateDatabase_ser ENGINE)
SET(INSTALLTARGETS ${INSTALLTARGETS} EPlasmaStateDatabase_ser)

IF(VISIT_PARALLEL)
    ADD_PARALLEL_LIBRARY(EPlasmaStateDatabase_par ${LIBE_SOURCES})
    TARGET_LINK_LIBRARIES(EPlasmaStateDatabase_par visitcommon avtdatabase_par avtpipeline_par ${NETCDF_CXX_LIB} ${NETCDF_LIB} )
    ADD_TARGET_DEFINITIONS(EPlasmaStateDatabase_par ENGINE)
    SET(INSTALLTARGETS ${INSTALLTARGETS} EPlasmaStateDatabase_par)
ENDIF(VISIT_PARALLEL)

VISIT_INSTALL_DATABASE_PLUGINS(${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_RTOD(databases ${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_FOLDER(databases PlasmaState ${INSTALLTARGETS})

