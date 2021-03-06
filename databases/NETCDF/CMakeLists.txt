# DO NOT EDIT THIS FILE! THIS FILE IS AUTOMATICALLY GENERATED BY xml2cmake
PROJECT(NETCDF)

INCLUDE(${VISIT_SOURCE_DIR}/CMake/PluginMacros.cmake)

SET(COMMON_SOURCES
NETCDFPluginInfo.C
NETCDFCommonPluginInfo.C
)

SET(LIBI_SOURCES 
NETCDFPluginInfo.C
)

SET(LIBM_SOURCES
NETCDFMDServerPluginInfo.C
${COMMON_SOURCES}
avtNETCDFFileFormat.C
avtBasicNETCDFFileFormat.C
avtADAPTFileFormat.C
avtLODIFileFormat.C
avtLODIParticleFileFormat.C
avtFVCOM_STSDFileFormat.C
avtFVCOMReader.C
avtFVCOMParticleFileFormat.C
avtFVCOM_MTSDFileFormat.C
avtFVCOM_MTMDFileFormat.C
avtCCSMFileFormat.C
avtCCSMReader.C
avtNETCDFReaderBase.C
avtBasicNETCDFReader.C
NETCDFFileObject.C
)

SET(LIBE_SOURCES
NETCDFEnginePluginInfo.C
${COMMON_SOURCES}
avtNETCDFFileFormat.C
avtBasicNETCDFFileFormat.C
avtADAPTFileFormat.C
avtLODIFileFormat.C
avtLODIParticleFileFormat.C
avtFVCOM_STSDFileFormat.C
avtFVCOMReader.C
avtFVCOMParticleFileFormat.C
avtFVCOM_MTSDFileFormat.C
avtFVCOM_MTMDFileFormat.C
avtCCSMFileFormat.C
avtCCSMReader.C
avtNETCDFReaderBase.C
avtBasicNETCDFReader.C
NETCDFFileObject.C
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
ENDIF(WIN32)

LINK_DIRECTORIES(${VISIT_LIBRARY_DIR} ${VTK_LIBRARY_DIRS} ${NETCDF_LIBRARY_DIR} )

ADD_LIBRARY(INETCDFDatabase ${LIBI_SOURCES})
TARGET_LINK_LIBRARIES(INETCDFDatabase visitcommon)
SET(INSTALLTARGETS INETCDFDatabase)

IF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)
    ADD_LIBRARY(MNETCDFDatabase ${LIBM_SOURCES}    )
    TARGET_LINK_LIBRARIES(MNETCDFDatabase visitcommon avtdbatts avtdatabase_ser ${NETCDF_LIB} )
    ADD_TARGET_DEFINITIONS(MNETCDFDatabase MDSERVER)
    SET(INSTALLTARGETS ${INSTALLTARGETS} MNETCDFDatabase)
ENDIF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)

ADD_LIBRARY(ENETCDFDatabase_ser ${LIBE_SOURCES})
TARGET_LINK_LIBRARIES(ENETCDFDatabase_ser visitcommon avtdatabase_ser avtpipeline_ser ${NETCDF_LIB} )
ADD_TARGET_DEFINITIONS(ENETCDFDatabase_ser ENGINE)
SET(INSTALLTARGETS ${INSTALLTARGETS} ENETCDFDatabase_ser)

IF(VISIT_PARALLEL)
    ADD_PARALLEL_LIBRARY(ENETCDFDatabase_par ${LIBE_SOURCES})
    TARGET_LINK_LIBRARIES(ENETCDFDatabase_par visitcommon avtdatabase_par avtpipeline_par ${NETCDF_LIB} )
    ADD_TARGET_DEFINITIONS(ENETCDFDatabase_par ENGINE)
    SET(INSTALLTARGETS ${INSTALLTARGETS} ENETCDFDatabase_par)
ENDIF(VISIT_PARALLEL)

VISIT_INSTALL_DATABASE_PLUGINS(${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_RTOD(databases ${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_FOLDER(databases NETCDF ${INSTALLTARGETS})

