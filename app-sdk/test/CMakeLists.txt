# CMake build script for ZeroMQ tests
cmake_minimum_required(VERSION "2.8.1")

# name of Windows project will tests.sln
PROJECT(tests)

set(tests 
			test_test1
			)
# add library and include dirs for all targets
include_directories("../utility/common/include")
include_directories(${ZEROMQ_INCLUDE})
include_directories(${PROTOBUF_INCLUDE})
link_directories(${ZEROMQ_LIBS})
link_directories(${SODIUM_LIBS})
link_directories(${TBB_LIBS})
link_directories(${PROTOBUF_LIBS})
link_libraries(common)
link_libraries(${ZEROMQ_LINK})

set(COMMON_BIN ${CMAKE_BINARY_DIR}/utility/common/Debug)

foreach(test ${tests})
	add_executable(${test} ${test}.cpp)
	add_test(NAME ${test} COMMAND ${test})

	foreach(file ${ZEROMQ_DLL})
		add_custom_command(TARGET ${test} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				${file}
				$<TARGET_FILE_DIR:${test}>)
	endforeach()
	
	add_custom_command(TARGET ${test} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${COMMON_BIN}/common.dll"
		$<TARGET_FILE_DIR:${test}>)

	set_target_properties(${test} PROPERTIES FOLDER "Tests")
endforeach()
		
#Check whether all tests in the current folder are present
file(READ "${CMAKE_CURRENT_LIST_FILE}" CURRENT_LIST_FILE_CONTENT)
file(GLOB ALL_TEST_SOURCES "test_*.cpp")
foreach(TEST_SOURCE ${ALL_TEST_SOURCES})
  get_filename_component(TESTNAME "${TEST_SOURCE}" NAME_WE)
  string(REGEX MATCH "${TESTNAME}" MATCH_TESTNAME "${CURRENT_LIST_FILE_CONTENT}")
  if (NOT MATCH_TESTNAME)
    message(AUTHOR_WARNING "Test '${TESTNAME}' is not known to CTest.")
  endif()
endforeach()

