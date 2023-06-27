cmake_minimum_required (VERSION 3.15)


set(target_name "EXA__SERIALIZER_EXAMPLE")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_executable(${target_name})

target_sources(${target_name}
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/exa_serializer.hpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/exa_serializer.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/exa_serializer_min.cpp"
)

target_link_libraries(${target_name} PUBLIC WLIB_SERIALIZER)
target_link_libraries(${target_name} PUBLIC  WLIB_BYTE_SINK)

add_test(${target_name} ${target_name})



