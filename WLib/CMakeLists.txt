cmake_minimum_required (VERSION 3.15)


set(target_name "WLIB")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

#add_subdirectory("./WLib_Byte_Sink")
#add_subdirectory("./WLib_Serializer")
#add_subdirectory("./WLib_SPI_Abstraction")
#add_subdirectory("./WLib_CRC")


add_library(${target_name} STATIC)

target_include_directories(	${target_name}
	PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc")


message(STATUS "${sub_dir_tree_val}Callback_Interface")
target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_Callback_Interface.hpp
	)
	
message(STATUS "${sub_dir_tree_val}Container")
target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_SPSC.hpp
	)

message(STATUS "${sub_dir_tree_val}Utility")
target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_Utility.hpp
	)


target_sources(${target_name}
	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/WLib.cpp
	)

target_compile_features(${target_name} PUBLIC cxx_std_20)





