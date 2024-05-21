cmake_minimum_required (VERSION 3.19)


set(target_name "WLIB")

add_library(${target_name} STATIC)
target_compile_features(${target_name} PUBLIC cxx_std_20)


message(STATUS "#                 W-Lib: ${target_name}")
message(STATUS "#          W-Lib-CONFIG: ${CMAKE_SYSTEM_NAME}-${CMAKE_OS_NAME}")
message(STATUS "########################")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_callback")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_container")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_CRC")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_PIN_abstraction")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_LED_abstraction")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_PWM_abstraction")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_SPI_abstraction")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_serializer")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_CStr_sink")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_OS")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib_publisher")



target_include_directories(${target_name}
 PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc"
)

target_sources(${target_name}
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/wlib.cpp"
)

target_link_libraries(${target_name}
 PUBLIC WLIB_PIN_ABSTRACTIONS
 PUBLIC WLIB_LED_ABSTRACTIONS
 PUBLIC WLIB_PWM_ABSTRACTIONS
 PUBLIC WLIB_SPI_ABSTRACTIONS
 PUBLIC WLIB_CSTRING_SINK_INTERFACE
 PUBLIC WLIB_OS
 PUBLIC WLIB_CRC
 PUBLIC WLIB_CONTAINER
 PUBLIC WLIB_SERIALIZER
 PUBLIC WLIB_CALLBACK
 PUBLIC WLIB_PUBLISHER
)



