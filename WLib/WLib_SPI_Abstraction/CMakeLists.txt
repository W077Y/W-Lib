cmake_minimum_required (VERSION 3.15)

set(target_name "WLIB_SPI_ABSTRACTIONS")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_library(${target_name} STATIC)

target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/WLib_SPI_Interface.hpp

	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc/synchron_SPI_Interface.hpp
	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/synchron_SPI_Interface.cpp
	)

target_include_directories(	${target_name}
	PUBLIC  "${CMAKE_CURRENT_LIST_DIR}")

target_compile_features(${target_name} PUBLIC cxx_std_17)

target_link_libraries(${target_name} PUBLIC WLIB_UTILITY)


