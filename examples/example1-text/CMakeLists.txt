# ================================================================================
# 
# This file is part of the MagBottleC project.
# MagBottleC : Example1-Text
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

project(Example1-Text)

set(mbc_ex1_cpp
  ${mbc_ex_dir}/example1-text/example1.cpp
)

source_group("Source Files" FILES ${mbc_ex1_cpp})

add_executable(Example1 ${mbc_ex1_cpp})

target_link_libraries(Example1 MagBottleC ${ALL_LIBS})
