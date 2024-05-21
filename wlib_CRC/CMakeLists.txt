cmake_minimum_required (VERSION 3.19)

set(target_name "WLIB_CRC")
add_library(${target_name} STATIC)

# Interface
target_include_directories(${target_name}
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc"
)

target_sources(${target_name}
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/inc/wlib_CRC_interface.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/inc/wlib_CRC_8.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/inc/wlib_CRC_16_ccitt_false.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/inc/wlib_CRC_32.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/inc/wlib_CRC_64_go_iso.hpp"
)

# Implementation
target_sources(${target_name}
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/wlib_CRC_8.cpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/wlib_CRC_16_ccitt_false.cpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/wlib_CRC_32.cpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/wlib_CRC_64_go_iso.cpp"
)
