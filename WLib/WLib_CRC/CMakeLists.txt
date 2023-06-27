cmake_minimum_required (VERSION 3.15)

set(target_name "WLIB_CRC")


IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()



add_library(${target_name} STATIC)

# Interface
target_include_directories(${target_name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc")
target_sources(${target_name}
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/inc/WLib_CRC_Interface.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/inc/WLib_CRC_8.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/inc/WLib_CRC_16_ccitt_false.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/inc/WLib_CRC_32.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/inc/WLib_CRC_64_go_iso.hpp"
)

# Implementation
target_sources(${target_name}
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/crc_8.hpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/crc_8.cpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/crc_16_ccitt_false.hpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/crc_16_ccitt_false.cpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/crc_32.hpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/crc_32.cpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/crc_64_go_iso.hpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/src/crc_64_go_iso.cpp"
)
target_compile_features(${target_name} PUBLIC cxx_std_17)
