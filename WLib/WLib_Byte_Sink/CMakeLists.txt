cmake_minimum_required (VERSION 3.15)

set(target_name "WLIB_BYTE_SINK")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_library(${target_name} STATIC)

target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_Byte_Sink_Concept.hpp
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_Byte_Buffer_Sink.hpp
	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/WLib_Byte_Sink.cpp
	)

target_include_directories(	${target_name}
	PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc")


target_compile_features(${target_name} PUBLIC cxx_std_17)


