cmake_minimum_required (VERSION 3.19)

set(target_name "WLIB_PUBLISHER")
add_library(${target_name} STATIC)

target_include_directories(${target_name}
 PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc"
)

target_sources(${target_name}
 PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc/wlib_publisher.hpp"
 PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc/wlib_publisher_Interface.hpp"
 PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc/wlib_LF_publisher.hpp"
 PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc/wlib_SPSC_subscriber.hpp"
)

target_sources(${target_name}
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/wlib_publisher.cpp"
)

target_link_libraries(${target_name}
 PUBLIC WLIB_OS
 PUBLIC WLIB_CONTAINER
)

