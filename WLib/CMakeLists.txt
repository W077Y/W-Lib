cmake_minimum_required (VERSION 3.15)


set(target_name "WLIB")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_subdirectory("./WLib_Byte_Sink")
add_subdirectory("./WLib_Serializer")
add_subdirectory("./WLib_SPI_Abstraction")
add_subdirectory("./WLib_CRC")
add_subdirectory("./WLib_Utility")



