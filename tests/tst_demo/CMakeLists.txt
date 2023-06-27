cmake_minimum_required (VERSION 3.15)


set(target_name "TEST__DEMO_TEST")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_executable(${target_name})

target_sources(${target_name}
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_main.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_demo.cpp"
)



target_link_libraries(${target_name} PRIVATE UT_CATCH)

add_test(${target_name} ${target_name})



