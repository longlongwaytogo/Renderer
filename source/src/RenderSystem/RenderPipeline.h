//////////////////////////////////////////////////////////////////////////
//@file: RenderPipeline.h
//@brief: ��Ⱦ��ˮ�ߣ����ڹ��������ӳ���Ⱦ����
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

		void DrawGeometryStage(Model* model);		// ���Ƽ��ν׶�
		void DrawDeferredLightStage(Model* model);  // �����ӳٹ��ս׶�
		//std::shared_ptr<GBuffer> GetGBuffer();
		 GBuffer& GetGBuffer();
	public:
		GBuffer m_gbuffer;					// GI ȫ�ֹ��ջ���
		Shader m_geometryShader;			// ���δ�����ɫ
		Shader m_deferrerLightShader;		// ������ɫ
		Shader m_lightShader;				// �ƹ�ģ��
		Shader m_fboDebugShader;
		QuadGeometry m_quad[2];
	};
}
#endif // RENDERER_INCLUDE_RENDERSYSTEM_RENDERPIPELINE_H