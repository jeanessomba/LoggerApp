cmake_minimum_required(VERSION 3.14)

project(LoggerApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(LoggerApp SHARED
  LoggerConfig.h
  LoggerConfig.cpp
  loggerapp-exports.h
)

target_include_directories(LoggerApp INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/LoggerApp>"
)
set_target_properties(LoggerApp PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/LoggerConfig.h"
)
target_link_libraries(LoggerApp Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(LoggerApp
    PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>:LOGGERAPP_API_LIBRARY>
)

include(GNUInstallDirs)

install(TARGETS LoggerApp EXPORT LoggerAppTargets RUNTIME
    PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LoggerApp
)

install(EXPORT LoggerAppTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/LoggerApp/cmake
    NAMESPACE Logging::
)

if (WIN32)
    # Run windeployqt immediately after build
    get_target_property(QMAKE_EXE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
    find_program(WINDEPLOYQT_ENV_SETUP qtenv2.bat HINTS "${QT_BIN_DIR}")
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")

    if (isMultiConfig)
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            add_custom_command(
                TARGET ${PROJECT_NAME}
                COMMENT "\n\n *** windeployqt for ${PROJECT_NAME} (${config}) ***"
                POST_BUILD
                COMMAND "${WINDEPLOYQT_ENV_SETUP}" &&
                    "${WINDEPLOYQT_EXECUTABLE}"
                        --plugindir "${CMAKE_INSTALL_PREFIX}/${config}/bin/plugins"
                        --libdir "${CMAKE_INSTALL_PREFIX}/${config}/bin"
                        # --verbose 2
                    \"$<TARGET_FILE:${PROJECT_NAME}>\"
            )
            install(
                DIRECTORY "${QT_BIN_DIR}/../plugins/platforms"
                DESTINATION "${CMAKE_INSTALL_PREFIX}/${config}/bin/plugins/"
            )
        endforeach()
    else()
        add_custom_command(
            TARGET ${PROJECT_NAME}
            COMMENT "\n\n *** windeployqt for ${PROJECT_NAME} (${CMAKE_BUILD_TYPE}) ***"
            POST_BUILD
            COMMAND "${WINDEPLOYQT_ENV_SETUP}" &&
                "${WINDEPLOYQT_EXECUTABLE}"
                    --plugindir "${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/bin/plugins"
                    --libdir "${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/bin"
                    # --verbose 2
                \"$<TARGET_FILE:${PROJECT_NAME}>\"
        )
        install(
            DIRECTORY "${QT_BIN_DIR}/../plugins/platforms"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/bin/plugins/"
        )
    endif()
endif()
