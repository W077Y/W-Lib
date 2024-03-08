cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("${target_name} TESTS")
ENDIF()



add_subdirectory("./externals")
#add_subdirectory("./tst_demo")
#add_subdirectory("./tst_byte_buffer_sink")
#add_subdirectory("./tst_serializer")
#add_subdirectory("./tst_SPI_Abstraction_Interface")
#add_subdirectory("./tst_CRC")
