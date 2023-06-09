cmake_minimum_required(VERSION 3.5)
if (WIN32)
    set(GLUT_INCLUDE_DIR ${GLUT_ROOT_PATH}/include)

    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:\\Windows\\System32")

    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "fftw")
    set(FFTW_INCLUDE_DIRS "fftw")
    set(FFTW_LIBRARIES "fftw")
    set(FFTW_LIBRARY_DIRS "fftw")

    include_directories(${FFTW_INCLUDE_DIRS})
    link_directories(${FFTW_LIBRARY_DIRS})
endif()

project(untitled VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Charts)

find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Gui)

set(TS_FILES untitled_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        graphwidget.cpp
        graphwidget.h
        graphwidget.ui
        graphtemplate.cpp
        graphtemplate.h
        graphtemplate.ui
        aboutwidget.cpp
        aboutwidget.h
        aboutwidget.ui
        glview.cpp
        glview.h

        modellingwidget.h
        modellingwidget.cpp

        Modeling/randommodeling.h
        Modeling/randommodeling.cpp

        Modeling/superposition.h
        Modeling/superposition.cpp
        Modeling/superpositiondialog.h
        Modeling/superpositiondialog.cpp

        Utility/generaldialog.h
        Utility/generaldialog.cpp

        Utility/utility.h
        Utility/utility.cpp

        Utility/config.h
        Utility/config.cpp

        Statistics/statistics.h
        Statistics/statistics.cpp
        Statistics/statisticswindow.h
        Statistics/statisticswindow.cpp
        Statistics/statisticswidget.h
        Statistics/statisticswidget.cpp

        Parser/Parser.hpp
        Parser/ParseData.hpp
        Parser/ParseDataTxt.hpp
        Parser/ParseDataTxt.cpp
        Parser/parsedatacsv.h
        Parser/parsedatacsv.cpp

        Saver/Saver.hpp
        Saver/Saver.cpp

        Saver/saverwindow.h
        Saver/saverwindow.cpp

        Modeling/modeling.h
        Modeling/modeling.cpp
        modellingwidget.ui

        graphinfo.cpp
        graphinfo.h
        graphinfo.ui

        graphshow.cpp
        graphshow.h
        graphshow.ui
        
        Parser/ParseDataWav.cpp
        Parser/ParseDataWav.hpp

        GraphGlData/generaldata.h
        GraphGlData/generaldata.cpp

        GraphGlData/Graph2DData.hpp
        GraphGlData/Graph2DData.cpp
        GraphGlData/Graph3DData.hpp
        GraphGlData/Graph3DData.cpp

        Transformation/TransformToFourierSpectrum.hpp
        Transformation/TransformToFourierSpectrum.cpp
        Transformation/TransformToOscillogram.hpp
        Transformation/TransformToOscillogram.cpp
        Transformation/TransformToSpectrogram.hpp
        Transformation/TransformToSpectrogram.cpp
        Transformation/TransformToWaveletogram.hpp
        Transformation/TransformToWaveletogram.cpp

        analyzewidget.h
        analyzewidget.cpp
        analyzewidget.ui

        glViewType/glOscillogram.hpp
        glViewType/glOscillogram.cpp

        glViewType/glType.hpp

         glViewType/glspectrogram.h
         glViewType/glspectrogram.cpp

        glViewTemplate/gltemplateoscillogram.h
        glViewTemplate/gltemplateoscillogram.cpp
        gltemplateoscillogram.ui

        fourier.ui
        fourierDialog.h
        fourierDialog.cpp

        ${TS_FILES}
        )
        
        


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(untitled
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(untitled SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(untitled
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(untitled PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::OpenGLWidgets Qt6::Gui Qt6::Charts)
if (WIN32)
    target_link_libraries(untitled PRIVATE opengl32)
endif()

if (UNIX)
    target_link_libraries(untitled PRIVATE fftw3)
endif()

set_target_properties(untitled PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS untitled
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(untitled)
endif()
