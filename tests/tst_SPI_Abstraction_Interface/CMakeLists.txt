cmake_minimum_required (VERSION 3.15)


set(target_name "TEST__SPI_ABSTRACTION_INTERFACE_TEST")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_executable(${target_name})

target_sources(${target_name}
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_main.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_SPI_Abstraction_Interface.cpp"
)

target_link_libraries(${target_name} PRIVATE UT_CATCH)

target_link_libraries(${target_name} PUBLIC WLIB_SPI_ABSTRACTIONS)

add_test(${target_name} ${target_name})



