# ================================================================================
# 
# This file is part of the MagBottleC project.
# MagBottleC : testsuite
# 
# File: CMakeLists.txt
# 
# Original Author(s) of this File:
# 	Tom Gade, 2016
# 
# Author(s) of Significant Updates/Modifications to the File:
# 	...
# 
# -----------------------------------------------------------------------------------

project(TestSuite)

set(mbc_test_testsuite_cpp
  ${mbc_test_dir}/TestSuite.cpp
  ${mbc_test_dir}/testsuitetools.cpp
)

set(mbc_test_testsuite_h
  ${mbc_inc_dir}/testsuite/testsuitetools.h
)

set(mbc_test_vectortools_cpp
  ${mbc_test_dir}/vectortoolstest.cpp
)

set(mbc_test_vectortools_h
  ${mbc_inc_dir}/testsuite/vectortoolstest.h
)

set(mbc_testsuite
  ${mbc_test_testsuite_cpp} ${mbc_test_testsuite_h}
  ${mbc_test_vectortools_cpp} ${mbc_test_vectortools_h}
)

source_group("Header Files" FILES ${mbc_test_testsuite_h})
source_group("Source Files" FILES ${mbc_test_testsuite_cpp})
source_group("Header Files\\Vector Tools" FILES ${mbc_test_testsuite_h})
source_group("Source Files\\Vector Tools" FILES ${mbc_test_testsuite_cpp})

add_executable(TestSuite ${mbc_testsuite})

target_link_libraries(TestSuite MagBottleC ${ALL_LIBS})
