# ================================================================================
# 
# This file is part of the MagBottleC project.
# MagBottleC : Example2-MinVR
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

project(Example2-MinVR)

find_package(OpenGL REQUIRED)

#Example depends on MinVR package located in same parent directory as MagBottleC root folder:
#{parent_dir}/MagBottleC
#{parent_dir}/MinVR2
#Or modify the path to match MinVR2 location

set(MinVR_root_dir ${mbc_base_dir}/../MinVR2)

set(MinVR_inc_dir
  ${MinVR_root_dir}/src
)

include_directories(${OPENGL_INCLUDE_DIR} ${MinVR_inc_dir})

set(mbc_ex2_cpp
  ${mbc_ex_dir}/example2-MinVR/example2.cpp
)

set(mbc_ex2_vis_cpp
  ${mbc_src_dir}/vis/MinVR.cpp
  ${mbc_src_dir}/vis/ItemDraw_c.cpp
)

set(mbc_ex2_vis_h
  ${mbc_inc_dir}/vis/MinVR.h
  ${mbc_inc_dir}/vis/ItemDraw_c.h
)

set(mbc_ex2
  ${mbc_ex2_cpp}
  ${mbc_ex2_vis_cpp} ${mbc_ex2_vis_h}
)

set(MinVR_lib ${MinVR_root_dir}/build/lib/MinVR.lib)

source_group("Source Files" FILES ${mbc_ex2_cpp})
source_group("Source Files\\Vis" FILES ${mbc_ex2_vis_cpp})
source_group("Header Files\\Vis" FILES ${mbc_ex2_vis_h})

add_executable(Example2 ${mbc_ex2})

target_link_libraries(Example2 MagBottleC ${MinVR_lib} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${ALL_LIBS})
