cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("${target_name} TESTS")
ENDIF()



add_subdirectory("./externals")
add_subdirectory("./tst_demo")
