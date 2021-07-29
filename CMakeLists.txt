cmake_minimum_required(VERSION 3.19)
project(huffman)

include(CTest)
enable_testing()

include_directories(
  include
)

add_library(huffmanlib
  include/huffman.h
  src/huffman.cpp
  include/huffmantree.h
  src/huffmantree.cpp
  include/queueprarrmin.h
  src/queueprarrmin.cpp
  include/node.h
)
set_property(TARGET huffmanlib PROPERTY CXX_STANDARD 17)

add_executable(huffman
  src/main.cpp
)

target_link_libraries(huffman
  huffmanlib
)

# Installation
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin CACHE PATH ${CMAKE_SOURCE_DIR}/build FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

install(TARGETS huffman CONFIGURATIONS Release RUNTIME DESTINATION ".")
install(DIRECTORY ./data/ DESTINATION ".")
install(DIRECTORY DESTINATION "./tests/compr_dict")
install(DIRECTORY DESTINATION "./tests/compr_no_dict")
install(DIRECTORY DESTINATION "./tests/out_dict")
install(DIRECTORY DESTINATION "./tests/out_no_dict")

# Testing
set(TestBinDir ${CMAKE_SOURCE_DIR}/bin)
set(TestInDir ${CMAKE_SOURCE_DIR}/bin/tests/in)
set(TestComprDictDir ${CMAKE_SOURCE_DIR}/bin/tests/compr_dict)
set(TestComprNoDictDir ${CMAKE_SOURCE_DIR}/bin/tests/compr_no_dict)
set(TestOutDictDir ${CMAKE_SOURCE_DIR}/bin/tests/out_dict)
set(TestOutNoDictDir ${CMAKE_SOURCE_DIR}/bin/tests/out_no_dict)

set(TestInputs text pict)
foreach(TestInput ${TestInputs})
  add_test(
    NAME ${TestInput}_TestWithDict_v
    COMMAND ${TestBinDir}/huffman.exe -v ${TestInDir}/${TestInput}.txt dict_${TestInput}.txt ${TestInput}.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
  )
  add_test(
    NAME ${TestInput}_Test1WithDict_p
    COMMAND ${TestBinDir}/huffman.exe -p ${TestInDir}/${TestInput}.txt ${TestComprDictDir}/${TestInput}.txt dict_${TestInput}.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
  )
  add_test(
    NAME ${TestInput}_Test1WithDict_u
    COMMAND ${TestBinDir}/huffman.exe -u ${TestComprDictDir}/${TestInput}.txt ${TestOutDictDir}/${TestInput}.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
  )
  add_test(
    NAME ${TestInput}_Test1WithDict_cmp
    COMMAND ${CMAKE_COMMAND} -E compare_files ${TestInDir}/${TestInput}.txt ${TestOutDictDir}/${TestInput}.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
  )

  add_test(
    NAME ${TestInput}_TestNoDict_v
    COMMAND ${TestBinDir}/huffman.exe -v ${TestInDir}/${TestInput}.txt dict_${TestInput}.txt ${TestInput}.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
  )
  add_test(
    NAME ${TestInput}_Test1NoDict_p
    COMMAND ${TestBinDir}/huffman.exe -p ${TestInDir}/${TestInput}.txt ${TestComprNoDictDir}/${TestInput}.txt dict_${TestInput}.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
  )
  add_test(
    NAME ${TestInput}_Test1NoDict_u
    COMMAND ${TestBinDir}/huffman.exe -u ${TestComprNoDictDir}/${TestInput}.txt ${TestOutNoDictDir}/${TestInput}.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
  )
  add_test(
    NAME ${TestInput}_Test1NoDict_cmp
    COMMAND ${CMAKE_COMMAND} -E compare_files ${TestInDir}/${TestInput}.txt ${TestOutNoDictDir}/${TestInput}.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
  )
endforeach()
