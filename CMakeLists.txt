cmake_minimum_required(VERSION 3.8)
set(PROJECT_NAME_STR paste_history)

# The project version number.
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "loading $ENV{VCPKG_ROOT}")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(myproject CXX)

project(${PROJECT_NAME_STR})

set(CMAKE_VERBOSE_MAKEFILE ON)

# Force to always compile with W4 and treat warnings as errors
if(MSVC)
    set(COMPILE_FLAGS /W4 /WX)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(COMPILE_FLAGS -Wall -Werror -Wno-long-long -pedantic)
endif()

function(enable_guideline_checker target)
    if(false)
    #if(MSVC)
        set_target_properties(${target} PROPERTIES
            VS_GLOBAL_EnableCppCoreCheck true
            VS_GLOBAL_CodeAnalysisRuleSet CppCoreCheckRules.ruleset
            VS_GLOBAL_RunCodeAnalysis true)
    endif()
endfunction()

# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*;-warnings-as-errors=*")

set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

add_subdirectory(src)

enable_testing()

add_subdirectory(test)
add_subdirectory(benchmark)
add_subdirectory(doc)

if(WIN32)
    add_custom_target(Qt5CopyBinaries ALL
        # todo: check if debug and release folder exist
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/platforms

        # debug version
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll                   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Testd.dll                   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Guid.dll                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libEGLd.dll                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug

        # release version
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Core.dll                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgets.dll                 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Test.dll                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Gui.dll                     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libEGL.dll                     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release

		# release with debug info version
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Core.dll                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgets.dll                 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Test.dll                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Gui.dll                     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libEGL.dll                     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo

        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../plugins/platforms/qwindows.dll     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/platforms
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../plugins/platforms/qwindowsd.dll    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/platforms

        # Output Message
        COMMENT "Copying Qt binaries from '${Qt5Core_DIR}/../../../bin/' to '${CMAKE_BINARY_DIR}'" VERBATIM
    )
endif(WIN32)
