if (WIN32)
  set (PROJECT_WINVER "0x0601" CACHE STRING "Windows version maro. Default is 0x0601 - win7, user can reset")
  add_definitions (-D_WIN32_WINNT=${PROJECT_WINVER})
endif ()

include_directories("${PROJECT_SOURCE_DIR}/../../boost_1_58_0")
include_directories("${PROJECT_SOURCE_DIR}/../../gce_install_boost_1.58_debug_32/include")
include_directories("${PROJECT_SOURCE_DIR}/../../gce_install_boost_1.58_debug_32/include/gce/adata/cpp")
include_directories("${PROJECT_SOURCE_DIR}/../../lua_install_5.15_debug_32/include")
include_directories("${PROJECT_SOURCE_DIR}/../../OpenSSL-Build/Build-OpenSSL-VC-32-dbg/include")
include_directories("${PROJECT_SOURCE_DIR}/Library")
include_directories("${PROJECT_SOURCE_DIR}/Common")
include_directories("${PROJECT_SOURCE_DIR}/../libs/protobuf/include")
include_directories("${PROJECT_SOURCE_DIR}/../libs/json/include")
include_directories("${PROJECT_SOURCE_DIR}/../libs/MySQL/include/Win32")
include_directories("${PROJECT_SOURCE_DIR}/../libs/MySQL Connector C++ 1.1.2/include/Win32")
include_directories("${PROJECT_SOURCE_DIR}/../libs/redis-cplusplus-client/include")

link_directories("${CMAKE_INSTALL_PREFIX}/lib")
link_directories("${PROJECT_SOURCE_DIR}/../../boost_1_58_0/stage/lib")
link_directories("${PROJECT_SOURCE_DIR}/../../lua_install_5.15_debug_32/lib")
link_directories("${PROJECT_SOURCE_DIR}/../../OpenSSL-Build/Build-OpenSSL-VC-32-dbg/lib")
link_directories("${PROJECT_SOURCE_DIR}/../libs/protobuf/lib/Win32/Debug")
link_directories("${PROJECT_SOURCE_DIR}/../libs/json/lib/Win32/")
link_directories("${PROJECT_SOURCE_DIR}/../libs/MySQL/lib/Win32/")
link_directories("${PROJECT_SOURCE_DIR}/../libs/MySQL Connector C++ 1.1.2/lib/Win32/")
link_directories("${PROJECT_SOURCE_DIR}/../libs/redis-cplusplus-client/lib/Win32/")

file (GLOB_RECURSE CLIENT_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file (GLOB_RECURSE CLIENT_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Bin)

add_executable(Client ${CLIENT_HEADER_FILES} ${CLIENT_SOURCE_FILES})

target_link_libraries(Client Library)
target_link_libraries(Client Common)
target_link_libraries(Client lua)
target_link_libraries(Client ssleay32)
target_link_libraries(Client libeay32)

if (UNIX)
	target_link_libraries(Client protobuf)
elseif(APPLE)
	target_link_libraries(Client protobuf)
else ()
	target_link_libraries(Client libprotobuf)
endif()

install (TARGETS Client DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/MirClientConfig.h" DESTINATION include)