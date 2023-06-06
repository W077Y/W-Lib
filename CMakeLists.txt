cmake_minimum_required (VERSION 3.15)

set(target_name "W-LIB_PROJECT")

FUNCTION(MESSAGE_TREEVIEW VAL)
IF (NOT DEFINED sub_dir_tree_val) 
	message(STATUS "${VAL}")
	SET(sub_dir_tree_val "+-> " PARENT_SCOPE)
ELSE()
	message(STATUS "${sub_dir_tree_val}${VAL}")
	SET(sub_dir_tree_val "|   ${sub_dir_tree_val}" PARENT_SCOPE)
ENDIF()
ENDFUNCTION()



MESSAGE_TREEVIEW(${target_name})
project(${target_name} LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON )

if (MSVC)
    # warning level 4
    # add_compile_options(/W4)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    # additional warnings
    add_compile_options(-O3 -Wall -Wextra -Wpedantic)
endif()


enable_testing()
add_subdirectory("./W-Lib")
add_subdirectory("./examples")
add_subdirectory("./tests")




