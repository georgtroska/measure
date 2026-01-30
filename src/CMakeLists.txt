cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(measure LANGUAGES CXX)

find_package(ROOT REQUIRED )
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})

set(CMAKE_CXX_FLAGS "${ROOT_CXX_FLAGS}")



root_generate_dictionary(DictOutput
    ${CMAKE_CURRENT_SOURCE_DIR}/Channel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DCELink.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DCEResource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Device.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ExcLimit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/HP33120A.h
    ${CMAKE_CURRENT_SOURCE_DIR}/K2000.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Limit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MeasEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MeasureChannel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Param.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/PosixFile.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/PosixSocket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RS232Link.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Scan1D.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ScanSave.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Signal.h
#    ${CMAKE_CURRENT_SOURCE_DIR}/TCPLink.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Window1DGraph.h
    OPTIONS
	-writeEmptyRootPCM
)
add_library(measure SHARED
    DictOutput.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/Channel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DCELink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DCEResource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HP33120A.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/K2000.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Limit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MeasureChannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Param.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/PosixFile.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/PosixSocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RS232Link.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scan1D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ScanSave.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Signal.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/TCPLink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window1DGraph.cpp
)
# set_target_properties(measure PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
# target_link_libraries(measure PUBLIC ROOT::Core)
target_link_libraries(measure PUBLIC ${ROOT_LIBRARIES})


