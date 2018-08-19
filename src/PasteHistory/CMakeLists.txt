cmake_minimum_required(VERSION 3.2)

project (paste_history_lib)

QT5_WRAP_UI(UI_HEADERS
        )

set(PASTE_HISTORY_SOURCE_FILES
		${UI_HEADERS}
		PasteHistoryWindow.cpp
		PasteHistoryWindow.h
		HistoryView.cpp
		HistoryView.h
		HistoryModel.cpp
		HistoryModel.h
		FuzzySearchBox.cpp
		FuzzySearchBox.h
)

add_library(paste_history_lib ${PASTE_HISTORY_SOURCE_FILES})
target_link_libraries(paste_history_lib ${QT_QTMAIN_LIBRARY} fuzzy_search_lib Qt5::Core Qt5::Widgets)
target_include_directories(paste_history_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
