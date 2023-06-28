cmake_minimum_required (VERSION 3.15)

set(target_name "WLIB_CALLBACK_INTERFACE")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_library(${target_name} STATIC)

target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/WLib_Callback_Interface.hpp

	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc/Callback_Interface_utility.hpp
	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc/Memberfunction_Callback.hpp
	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/WLib_Callback_Interface.cpp
	)

target_include_directories(	${target_name}
	PUBLIC  "${CMAKE_CURRENT_LIST_DIR}")

target_compile_features(${target_name} PUBLIC cxx_std_17)

