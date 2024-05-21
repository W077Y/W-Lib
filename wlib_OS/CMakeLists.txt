cmake_minimum_required (VERSION 3.19)

set(target_name "WLIB_OS")
add_library(${target_name} STATIC)

target_include_directories(${target_name}
 PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc"
)

if(${CMAKE_SYSTEM_NAME} STREQUAL  "Windows" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	target_include_directories(	${target_name}
	 PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc_PC"
	)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL  "Generic")
	if (${CMAKE_OS_NAME} STREQUAL "FreeRTOS")
		target_include_directories(	${target_name}
		 PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc_FreeRTOS"
		)
		target_sources(${target_name}
		 PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src_FreeRTOS/wlib_OS.cpp"
		)
		target_link_libraries(${target_name} PUBLIC freertos_kernel)
	elseif(${CMAKE_OS_NAME} STREQUAL "no-OS")
		target_include_directories(	${target_name}
		 PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc_no_OS"
		)
		target_sources(${target_name}
		 PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src_no_OS/wlib_no_OS.cpp"
		)
	else()
		message(SEND_ERROR "missing OS port")
	endif()
else()
	message(SEND_ERROR "missing OS port")
endif()

target_sources(${target_name}
 PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src/wlib_OS.cpp"
)

