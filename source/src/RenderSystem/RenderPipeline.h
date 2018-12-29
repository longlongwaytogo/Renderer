//////////////////////////////////////////////////////////////////////////
//@file: RenderPipeline.h
//@brief: 渲染流水线，用于管理整个延迟渲染流程
//@author: szh
//@date: 2018/12/26

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_RENDERPIPELINE_H
#define RENDERER_INCLUDE_RENDERSYSTEM_RENDERPIPELINE_H

#include <memory>
#include <comm/model.h>
#include <RenderSystem/effect/Gbuffer.h>
#include <RenderSystem/scene/QuadGeometry.h>

namespace RenderSystem
{
	class RenderPipeline
	{
		RenderPipeline();
	public:

		void Init();
		static RenderPipeline& GetInstance();

		void DrawGeometryStage(Model* model);		// 绘制几何阶段
		void DrawDeferredLightStage(Model* model);  // 绘制延迟光照阶段
		//std::shared_ptr<GBuffer> GetGBuffer();
		 GBuffer& GetGBuffer();
	public:
		GBuffer m_gbuffer;					// GI 全局光照缓存
		Shader m_geometryShader;			// 几何处理着色
		Shader m_deferrerLightShader;		// 光照着色
		Shader m_lightShader;				// 灯光模型
		Shader m_fboDebugShader;
		QuadGeometry m_quad[2];
	};
}
#endif // RENDERER_INCLUDE_RENDERSYSTEM_RENDERPIPELINE_H