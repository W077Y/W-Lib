cmake_minimum_required (VERSION 3.15)



set(target_name "EXA__PUPLISHER")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_executable(${target_name})

target_sources(${target_name}
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/exa_puplisher.cpp"
)


target_link_libraries(${target_name} PUBLIC WLIB)

add_test(${target_name} ${target_name})

target_compile_features(${target_name} PUBLIC cxx_std_20)
