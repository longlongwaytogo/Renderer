//////////////////////////////////////////////////////////////////////////
//@file: GBuffer.h
//@brief: 封装GBuffer相关操作和接口
//@author: szh
//@date: 2018/12/26

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_GBUFFER_H
#define RENDERER_INCLUDE_RENDERSYSTEM_GBUFFER_H



namespace RenderSystem {

	class  GBuffer
	{
	public:
		GBuffer(int w = 1, int h = 1);
		~GBuffer();

		void Create();
		void SetSize(int w, int h);
	private:
		unsigned int m_gBufferId;	// OpenGL FBO ID
		unsigned int m_gPosition;	// OpenGL Texture ID for Position
		unsigned int m_gNormal;		// OpenGL Texture ID for Normal
		unsigned int m_gAlbedoSpec; // OpenGL Texture ID for AlbedoSpec
		unsigned int m_gDepth;		// OpenGL FBO ID for Depth

		int m_width;
		int m_height;
	};
}
#endif // RENDERER_INCLUDE_RENDERSYSTEM_GBUFFER_H