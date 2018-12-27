
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/comm)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/comm)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/RenderSystem/scene)

LINK_LIBRARIES(glfw3.lib)
LINK_LIBRARIES(glad)
LINK_LIBRARIES(stb_image)
LINK_LIBRARIES(${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib/win32/assimp.lib)

SET(SHADER_DIR ${CMAKE_SOURCE_DIR}/resources/shaders/tests/LearnOpenGL/Shaders/)
#2 lighting
# 2.1 
ADD_EXECUTABLE(1.color LearnOpenGL/2.lighting/1.color.cpp)
SET_TARGET_PROPERTIES(1.color PROPERTIES FOLDER tests/2.lighting)
# 2.2.1
ADD_EXECUTABLE(2.1.basic_lighting_diffuse LearnOpenGL/2.lighting/2.basic_lighting_diffuse.cpp)
SET_TARGET_PROPERTIES(2.1.basic_lighting_diffuse PROPERTIES FOLDER tests/2.lighting)
# 2.2.2
ADD_EXECUTABLE(2.2.basic_lighting_specular LearnOpenGL/2.lighting/3.basic_lighting_specular.cpp)
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

#5 advanced lighting
#add_executable(1.advanced_lighting LearnOpenGL/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp)
#SET_TARGET_PROPERTIES(1.advanced_lighting PROPERTIES FOLDER tests/5.advanced_lighting)

# test 
ADD_EXECUTABLE(testOpenGL LearnOpenGL/TestOpenGL.cpp)
SET_TARGET_PROPERTIES(testOpenGL PROPERTIES FOLDER tests)