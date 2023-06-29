cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("${target_name} EXAMPLES")
ENDIF()


add_subdirectory("./demo_example")
add_subdirectory("./demo_serializer")
add_subdirectory("./demo_crc")
add_subdirectory("./demo_puplisher")


