# DO NOT EDIT THIS FILE! THIS FILE IS AUTOMATICALLY GENERATED BY xml2cmake
PROJECT(Vector)

INCLUDE(${VISIT_SOURCE_DIR}/CMake/PluginMacros.cmake)

SET(COMMON_SOURCES
VectorPluginInfo.C
VectorCommonPluginInfo.C
VectorAttributes.C
)

SET(LIBI_SOURCES 
VectorPluginInfo.C
)

SET(LIBG_SOURCES
VectorGUIPluginInfo.C
QvisVectorPlotWindow.C
${COMMON_SOURCES}
)
SET(LIBG_MOC_SOURCES
QvisVectorPlotWindow.h
)

SET(LIBV_SOURCES
VectorViewerPluginInfo.C
avtVectorPlot.C
avtVectorFilter.C
${COMMON_SOURCES}
)

SET(LIBE_SOURCES
VectorEnginePluginInfo.C
avtVectorPlot.C
avtVectorFilter.C
${COMMON_SOURCES}
)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}
${VISIT_COMMON_INCLUDES}
${VISIT_INCLUDE_DIR}/avt/DBAtts/MetaData
${VISIT_INCLUDE_DIR}/avt/DBAtts/SIL
${VISIT_INCLUDE_DIR}/avt/Database/Database
${VISIT_INCLUDE_DIR}/avt/FileWriter
${VISIT_INCLUDE_DIR}/avt/Filters
${VISIT_INCLUDE_DIR}/avt/IVP
${VISIT_INCLUDE_DIR}/avt/Math
${VISIT_INCLUDE_DIR}/avt/Pipeline/AbstractFilters
${VISIT_INCLUDE_DIR}/avt/Pipeline/Data
${VISIT_INCLUDE_DIR}/avt/Pipeline/Pipeline
${VISIT_INCLUDE_DIR}/avt/Pipeline/Sinks
${VISIT_INCLUDE_DIR}/avt/Pipeline/Sources
${VISIT_INCLUDE_DIR}/avt/Plotter
${VISIT_INCLUDE_DIR}/avt/QtVisWindow
${VISIT_INCLUDE_DIR}/avt/View
${VISIT_INCLUDE_DIR}/avt/VisWindow/Colleagues
${VISIT_INCLUDE_DIR}/avt/VisWindow/Interactors
${VISIT_INCLUDE_DIR}/avt/VisWindow/Proxies
${VISIT_INCLUDE_DIR}/avt/VisWindow/Tools
${VISIT_INCLUDE_DIR}/avt/VisWindow/VisWindow
${VISIT_INCLUDE_DIR}/gui
${VISIT_INCLUDE_DIR}/viewer/main
${VISIT_INCLUDE_DIR}/viewer/proxy
${VISIT_INCLUDE_DIR}/viewer/rpc
${VISIT_INCLUDE_DIR}/winutil
${VISIT_INCLUDE_DIR}/visit_vtk/full
${VISIT_INCLUDE_DIR}/visit_vtk/lightweight
${QT_INCLUDE_DIR}
${QT_QTCORE_INCLUDE_DIR}
${QT_QTGUI_INCLUDE_DIR}
${VTK_INCLUDE_DIRS} 
${PYTHON_INCLUDE_PATH} 
${VISIT_INCLUDE_DIR}/visitpy/visitpy 
)


LINK_DIRECTORIES(${VISIT_LIBRARY_DIR} ${QT_LIBRARY_DIR} ${GLEW_LIBRARY_DIR} ${VTK_LIBRARY_DIRS} )

ADD_LIBRARY(IVectorPlot ${LIBI_SOURCES})
TARGET_LINK_LIBRARIES(IVectorPlot visitcommon)
SET(INSTALLTARGETS IVectorPlot)

IF(NOT VISIT_SERVER_COMPONENTS_ONLY AND NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)
    QT_WRAP_CPP(GVectorPlot LIBG_SOURCES ${LIBG_MOC_SOURCES})
    ADD_LIBRARY(GVectorPlot ${LIBG_SOURCES})
    TARGET_LINK_LIBRARIES(GVectorPlot visitcommon gui )

    ADD_LIBRARY(VVectorPlot ${LIBV_SOURCES})
    TARGET_LINK_LIBRARIES(VVectorPlot visitcommon viewer )

    SET(INSTALLTARGETS ${INSTALLTARGETS} GVectorPlot VVectorPlot)

    IF(VISIT_PYTHON_SCRIPTING)
        SET(LIBS_SOURCES
            VectorScriptingPluginInfo.C
            PyVectorAttributes.C
            ${COMMON_SOURCES}
        )
        ADD_LIBRARY(SVectorPlot ${LIBS_SOURCES})
        TARGET_LINK_LIBRARIES(SVectorPlot visitcommon visitpy ${PYTHON_LIBRARY})
        SET(INSTALLTARGETS ${INSTALLTARGETS} SVectorPlot)
    ENDIF(VISIT_PYTHON_SCRIPTING)

    IF(VISIT_JAVA)
        ADD_CUSTOM_TARGET(JavaVector ALL ${CMAKE_Java_COMPILER} ${CMAKE_Java_FLAGS} -d ${VISIT_BINARY_DIR}/java -classpath ${VISIT_BINARY_DIR}/java VectorAttributes.java)
        ADD_DEPENDENCIES(JavaVector JavaClient)
    ENDIF(VISIT_JAVA)
ENDIF(NOT VISIT_SERVER_COMPONENTS_ONLY AND NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)

ADD_LIBRARY(EVectorPlot_ser ${LIBE_SOURCES})
TARGET_LINK_LIBRARIES(EVectorPlot_ser visitcommon avtplotter_ser avtpipeline_ser )
SET(INSTALLTARGETS ${INSTALLTARGETS} EVectorPlot_ser)
ADD_TARGET_DEFINITIONS(EVectorPlot_ser ENGINE)

IF(VISIT_PARALLEL)
    ADD_PARALLEL_LIBRARY(EVectorPlot_par ${LIBE_SOURCES})
    TARGET_LINK_LIBRARIES(EVectorPlot_par visitcommon avtplotter_par avtpipeline_par )
    SET(INSTALLTARGETS ${INSTALLTARGETS} EVectorPlot_par)
    ADD_TARGET_DEFINITIONS(EVectorPlot_par ENGINE)
ENDIF(VISIT_PARALLEL)

VISIT_INSTALL_PLOT_PLUGINS(${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_RTOD(plots ${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_FOLDER(plots Vector ${INSTALLTARGETS})

