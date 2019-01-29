#####################################################################################################
#@brief : ¥¥Ω®≤‚ ‘”√¿˝
#@author: szh
#@date: 2018/12/28


CreateHead( Renderer RenderSystem Skybox "${CMAKE_CURRENT_SOURCE_DIR}")

SET(SHADER_DIR ${CMAKE_SOURCE_DIR}/resources/shaders/tests/LearnOpenGL/Shaders/)



INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/comm)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/comm)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/RenderSystem/scene)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/RenderSystem)

LINK_LIBRARIES(glfw3.lib)
LINK_LIBRARIES(glad)
LINK_LIBRARIES(stb_image)
LINK_LIBRARIES(${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib/win32/assimp.lib)
#LINK_LiBRARIES(RenderSystem)


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
CreateTest(2.stencil_testing 4.advanced_opengl)
CreateTest(2.1.stencil_tesing_wrap 4.advanced_opengl)
CreateTest(3.1.blend_discard 4.advanced_opengl)
CreateTest(3.2.blend_sort    4.advanced_opengl)
CreateTest(5.1.framebuffers 4.advanced_opengl)

SET(TEST_Dependcy_FILE "${SHADER_DIR}/4.advanced_opengl/6.1.model.vs" "${SHADER_DIR}/4.advanced_opengl/6.1.model.ps")
CreateTest(6.1.cubemap_skybox 4.advanced_opengl)
CreateTest(8.advanced_glsl_ubo 4.advanced_opengl)
SET(TEST_Dependcy_FILE "${SHADER_DIR}/4.advanced_opengl/9.1.GeometryShader_1.gs"  "${SHADER_DIR}/4.advanced_opengl/9.1.GeometryShader_2.gs")
CreateTest(9.1.GeometryShader 4.advanced_opengl)

SET(TEST_Dependcy_FILE "${SHADER_DIR}/4.advanced_opengl/9.2.GeometryShader_1.gs" "${SHADER_DIR}/4.advanced_opengl/9.2.GeometryShader_2.gs"
	"${SHADER_DIR}/4.advanced_opengl/9.2.GeometryShader_1.ps" "${SHADER_DIR}/4.advanced_opengl/9.2.GeometryShader_2.ps")
CreateTest(9.2.GeometryShader 4.advanced_opengl)
CreateTest(10.1.Instancing_quads 4.advanced_opengl)
SET(TEST_Dependcy_FILE "${SHADER_DIR}/4.advanced_opengl/10.2.planet.vs"  "${SHADER_DIR}/4.advanced_opengl/10.2.planet.ps")
CreateTest(10.2.Instancing_asteroids 4.advanced_opengl)
CreateTest(11.1.anti_aliasing 4.advanced_opengl) 
CreateTest(11.2.anti_aliasing_offscreen 4.advanced_opengl)


#5 advanced lighting
CreateTest(1.Blinn_Phong 5.advanced_lighting)
CreateTest(2.gamma_correction  5.advanced_lighting)

CreateTest(4.normal_mapping 5.advanced_lighting)
CreateTest(5.parallax_mapping 5.advanced_lighting)

SET(TEST_Dependcy_FILE "${SHADER_DIR}/5.advanced_lighting/6.lighting.vs"  "${SHADER_DIR}/5.advanced_lighting/6.lighting.ps")
CreateTest(6.hdr 5.advanced_lighting)
CreateTest(8.deferredRendering 5.advanced_lighting)
CreateTest(9.ssao 5.advanced_lighting)

# test 
ADD_EXECUTABLE(testOpenGL LearnOpenGL/TestOpenGL.cpp)
SET_TARGET_PROPERTIES(testOpenGL PROPERTIES FOLDER tests)