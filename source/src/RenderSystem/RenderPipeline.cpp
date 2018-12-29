#include <RenderSystem/RenderPipeline.h>
#include <comm/filesystem.h>


RenderSystem::RenderPipeline::RenderPipeline()
{


}

void RenderSystem::RenderPipeline::Init()
{
	//m_gbuffer = std::make_shared<GBuffer>();
	// 几何处理shader
	std::string shaderPath = FileSystem::getPath("resources/shaders/pipeline/");
	std::string geometryShaderVS = shaderPath + "GBuffer_GeometryShader.vs";
	std::string geometryShaderPS = shaderPath + "GBuffer_GeometryShader.ps";
	m_geometryShader.loadFile(geometryShaderVS, geometryShaderPS);

	// 延迟光照处理shader
	std::string deferredLightVS = shaderPath + "GBuffer_DefferLightShader.vs";
	std::string deferredLightPS = shaderPath + "GBuffer_DefferLightShader.ps";
	m_deferrerLightShader.loadFile(deferredLightVS, deferredLightPS);

	// 调试shader, 用于查看个阶段输出的buffer资源
	std::string fboDebugvs = shaderPath + "fbo_debug.vs";
	std::string fboDebugfs = shaderPath + "fbo_debug.fs";
	m_fboDebugShader.loadFile(fboDebugvs, fboDebugfs);

}

RenderSystem::RenderPipeline& RenderSystem::RenderPipeline::GetInstance()
{
	static RenderPipeline s_RenderPipeline;
	return s_RenderPipeline;
}

void RenderSystem::RenderPipeline::DrawGeometryStage(Model* model)
{
	model->Draw(m_geometryShader);
}

void RenderSystem::RenderPipeline::DrawDeferredLightStage(Model* model)
{
	model->Draw(m_deferrerLightShader);
}

RenderSystem::GBuffer& RenderSystem::RenderPipeline::GetGBuffer()
{
	return m_gbuffer;
}

