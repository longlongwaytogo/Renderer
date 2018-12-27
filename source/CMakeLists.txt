Project(Renderer)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
include(modules/comm.cmake)

# comm setting 
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(PLAT_FROM Win32)
#SET_OUTPUT_DIR()  直接调用SET_OUT_DIR()没有效果，将指令放在当前目录就可以
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLAT_FROM})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLAT_FROM})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${PLAT_FROM})

include_directories(${CMAKE_SOURCE_DIR}/thirdparty)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/include)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/glew-2.0.0/include)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/glfw-3.2.1.bin/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/assimp/include)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty)

#GET_PLATFORM_INFO(${IS_WIN32} ${IS_WIN64})
GET_PLATFORM_INFO()
MESSAGE("poiter:${CMAKE_SIZEOF_VOID_P}")
MESSAGE(is win32:${IS_WIN32},is win64:${IS_WIN64})
if(${IS_WIN32}) 
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/glfw-3.2.1.bin/Win32/lib-vc2015)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/glew-2.0.0/lib/Release/Win32)
else()
MESSAGE("${IS_WIN32_}")
MESSAGE("${${IS_WIN32_}}")
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/glfw-3.2.1.bin/x64/lib-vc2015)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/glew-2.0.0/lib/Release/x64)
endif()

add_subdirectory(src/RenderSystem)
add_subdirectory(src/Tests)
add_subdirectory(src/Launcher)
add_subdirectory(src/Comm)

#SET_PROPERTY(TARGET SGComm PROPERTY FOLDER Base)

