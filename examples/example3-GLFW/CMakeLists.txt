# ================================================================================
# 
# This file is part of the MagBottleC project.
# MagBottleC : Example3-GLFW
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

project(Example3-GLFW)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

set(mbc_ex3_cpp
  ${mbc_ex_dir}/example3-GLFW/example3.cpp
)

set(mbc_ex3_vis_cpp
  ${mbc_src_dir}/vis/ItemDraw_c.cpp
)

set(mbc_ex3_vis_h
  ${mbc_inc_dir}/vis/ItemDraw_c.h
)

set(mbc_ex3
  ${mbc_ex3_cpp}
  ${mbc_ex3_vis_cpp} ${mbc_ex3_vis_h}
)

source_group("Source Files" FILES ${mbc_ex3_cpp})
source_group("Source Files\\Vis" FILES ${mbc_ex3_vis_cpp})
source_group("Header Files\\Vis" FILES ${mbc_ex3_vis_h})

add_executable(Example3 ${mbc_ex3})

target_link_libraries(Example3 MagBottleC "glfw3.lib" "glew32.lib" ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${ALL_LIBS})
