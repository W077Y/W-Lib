cmake_minimum_required (VERSION 3.15)

set(target_name "W-LIB_UTILITY")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_library(${target_name} STATIC)

target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/W-Lib_Utility.hpp

	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/W-Lib_utility.cpp
	)

target_include_directories(	${target_name}
	PUBLIC  "${CMAKE_CURRENT_LIST_DIR}")


target_compile_features(${target_name} PUBLIC cxx_std_17)


