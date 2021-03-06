#cmake工具所需版本号
cmake_minimum_required(VERSION 3.0)

#项目名称
project(MirClient)

#设置工程版本号
set (MirClient_VERSION_MAJOR 0)
set (MirClient_VERSION_MINOR 1)

#配置一个头文件，通过它向源代码中传递一些CMake设置
configure_file (
"${PROJECT_SOURCE_DIR}/MirClientConfig.h.in"
"${PROJECT_BINARY_DIR}/MirClientConfig.h"
)

# 将二进制文件树添加到包含文件的搜索路径中，这样我们可以找到ConquerorConfig.h
include_directories("${PROJECT_BINARY_DIR}")

#工程安装目录
set(CMAKE_INSTALL_PREFIX ../mir_install)

if (WIN32)
  # Set vc bigobj
  if (${CMAKE_GENERATOR} MATCHES "Visual Studio *")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
  endif ()
endif ()

if (WIN32)
  # Set >= vc10 SAFESEH to NO
  if (${CMAKE_GENERATOR} MATCHES "Visual Studio 1*")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
  endif ()
endif()

#添加子工程目录
add_subdirectory(Common)
add_subdirectory(Client)
add_subdirectory(Library)