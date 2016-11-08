cmake_minimum_required(VERSION 2.8.10)

# Project Name
project(supervisor)

set(VERSION_MAJOR "1" CACHE STRING "Major")
set(VERSION_MINOR "0" CACHE STRING "Minor")
set(VERSION_TYPE "0" CACHE STRING "Type")
set(VERSION_BUILD "0" CACHE STRING "Build")

message(STATUS "Version: " ${VERSION_MAJOR} "." ${VERSION_MINOR} "." ${VERSION_TYPE} "." ${VERSION_BUILD})

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  @ONLY)
  
 configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/SuperVisorVersion.cpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/SuperVisorVersion.cpp
  @ONLY)

FILE(GLOB TARGET_H 							"include/*.h*" "src/*.h*")
FILE(GLOB TARGET_SRC		 				"src/*.c*")

include_directories(include/)
include_directories(${TBB_INCLUDE})
include_directories(${ZEROMQ_INCLUDE})
include_directories(${PROTOBUF_INCLUDE})
include_directories(${COMMON_PATH}/include)
include_directories(${SPDLOG_INCLUDE})

# add library directories
link_directories(${TBB_LIBS})
link_directories(${ZEROMQ_LIBS})
link_directories(${SODIUM_LIBS})
link_directories(${PROTOBUF_LIBS})

add_executable(${PROJECT_NAME} 	${TARGET_SRC} 
								${TARGET_H} 
								"${COMMON_PATH}/include/hos_protocol.pb.h" "${COMMON_PATH}/src/hos_protocol.pb.cc"
								${CMAKE_CURRENT_BINARY_DIR}/version.rc ${CMAKE_CURRENT_BINARY_DIR}/SuperVisorVersion.cpp)

target_link_libraries(${PROJECT_NAME} ${SODIUM_LINK})
target_link_libraries(${PROJECT_NAME} ${TBB_LINK})
target_link_libraries(${PROJECT_NAME} ${ZEROMQ_LINK})
target_link_libraries(${PROJECT_NAME} common)

if (MSVC)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${TBB_BINS}/tbb.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${TBB_BINS}/tbb_debug.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${COMMON_BIN}/common.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	foreach(file ${ZEROMQ_DLL})
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				${file}
				$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	endforeach()
endif()
