GEN_LIB(RenderSystem)
TARGET_LINK_LIBRARIES(RenderSystem glfw3.lib)
TARGET_LINK_LIBRARIES(RenderSystem glad)
TARGET_LINK_LIBRARIES(RenderSystem stb_image)
TARGET_LINK_LIBRARIES(RenderSystem ${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib/win32/assimp.lib)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/comm)


