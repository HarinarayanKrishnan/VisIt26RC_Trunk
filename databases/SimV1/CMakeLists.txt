# DO NOT EDIT THIS FILE! THIS FILE IS AUTOMATICALLY GENERATED BY xml2cmake
PROJECT(SimV1)

INCLUDE(${VISIT_SOURCE_DIR}/CMake/PluginMacros.cmake)

SET(COMMON_SOURCES
SimV1PluginInfo.C
SimV1CommonPluginInfo.C
)

SET(LIBI_SOURCES 
SimV1PluginInfo.C
)

SET(LIBM_SOURCES
SimV1MDServerPluginInfo.C
${COMMON_SOURCES}
avtSimV1FileFormat.C
)

SET(LIBE_SOURCES
SimV1EnginePluginInfo.C
${COMMON_SOURCES}
avtSimV1FileFormat.C
)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}
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


LINK_DIRECTORIES(${VISIT_LIBRARY_DIR} ${VTK_LIBRARY_DIRS} ${ICET_LIBRARY_DIR} )

ADD_LIBRARY(ISimV1Database ${LIBI_SOURCES})
TARGET_LINK_LIBRARIES(ISimV1Database visitcommon)
SET(INSTALLTARGETS ISimV1Database)

IF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)
    ADD_LIBRARY(MSimV1Database ${LIBM_SOURCES}    )
    TARGET_LINK_LIBRARIES(MSimV1Database visitcommon avtdbatts avtdatabase_ser )
    ADD_TARGET_DEFINITIONS(MSimV1Database MDSERVER)
    SET(INSTALLTARGETS ${INSTALLTARGETS} MSimV1Database)
ENDIF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)

ADD_LIBRARY(ESimV1Database_ser ${LIBE_SOURCES})
TARGET_LINK_LIBRARIES(ESimV1Database_ser visitcommon avtdatabase_ser avtpipeline_ser )
ADD_TARGET_DEFINITIONS(ESimV1Database_ser ENGINE)
SET(INSTALLTARGETS ${INSTALLTARGETS} ESimV1Database_ser)

IF(VISIT_PARALLEL)
    ADD_PARALLEL_LIBRARY(ESimV1Database_par ${LIBE_SOURCES})
    TARGET_LINK_LIBRARIES(ESimV1Database_par visitcommon avtdatabase_par avtpipeline_par )
    ADD_TARGET_DEFINITIONS(ESimV1Database_par ENGINE)
    SET(INSTALLTARGETS ${INSTALLTARGETS} ESimV1Database_par)
ENDIF(VISIT_PARALLEL)

VISIT_INSTALL_DATABASE_PLUGINS(${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_RTOD(databases ${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_FOLDER(databases SimV1 ${INSTALLTARGETS})

