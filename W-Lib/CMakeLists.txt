cmake_minimum_required (VERSION 3.15)


set(target_name "W-LIB")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_subdirectory("./W-Lib_Utility")



