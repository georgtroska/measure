cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(test LANGUAGES CXX)

find_package(ROOT REQUIRED )
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})

set(CMAKE_CXX_FLAGS "${ROOT_CXX_FLAGS}")
# set(CMAKE_ROOTTEST_NOROOTMAP ON)

root_generate_dictionary(DictOutput
	${CMAKE_CURRENT_SOURCE_DIR}/A.h
    ${CMAKE_CURRENT_SOURCE_DIR}/oneheader.h
	OPTIONS
	-writeEmptyRootPCM
	
)
add_library(test SHARED
    DictOutput.cxx
)

add_executable(testexe ${CMAKE_CURRENT_SOURCE_DIR}/testexe.cxx)

# set_target_properties(test PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
# target_link_libraries(test PUBLIC ROOT::Core)
target_link_libraries(test PUBLIC ${ROOT_LIBRARIES})


