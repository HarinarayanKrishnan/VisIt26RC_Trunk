# DO NOT EDIT THIS FILE! THIS FILE IS AUTOMATICALLY GENERATED BY xml2cmake
PROJECT(DeformSphereGlyph)

INCLUDE(${VISIT_SOURCE_DIR}/CMake/PluginMacros.cmake)

SET(COMMON_SOURCES
DeformSphereGlyphPluginInfo.C
DeformSphereGlyphCommonPluginInfo.C
DeformSphereGlyphAttributes.C
)

SET(LIBI_SOURCES 
DeformSphereGlyphPluginInfo.C
)

SET(LIBG_SOURCES
DeformSphereGlyphGUIPluginInfo.C
QvisDeformSphereGlyphWindow.C
${COMMON_SOURCES}
)
SET(LIBG_MOC_SOURCES
QvisDeformSphereGlyphWindow.h
)

SET(LIBV_SOURCES
DeformSphereGlyphViewerPluginInfo.C
${COMMON_SOURCES}
)

SET(LIBE_SOURCES
DeformSphereGlyphEnginePluginInfo.C
avtDeformSphereGlyphFilter.C
${COMMON_SOURCES}
)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}
${VISIT_COMMON_INCLUDES}
${VISIT_INCLUDE_DIR}/avt/DBAtts/MetaData
${VISIT_INCLUDE_DIR}/avt/DBAtts/SIL
${VISIT_INCLUDE_DIR}/avt/Database/Database
${VISIT_INCLUDE_DIR}/avt/Expressions/Abstract
${VISIT_INCLUDE_DIR}/avt/Expressions/CMFE
${VISIT_INCLUDE_DIR}/avt/Expressions/Conditional
${VISIT_INCLUDE_DIR}/avt/Expressions/Derivations
${VISIT_INCLUDE_DIR}/avt/Expressions/General
${VISIT_INCLUDE_DIR}/avt/Expressions/ImageProcessing
${VISIT_INCLUDE_DIR}/avt/Expressions/Management
${VISIT_INCLUDE_DIR}/avt/Expressions/Math
${VISIT_INCLUDE_DIR}/avt/Expressions/MeshQuality
${VISIT_INCLUDE_DIR}/avt/Expressions/TimeIterators
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
${VISIT_INCLUDE_DIR}/mdserver/proxy
${VISIT_INCLUDE_DIR}/mdserver/rpc
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

ADD_LIBRARY(IDeformSphereGlyphOperator ${LIBI_SOURCES})
TARGET_LINK_LIBRARIES(IDeformSphereGlyphOperator visitcommon)
SET(INSTALLTARGETS IDeformSphereGlyphOperator)

IF(NOT VISIT_SERVER_COMPONENTS_ONLY AND NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)
    QT_WRAP_CPP(GDeformSphereGlyphOperator LIBG_SOURCES ${LIBG_MOC_SOURCES})
    ADD_LIBRARY(GDeformSphereGlyphOperator ${LIBG_SOURCES})
    TARGET_LINK_LIBRARIES(GDeformSphereGlyphOperator visitcommon gui )

    ADD_LIBRARY(VDeformSphereGlyphOperator ${LIBV_SOURCES})
    TARGET_LINK_LIBRARIES(VDeformSphereGlyphOperator visitcommon viewer )
    SET(INSTALLTARGETS ${INSTALLTARGETS} GDeformSphereGlyphOperator VDeformSphereGlyphOperator)

    IF(VISIT_PYTHON_SCRIPTING)
        SET(LIBS_SOURCES
            DeformSphereGlyphScriptingPluginInfo.C
            PyDeformSphereGlyphAttributes.C
            ${COMMON_SOURCES}
        )
        ADD_LIBRARY(SDeformSphereGlyphOperator ${LIBS_SOURCES})
        TARGET_LINK_LIBRARIES(SDeformSphereGlyphOperator visitcommon visitpy ${PYTHON_LIBRARY})
        SET(INSTALLTARGETS ${INSTALLTARGETS} SDeformSphereGlyphOperator)
    ENDIF(VISIT_PYTHON_SCRIPTING)

    IF(VISIT_JAVA)
        ADD_CUSTOM_TARGET(JavaDeformSphereGlyph ALL ${CMAKE_Java_COMPILER} ${CMAKE_Java_FLAGS} -d ${VISIT_BINARY_DIR}/java -classpath ${VISIT_BINARY_DIR}/java DeformSphereGlyphAttributes.java)
        ADD_DEPENDENCIES(JavaDeformSphereGlyph JavaClient)
    ENDIF(VISIT_JAVA)
ENDIF(NOT VISIT_SERVER_COMPONENTS_ONLY AND NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)

ADD_LIBRARY(EDeformSphereGlyphOperator_ser ${LIBE_SOURCES})
TARGET_LINK_LIBRARIES(EDeformSphereGlyphOperator_ser visitcommon avtexpressions_ser avtfilters_ser avtpipeline_ser )
SET(INSTALLTARGETS ${INSTALLTARGETS} EDeformSphereGlyphOperator_ser)

IF(VISIT_PARALLEL)
    ADD_PARALLEL_LIBRARY(EDeformSphereGlyphOperator_par ${LIBE_SOURCES})
    TARGET_LINK_LIBRARIES(EDeformSphereGlyphOperator_par visitcommon avtexpressions_par avtfilters_par avtpipeline_par )
    SET(INSTALLTARGETS ${INSTALLTARGETS} EDeformSphereGlyphOperator_par)
ENDIF(VISIT_PARALLEL)

VISIT_INSTALL_OPERATOR_PLUGINS(${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_RTOD(operators ${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_FOLDER(operators DeformSphereGlyph ${INSTALLTARGETS})

