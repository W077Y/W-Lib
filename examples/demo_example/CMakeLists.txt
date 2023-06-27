cmake_minimum_required (VERSION 3.15)



set(target_name "EXA__DEMO_EXAMPLE")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_executable(${target_name})

target_sources(${target_name}
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/exa_demo.cpp"
)

add_test(${target_name} ${target_name})



