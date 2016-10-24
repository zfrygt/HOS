cmake_minimum_required(VERSION 2.8.10)

# Project Name
project(Common)

set(VERSION_MAJOR "1" CACHE STRING "Major")
set(VERSION_MINOR "0" CACHE STRING "Minor")
set(VERSION_TYPE "0" CACHE STRING "Type")
set(VERSION_BUILD "0" CACHE STRING "Build")

message(STATUS "Version: " ${VERSION_MAJOR} "." ${VERSION_MINOR} "." ${VERSION_TYPE} "." ${VERSION_BUILD})

execute_process(COMMAND protoc -I=${CMAKE_SOURCE_DIR}/common --cpp_out=${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/common/hos_protocol.proto WORKING_DIRECTORY ${PROTOBUF_BIN})

file(RENAME ${CMAKE_SOURCE_DIR}/common/hos_protocol.pb.h ${CMAKE_SOURCE_DIR}/common/include/hos_protocol.pb.h)
file(RENAME ${CMAKE_SOURCE_DIR}/common/hos_protocol.pb.cc ${CMAKE_SOURCE_DIR}/common/src/hos_protocol.pb.cc)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  @ONLY)

FILE(GLOB TARGET_H 							"include/*.h*" "src/*.h*")
FILE(GLOB TARGET_SRC		 				"src/*.c*")
add_definitions(-DCOMMON_EXPORTS)

include_directories(include/)
include_directories(${TBB_INCLUDE})
include_directories(${ZEROMQ_INCLUDE})
include_directories(${PROTOBUF_INCLUDE})

# add library directories
link_directories(${TBB_LIBS})
link_directories(${ZEROMQ_LIBS})
link_directories(${SODIUM_LIBS})
link_directories(${PROTOBUF_LIBS})

add_library(${PROJECT_NAME} SHARED ${TARGET_SRC} ${TARGET_H} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

target_link_libraries(${PROJECT_NAME} ${SODIUM_LINK})
target_link_libraries(${PROJECT_NAME} ${TBB_LINK})
target_link_libraries(${PROJECT_NAME} ${ZEROMQ_LINK})
target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LINK})