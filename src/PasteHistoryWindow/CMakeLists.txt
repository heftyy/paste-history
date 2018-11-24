cmake_minimum_required(VERSION 3.8)

project (paste_history_lib)

set(CMAKE_AUTOMOC ON)

QT5_WRAP_UI(UI_HEADERS
        )

set(PASTE_HISTORY_SOURCE_FILES
		${UI_HEADERS}
		PasteHistoryWindow.cpp
		PasteHistoryWindow.h
		HistoryItem.cpp
		HistoryItem.h
		HistoryView.cpp
		HistoryView.h
		HistoryViewConstants.h
		HistoryItemModel.cpp
		HistoryItemModel.h
		HistoryItemDelegate.cpp
		HistoryItemDelegate.h
		FuzzySearchBox.cpp
		FuzzySearchBox.h
		Clipboard.cpp
		Clipboard.h
)

add_library(paste_history_window_lib ${PASTE_HISTORY_SOURCE_FILES})
target_compile_options(paste_history_window_lib PRIVATE ${COMPILE_FLAGS})
if(MSVC)
	target_compile_options(paste_history_window_lib PRIVATE /wd4127)
endif()
target_compile_features(paste_history_window_lib PUBLIC cxx_std_17)
target_link_libraries(paste_history_window_lib PRIVATE fuzzy_search_lib PUBLIC ${QT_QTMAIN_LIBRARY} Qt5::Core Qt5::Widgets Qt5::Test)
target_include_directories(paste_history_window_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_guideline_checker(paste_history_window_lib)
