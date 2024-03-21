cmake_minimum_required (VERSION 3.15)


set(target_name "WLIB")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

#add_subdirectory("./WLib_Byte_Sink")
#add_subdirectory("./WLib_Serializer")
#add_subdirectory("./WLib_SPI_Abstraction")
#add_subdirectory("./WLib_CRC")


add_library(${target_name} STATIC)

target_include_directories(	${target_name}
	PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/inc")

message(STATUS "${sub_dir_tree_val}CONCEPTS")
target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_Concepts.hpp
	)

message(STATUS "${sub_dir_tree_val}PUBLISHER")
target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_Publisher_Interface.hpp
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_LockFree_Puplisher.hpp
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_SPSC_Subscriber.hpp
	)

message(STATUS "${sub_dir_tree_val}BYTE_SINK")
target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_Byte_Sink_Buffer.hpp
	)

message(STATUS "${sub_dir_tree_val}CALLBACK_INTERFACE")
target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_Callback_Interface.hpp
	)

message(STATUS "${sub_dir_tree_val}CONTAINER")
target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_SPSC.hpp
	)


if(${CMAKE_SYSTEM_NAME} STREQUAL  "Windows")
	message(STATUS "${sub_dir_tree_val}OS")
	target_include_directories(	${target_name}
		PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/port/PC/inc"
		)
	target_sources(${target_name}
		PUBLIC ${CMAKE_CURRENT_LIST_DIR}/port/PC/inc/WLib_OS.hpp
		)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL  "Generic")
	message(SEND_ERROR "missing OS port")
else()
	message(SEND_ERROR "missing OS port")
endif()


message(STATUS "${sub_dir_tree_val}UTILITY")
target_sources(${target_name}
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc/WLib_Utility.hpp
	)


target_sources(${target_name}
	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/WLib.cpp
	)
	

target_compile_features(${target_name} PUBLIC cxx_std_20)





