cmake_minimum_required(VERSION 3.8)

set(SOURCE_FILES
		resources/res.qrc
		main.cpp
		)

set(CMAKE_AUTORCC ON)

find_path(GSL_INCLUDE_DIR gsl/gsl)
if(GSL_INCLUDE_DIR STREQUAL "GSL_INCLUDE_DIR-NOTFOUND")
	message(FATAL_ERROR "Could not find Guidelines Support Library.")
endif()

add_subdirectory(Config)
add_subdirectory(FuzzySearch)
#add_subdirectory(SearchableHistory)
add_subdirectory(PasteHistoryWindow)
#add_subdirectory(HistoryTypes)

add_executable(paste_history WIN32 ${SOURCE_FILES})
target_compile_options(paste_history PRIVATE ${COMPILE_FLAGS})
target_compile_features(paste_history PRIVATE cxx_std_17)
target_link_libraries(paste_history PRIVATE paste_history_window_lib)

enable_guideline_checker(paste_history)

IF(WIN32) # Check if we are on Windows
	if(MSVC) # Check if we are using the Visual Studio compiler
		set_target_properties(paste_history PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
		set_target_properties(paste_history PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /DEBUG:FASTLINK")
	else()
		message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC)")
	endif()
elseif(UNIX)
  # Nothing special required
else()
	message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()
