#####################################################################################################
#@brief : 创建测试用例
#@author: szh
#@date: 2018/12/28

# 函数返回值测试
#函数和宏可以通过命令 return() 返回，但是函数和宏的返回值必须通过参数传递出去
# 函数或宏的返回值不可以使用 ${} 引用，直接使用变量即可
function(get_func RESULT)
#RESULT 的值为实参的值，因此需要使用 ${RESULT}
#这里使用 PARENT_SCOPE 是因为函数会构建一个局部作用域
set(${RESULT} "Hello Function" PARENT_SCOPE)
endfunction()

macro(get_macro RESULT)
  set(${RESULT} "Hello Macro")
endmacro()

get_func(V1)
# 输出 Hello Function
message(${V1})
get_macro(V2)
# 输出 Hello Macro
message(${V2})

# test retrun function

#https://markdewing.github.io/blog/posts/notes-on-cmake/
#https://preshing.com/20170522/learn-cmakes-scripting-language-in-15-minutes/


SET(SHADER_DIR ${CMAKE_SOURCE_DIR}/resources/shaders/tests/LearnOpenGL/Shaders/)
#函数会为变量创建一个局部作用域，而宏则使用全局作用域

FUNCTION(GET_EXT fileName EXT)
	STRING(FIND  ${fileName} "." file_ext_pos REVERSE) 
	STRING(SUBSTRING ${fileName} ${file_ext_pos} -1 file_ext)
	SET(${EXT} ${file_ext}  PARENT_SCOPE)
ENDFUNCTION()

# 获取不含有.后缀的EXT
FUNCTION(GET_EXT_WITHOUTDOT fileName EXT)
	STRING(FIND  ${fileName} "." file_ext_pos REVERSE) 
	MATH(EXPR pos "${file_ext_pos} + 1")
	STRING(SUBSTRING ${fileName} ${pos} -1 file_ext)
	SET(${EXT} ${file_ext}  PARENT_SCOPE)
ENDFUNCTION()

#####################################################################################################
# CreateTest 
# @testName: 测试用例名字
# @src_dir: src \ shader\ group_dir 所在目录
MACRO(CreateTest testName  src_dir)
    SET(VS_FILE ${SHADER_DIR}/${src_dir}/${testName}.vs)
	SET(PS_FILE ${SHADER_DIR}/${src_dir}/${testName}.ps)
	SET(SRC_FILE  ${CMAKE_CURRENT_SOURCE_DIR}/LearnOpenGL/${src_dir}/${testName}.cpp)
	
	SET(FILES ${VS_FILE} ${PS_FILE} ${SRC_FILE})
	
	FOREACH(file ${FILES})
		IF(NOT EXISTS  ${file})
			GET_FILENAME_COMPONENT(file_name ${file} NAME)
			GET_EXT(${file} file_ext)
			GET_EXT_WITHOUTDOT(${testName} test_name)
			MESSAGE("${file_ext}")
			IF( ${file_ext} STREQUAL ".cpp")	
				FILE(WRITE ${file} "//${file_name}\n")
				FILE(APPEND ${file} "#include<TestOpenGLBase.h>\n")
				FILE(APPEND ${file} "#include <box.h>\n")
				FILE(APPEND ${file} "using namespace RenderSystem;\n")
				FILE(APPEND ${file} "class ${test_name} :public TestBase\n")
				FILE(APPEND ${file} "{\n")
				FILE(APPEND ${file} "\n")
				FILE(APPEND ${file} "	virtual void InitShader()\n	{\n		TestBase::InitShader();\n	}\n")
				FILE(APPEND ${file} "	virtual void InitData()\n	{\n		TestBase::InitData();\n	}\n")
				FILE(APPEND ${file} "	virtual void Render()\n	{\n		TestBase::Render();\n	}\n")
				FILE(APPEND ${file} "private:\n")
				FILE(APPEND ${file} "};\n")
				FILE(APPEND ${file} "START_TEST(${test_name})\n")
			ELSEIF( ${file_ext} STREQUAL ".vs" OR ${file_ext} STREQUAL ".ps")
				FILE(WRITE ${file} "//${file_name}\n")
				FILE(APPEND ${file} "#version 330 core\n")
				FILE(APPEND ${file} "void main()\n")
				FILE(APPEND ${file} "{\n")
				FILE(APPEND ${file} "\n")
				FILE(APPEND ${file} "}\n")
			ENDIF()	
		ENDIF()
	ENDFOREACH()
	
	SET(SHADER_SRC ${SHADER_DIR}/${src_dir}/${testName}.vs ${SHADER_DIR}/${src_dir}/${testName}.ps) 
	MESSAGE("${testName} ${FILES}")
	ADD_EXECUTABLE(${testName} ${FILES})
	SET_TARGET_PROPERTIES(${testName} PROPERTIES FOLDER tests/${src_dir})
ENDMACRO()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/comm)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/comm)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/RenderSystem/scene)

LINK_LIBRARIES(glfw3.lib)
LINK_LIBRARIES(glad)
LINK_LIBRARIES(stb_image)
LINK_LIBRARIES(${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib/win32/assimp.lib)


#2 lighting
# 2.1
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/1.color.vs ${SHADER_DIR}/2.lighting/1.color.ps) 
ADD_EXECUTABLE(1.color LearnOpenGL/2.lighting/1.color.cpp)
SET_TARGET_PROPERTIES(1.color PROPERTIES FOLDER tests/2.lighting ${SHADER_SRC} )
# 2.2.1
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/2.basic_lighting_diffuse.vs ${SHADER_DIR}/2.lighting/2.basic_lighting_diffuse.ps) 
ADD_EXECUTABLE(2.1.basic_lighting_diffuse LearnOpenGL/2.lighting/2.basic_lighting_diffuse.cpp ${SHADER_SRC})
SET_TARGET_PROPERTIES(2.1.basic_lighting_diffuse PROPERTIES FOLDER tests/2.lighting)
# 2.2.2
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/3.basic_lighting_specular.vs ${SHADER_DIR}/2.lighting/3.basic_lighting_specular.ps) 
ADD_EXECUTABLE(2.2.basic_lighting_specular LearnOpenGL/2.lighting/3.basic_lighting_specular.cpp ${SHADER_SRC})
SET_TARGET_PROPERTIES(2.2.basic_lighting_specular PROPERTIES FOLDER tests/2.lighting)
# 2.3.1 material
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/3.1.material.vs ${SHADER_DIR}/2.lighting/3.1.material.ps)
ADD_EXECUTABLE(3.1.material LearnOpenGL/2.lighting/3.1.material.cpp ${SHADER_SRC} )
SET_TARGET_PROPERTIES(3.1.material PROPERTIES FOLDER tests/2.lighting)
# 2.4.1 diffuse map
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/4.1.lighting_map_diffusemap.vs ${SHADER_DIR}/2.lighting/4.1.lighting_map_diffusemap.ps)
ADD_EXECUTABLE(4.1.lighting_map_diffusemap LearnOpenGL/2.lighting/4.1.lighting_map_diffusemap.cpp ${SHADER_SRC} )
SET_TARGET_PROPERTIES(4.1.lighting_map_diffusemap PROPERTIES FOLDER tests/2.lighting)
# 2.4.3 
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/4.3.lighting_maps_exercise4.vs ${SHADER_DIR}/2.lighting/4.3.lighting_maps_exercise4.ps)
ADD_EXECUTABLE(4.3.lighting_maps_exercise4 LearnOpenGL/2.lighting/4.3.lighting_maps_exercise4.cpp ${SHADER_SRC} )
SET_TARGET_PROPERTIES(4.1.lighting_map_diffusemap PROPERTIES FOLDER tests/2.lighting)

# 2.5.1 direction
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/5.1.light_casters_directional.vs ${SHADER_DIR}/2.lighting/5.1.light_casters_directional.ps)
ADD_EXECUTABLE(5.1.light_casters_directional LearnOpenGL/2.lighting/5.1.light_casters_directional.cpp ${SHADER_SRC} )
SET_TARGET_PROPERTIES(5.1.light_casters_directional PROPERTIES FOLDER tests/2.lighting)
 
# 2.5.2 point
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/5.2.light_casters_point.vs ${SHADER_DIR}/2.lighting/5.2.light_casters_point.ps)
ADD_EXECUTABLE(5.2.light_casters_point LearnOpenGL/2.lighting/5.2.light_casters_point.cpp ${SHADER_SRC} )
SET_TARGET_PROPERTIES(5.2.light_casters_point PROPERTIES FOLDER tests/2.lighting)

# 2.5.3  spot
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/5.3.light_casters_spot.vs ${SHADER_DIR}/2.lighting/5.3.light_casters_spot.ps)
ADD_EXECUTABLE(5.3.light_casters_spot LearnOpenGL/2.lighting/5.3.light_casters_spot.cpp ${SHADER_SRC} )
SET_TARGET_PROPERTIES(5.3.light_casters_spot PROPERTIES FOLDER tests/2.lighting)
# 2.5.4   soft spot
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/5.4.light_casters_spot_soft.vs ${SHADER_DIR}/2.lighting/5.4.light_casters_spot_soft.ps)
ADD_EXECUTABLE(5.4.light_casters_spot_soft LearnOpenGL/2.lighting/5.4.light_casters_spot_soft.cpp ${SHADER_SRC} )
SET_TARGET_PROPERTIES(5.4.light_casters_spot_soft PROPERTIES FOLDER tests/2.lighting)

# 2.6
SET(SHADER_SRC ${SHADER_DIR}/2.lighting/6.multiple_lights.vs ${SHADER_DIR}/2.lighting/6.multiple_lights.ps)
ADD_EXECUTABLE(6.multiple_lights LearnOpenGL/2.lighting/6.multiple_lights.cpp ${SHADER_SRC} )
SET_TARGET_PROPERTIES(6.multiple_lights PROPERTIES FOLDER tests/2.lighting)

# 4.1 advanced_openg  depth test
CreateTest(1.1.depth_testing 4.advanced_opengl)
CreateTest(1.2.depth_testing 4.advanced_opengl)
#5 advanced lighting
#add_executable(1.advanced_lighting LearnOpenGL/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp)
#SET_TARGET_PROPERTIES(1.advanced_lighting PROPERTIES FOLDER tests/5.advanced_lighting)


# test 
ADD_EXECUTABLE(testOpenGL LearnOpenGL/TestOpenGL.cpp)
SET_TARGET_PROPERTIES(testOpenGL PROPERTIES FOLDER tests)