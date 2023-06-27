cmake_minimum_required (VERSION 3.15)


set(target_name "TEST__CRC")


IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()


add_executable(${target_name})

target_sources(${target_name}
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_main.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_8.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_16_ccitt_false.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_32.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_64_go_iso.cpp"
)


target_link_libraries(${target_name} PRIVATE WLIB_CRC)
target_link_libraries(${target_name} PRIVATE UT_CATCH)
add_test(${target_name} ${target_name})
