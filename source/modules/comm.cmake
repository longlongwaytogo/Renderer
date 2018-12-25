SET(COMM_INFO "This is the comm info")
# 当项目目录所处位置不在source下，可以设置单独的include， source 位置
SET(CPP_RELATIVE_PATH_NAME  "")  #工程的相对目录，如果osgExtern/EffectCompositor 工程cpp文件位于
SET(H_RELATIVE_PATH_NAME "")     #工程相对目录，如osgExtern/EffectCompositor 

###################################################################################### 
# platform 
SET(PLAT_FROM "x86")
SET(IS_WIN32 TRUE)
SET(IS_WIN64 FALSE)

#=============================================================================
# 函数名 gen_group 
#=============================================================================
# baseDir： 基于项目生成的根目录如，include， source 
# findPath： 文件搜索目录
# files: 文件列表
FUNCTION(gen_group  baseDir findPath files)
message(--------------------------------------------)
	foreach( filename ${files})
	#message(origin:${filename})
	#MESSAGE(findpath:${findPath})
		file(RELATIVE_PATH relativepath ${findPath} "${filename}")	 # 获取相对路径
		get_filename_component(group_name ${relativepath} DIRECTORY)
		SET(group_name_new "${group_name}")
		string(REPLACE "/" "\\" _source_path_msvc "${group_name_new}") # 将Unix风格斜杠转换为window风格
		source_group("${baseDir}\\${_source_path_msvc}"  FILES ${filename})
		#message(---"${baseDir}\\${_source_path_msvc}"  ${filename}---)
	ENDFOREACH()
EndFunction()
# eg.: gen_group("source1" ${CMAKE_SOURCE_DIR}/source "${Local_CPP}")
#=============================================================================


###########################################################################################################
# 创建非MFC子项目 宏定义 
# 函数名称：GENERATE_PROJ 
# ProjName: 项目名称
# 生成 LIB OR  EXE: TRUE/FALSE ON/OFF 1/0
###########################################################################################################
MACRO( GENERATE_PROJ ProjName IsLib IsStatic)
    STRING(TOUPPER ${ProjName} Upper_ProjName)
    MESSAGE("Generate Project:${Upper_ProjName}")

	IF( EXISTS ${CMAKE_SOURCE_DIR}/src/${ProjName})
		# cpp 目录中存在.h文件,则加入到cpp目录下
		FILE(GLOB_RECURSE Local_CPP   ${CMAKE_SOURCE_DIR}/src/${ProjName}/*.cpp ${CMAKE_SOURCE_DIR}/src/${ProjName}/*.h)
		gen_group("source" "${CMAKE_SOURCE_DIR}/src/${ProjName}" "${Local_CPP}")
	ELSE()
		FILE(GLOB_RECURSE Local_CPP   ${CMAKE_SOURCE_DIR}/src/${CPP_RELATIVE_PATH_NAME}/${ProjName}/*.cpp ${CMAKE_SOURCE_DIR}/src/${CPP_RELATIVE_PATH_NAME}/${ProjName}/*.h)
		gen_group("source" "${CMAKE_SOURCE_DIR}/src/${CPP_RELATIVE_PATH_NAME}/${ProjName}" "${Local_CPP}")
	ENDIF()
	#MESSAGE(========================)
	#MESSAGE(STATUS ${H_RELATIVE_PATH_NAME})
	IF( EXISTS ${CMAKE_SOURCE_DIR}/include/${ProjName})
		FILE(GLOB_RECURSE Local_H   ${CMAKE_SOURCE_DIR}/include/${ProjName}/*.*  )
		gen_group("include" "${CMAKE_SOURCE_DIR}/include/${ProjName}" "${Local_H}")
	ELSE()
		FILE(GLOB_RECURSE Local_H   ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/*.h)
	    gen_group("include" "${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}" "${Local_H}")
		INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME})
		#MESSAGE("include_dir:${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}")
	ENDIF()
	
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
	
	MESSAGE("${Local_CPP} ${Local_H} ${RC}")
	IF(${IsLib})  
		ADD_LIBRARY(${ProjName} SHARED ${Local_CPP} ${Local_H} ${RC})
		#my_add_library(${ProjName} SHARED ${Local_H} ${Local_CPP} ${RC})
		
		ADD_DEFINITIONS(-D${Upper_ProjName}_EXPORTS)
		#ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
		# gen export file
		MESSAGE("${H_RELATIVE_PATH_NAME}" )
		string(LENGTH "${H_RELATIVE_PATH_NAME}"  size)
		set(SolutionName "Render")
		IF(size GREATER 0)
			MESSAGE("Gen Relative dir:${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}")
			FILE(WRITE  ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/Export.h "#ifndef ${SolutionName}_INCLUDE_${Upper_ProjName}_EXPORT_H\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/Export.h "#define ${SolutionName}_INCLUDE_${Upper_ProjName}_EXPORT_H\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/Export.h "\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/Export.h "#ifdef ${Upper_ProjName}_EXPORTS\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/Export.h "#define ${Upper_ProjName}_API __declspec(dllexport)\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/Export.h "#else\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/Export.h "#define ${Upper_ProjName}_API __declspec(dllimport)\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/Export.h "#endif\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/Export.h "\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${H_RELATIVE_PATH_NAME}/${ProjName}/Export.h "#endif // ${SolutionName}_INCLUDE_${Upper_ProjName}_EXPORT_H\n" )
		
		ELSE()
		MESSAGE("absolute dir:${H_RELATIVE_PATH_NAME}")
			FILE(WRITE  ${CMAKE_SOURCE_DIR}/include/${ProjName}/Export.h "#ifndef ${SolutionName}_INCLUDE_${Upper_ProjName}_EXPORT_H\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${ProjName}/Export.h "#define ${SolutionName}_INCLUDE_${Upper_ProjName}_EXPORT_H\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${ProjName}/Export.h "\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${ProjName}/Export.h "#ifdef ${Upper_ProjName}_EXPORTS\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${ProjName}/Export.h "#define ${Upper_ProjName}_API __declspec(dllexport)\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${ProjName}/Export.h "#else\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${ProjName}/Export.h "#define ${Upper_ProjName}_API __declspec(dllimport)\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${ProjName}/Export.h "#endif\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${ProjName}/Export.h "\n" )
			FILE(APPEND ${CMAKE_SOURCE_DIR}/include/${ProjName}/Export.h "#endif // ${SolutionName}_INCLUDE_${Upper_ProjName}_EXPORT_H\n" )
		ENDIF()
	ELSEIF(${IsStatic})
		ADD_LIBRARY(${ProjName} STATIC ${Local_CPP} ${Local_H} ${RC})
	ELSE()
	 # ADD_LIBRARY(${ProjName} Win32 ${Local_CPP} ${Local_H} ${RC})
	ADD_EXECUTABLE(${ProjName} ${Local_CPP} ${Local_H} ${RC})
	ENDIF()
# set debug
#set_target_properties (myTarget PROPERTIES DEBUG_POSTFIX _d)
#set_property(TARGET myTarget PROPERTY DEBUG_POSTFIX _d) 
	SET_TARGET_PROPERTIES(${ProjName} PROPERTIES DEBUG_POSTFIX _d)
	SET_TARGET_PROPERTIES(${ProjName} PROPERTIES COMPILE_FLAGS "/MP")
	# SET_TARGET_PROPERTIES(${ProjName} PROPERTIES )
	add_definitions(-DUNICODE -D_UNICODE)
	

# 清除变量值
	SET(CPP_RELATIVE_PATH_NAME  "")   #工程的相对目录，如果osgExtern/EffectCompositor 工程cpp文件位于
	SET(H_RELATIVE_PATH_NAME "" )     #工程相对目录，如osgExtern/EffectCompositor 
ENDMACRO()
################################################################
# 生成Lib库
MACRO(GEN_LIB ProjName)
GENERATE_PROJ(${ProjName} TRUE FALSE)
ENDMACRO()

MACRO(GEN_STATIC_LIB ProjName)
GENERATE_PROJ(${ProjName} TRUE TRUE)
ENDMACRO()

###############################################################
# 生成exe
MACRO(GEN_EXE ProjName)
GENERATE_PROJ(${ProjName} FALSE FALSE)
ENDMACRO()

################################################################
## test function
#@testName: 测试项目名称
#@src: 源码集合
#@folder： 生成工程位于整个项目结构的那个目录下(项目工程中目录，不是磁盘中文件目录)
#@isExe： 是否是exe工程
################################################################
FUNCTION(Add_Test_case  testName src folder isExe)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
	if(${isExe})
		add_executable(${testName} ${src})
		#MESSAGE("create exe of test")
	else()
		add_library(${testName} ${src})
		#MESSAGE("create lib of test")
	endif()
	SET_PROPERTY(TARGET ${testName} PROPERTY DEBUG_POSTFIX _d)  
	#set_target_properties (${testName} PROPERTIES DEBUG_POSTFIX _d)
	SET_PROPERTY(TARGET ${testName} PROPERTY FOLDER ${folder})
	# 设置字符集
	ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
ENDFUNCTION()

MACRO(GEN_TEST_EXE testName src folder)
	Add_Test_case(${testName} ${src} ${folder} TRUE)
ENDMACRO()


FUNCTION(GET_PLATFORM_INFO )
	IF ( "${CMAKE_SIZEOF_VOID_P}" EQUAL "4" )
		MESSAGE(System:x86) 
		SET(PLAT_FROM "x86" PARENT_SCOPE)
		SET(IS_WIN32 TRUE PARENT_SCOPE)
		SET(IS_WIN64 FALSE PARENT_SCOPE)
	ELSEIF("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
		MESSAGE(System:x64)
		SET(PLAT_FROM "x64" PARENT_SCOPE)
		SET(IS_WIN32 FALSE PARENT_SCOPE)
		SET(IS_WIN64 TRUE PARENT_SCOPE)
	ENDIF()
	MESSAGE("isWin32:${${IS_WIN32}},isWin64:${${IS_WIN64}}")
ENDFUNCTION()

FUNCTION(SET_OUTPUT_DIR)
	MESSAGE("--plat form--:${PLAT_FROM}")
	# runtime
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLAT_FROM})
	# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 					${CMAKE_BINARY_DIR}/bin/${PLAT_FROM}/Debug)
	# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 					${CMAKE_BINARY_DIR}/bin/${PLAT_FROM}/Release)
	# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO 			${CMAKE_BINARY_DIR}/bin/${PLAT_FROM}/RelWithDebInfo)
	# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL				${CMAKE_BINARY_DIR}/bin/${PLAT_FROM}/MinSizeRel)
	
	# lib
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLAT_FROM})
	# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG 					${CMAKE_BINARY_DIR}/bin/${PLAT_FROM}/Debug)
	# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE 					${CMAKE_BINARY_DIR}/bin/${PLAT_FROM}/Release)
	# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO 			${CMAKE_BINARY_DIR}/bin/${PLAT_FROM}/RelWithDebInfo)

	# archive
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${PLAT_FROM})
	# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 					${CMAKE_BINARY_DIR}/lib/${PLAT_FROM}/Debug)
	# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE 					${CMAKE_BINARY_DIR}/lib/${PLAT_FROM}/Release)
	# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO 			${CMAKE_BINARY_DIR}/lib/${PLAT_FROM}/RelWithDebInfo)

	#原文：https://blog.csdn.net/justin_bkdrong/article/details/81631758 

 ENDFUNCTION()
 
 
 FUNCTION(BASE_SETTING)
	GET_PLATFORM_INFO()
	SET_OUTPUT_DIR()
 ENDFUNCTION()
 
 
 
 

 
