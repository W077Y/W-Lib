cmake_minimum_required (VERSION 3.19)

set(target_name "WLIB_CONTAINER")
add_library(${target_name} STATIC)

target_include_directories(${target_name}
 PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/inc"
)

target_sources(${target_name}
 PUBLIC	 "${CMAKE_CURRENT_LIST_DIR}/inc/wlib_SPSC.hpp"
 PUBLIC	 "${CMAKE_CURRENT_LIST_DIR}/inc/wlib_MPSC.hpp"
)

target_sources(${target_name}
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/WLib_Container.cpp"
)
