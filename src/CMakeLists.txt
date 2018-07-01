cmake_minimum_required(VERSION 3.2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

QT5_WRAP_UI(UI_HEADERS
        )

set(SOURCE_FILES
        ${UI_HEADERS}
        resources/style.qrc
        main.cpp
        )

add_subdirectory(fuzzy_search)

add_executable(paste_history WIN32 ${SOURCE_FILES})
target_link_libraries(paste_history ${QT_QTMAIN_LIBRARY} fuzzy_search_lib Qt5::Network Qt5::Widgets)

IF(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    set_target_properties(paste_history PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC)")
  endif()
elseif(UNIX)
  # Nothing special required
else()
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()
