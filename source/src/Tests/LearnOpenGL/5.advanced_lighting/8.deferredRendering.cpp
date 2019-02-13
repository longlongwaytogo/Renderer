//8.deferredRendering.cpp
#include <TestOpenGLBase.h>
#include <RenderSystem/effect/Gbuffer.h>
#include <RenderSystem/scene/Box2.h>
#include <RenderSystem/scene/QuadGeometry.h>
#include <shader_m.h>
#include <model.h>
#include <vector>
#include <glm/vec3.hpp>
#include <debugger.h>


/* 伪代码：
while(...) // 游戏循环
{
// 1. 几何处理阶段：渲染所有的几何/颜色数据到G缓冲
glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
gBufferShader.Use();
for(Object obj : Objects)
{
ConfigureShaderTransformsAndUniforms();
obj.Draw();
}
// 2. 光照处理阶段：使用G缓冲计算场景的光照
glBindFramebuffer(GL_FRAMEBUFFER, 0);
glClear(GL_COLOR_BUFFER_BIT);
lightingPassShader.Use();
BindAllGBufferTextures();
SetLightingUniforms();
RenderQuad();
}
*/





using namespace RenderSystem;
class deferredRendering :public TestBase
{

public:
	virtual void InitShader()
	{	
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/Shaders/pipeline/");
		m_geometryPassShader.loadFile( shaderPath + "GBuffer_Geometry.vs", shaderPath + "GBuffer_Geometry.ps");
		
		m_deferrerLightPassShader.loadFile(shaderPath + "GBuffer_DeferredLight.vs", shaderPath + "GBuffer_DeferredLight.ps");
		m_deferrerLightPassShader.use();
		m_deferrerLightPassShader.setInt("gPosition", 0);
		m_deferrerLightPassShader.setInt("gNormal", 1);
		m_deferrerLightPassShader.setInt("gAlbedoSpec", 2);

		shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/5.advanced_lighting/");
		m_lightBoxShader.loadFile(shaderPath + "8.deferredRendering.vs", shaderPath +"8.deferredRendering.ps");
	
		Debugger::setUpDebugInfo();
	}
	virtual void InitData()
	{
		TestBase::InitData();

		m_nanosuit.loadModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
		
		// model position
		m_objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
		m_objectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
		m_objectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
		m_objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
		m_objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
		m_objectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
		m_objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
		m_objectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
		m_objectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));

		// lighting
		const unsigned int NR_LIGHTS = 32;
		srand(13);
		for (unsigned int i = 0; i < NR_LIGHTS; i++)
		{
			// calculate slightly random offsets
			float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
			float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
			float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
			m_lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
			// also calculate random color
			float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
			float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
			float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
			m_lightColors.push_back(glm::vec3(rColor, gColor, bColor));
		}
		//　创建GBuffer
		m_gBuffer.Create(SCR_WIDTH, SCR_HEIGHT);

		m_box.Init();
		m_quad.Init();

		m_camera.SetPose(glm::vec3(0, 0, 5));


	}
	virtual void Render()
	{
		glm::mat4 projection = glm::perspective(glm::radians(m_camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_camera.GetViewMatrix();
		glm::mat4 model;

		// 1. pass one, Render geometry  to gbuffer
		m_gBuffer.Enable(true);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		m_geometryPassShader.use();
		m_geometryPassShader.setMat4("projection", projection);
		m_geometryPassShader.setMat4("view", view);
		for (int i = 0; i < m_objectPositions.size(); ++i)
		{
			model = glm::mat4();
			model = glm::translate(model, m_objectPositions[i]);
			model = glm::scale(model, glm::vec3(0.25));
			m_geometryPassShader.setMat4("model", model);

			m_nanosuit.Draw(m_geometryPassShader);
		}
		m_gBuffer.Enable(false);

		// 2 render light to the quad
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_deferrerLightPassShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.GetPositionTexID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.GetNormalTexID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_gBuffer.GetAlbedoSpecTexID());
		for (int i = 0; i < m_lightPositions.size(); ++i)
		{
			m_deferrerLightPassShader.setVec3("lights[" + std::to_string(i) + "].Position", m_lightPositions[i]);
			m_deferrerLightPassShader.setVec3("lights[" + std::to_string(i) + "].Color", m_lightColors[i]);
			// update attenuation parameters and calculate radius
			const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
			const float linear = 0.7;
			const float quadratic = 1.8;
			m_deferrerLightPassShader.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
			m_deferrerLightPassShader.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);

			// then calculate radius of light volume/sphere
			const float maxBrightness = std::fmaxf(std::fmaxf(m_lightColors[i].r, m_lightColors[i].g), m_lightColors[i].b);
			float radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
			m_deferrerLightPassShader.setFloat("lights[" + std::to_string(i) + "].Radius", radius);

		}
		m_deferrerLightPassShader.setVec3("viewPos", m_camera.Position);
		// finally render quad
		m_quad.Draw();

		// 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer.GetFrameBufferID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer

		// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
		// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
		// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // GL_FRAMEBUFFER, equal bind GL_READ_FRAMEBUFFER and GL_DRAW_FRAMEBUFFER

		// 3 render light model on top of scene
		m_lightBoxShader.use();
		m_lightBoxShader.setMat4("projection", projection);
		m_lightBoxShader.setMat4("view", view);
		for (int i = 0; i < m_lightPositions.size(); ++i)
		{
			model = glm::mat4();
			model = glm::translate(model, m_lightPositions[i]);
			model = glm::scale(model, glm::vec3(0.125f));
			m_lightBoxShader.setMat4("model", model);
			m_lightBoxShader.setVec3("lightColor", m_lightColors[i]);

			m_box.Draw();
		}
	}
private:

	GBuffer m_gBuffer;						// GBuffer 全局光照缓存
	Shader m_geometryPassShader;			// 几何处理着色
	Shader m_deferrerLightPassShader;		// 光照着色
	Shader m_lightBoxShader;				// 灯光模型着色器， 由于光源本身不需要执行延迟光照计算，所以光照模型在延迟渲染完场景进行单独绘制

	QuadGeometry	m_quad;
	Box2 m_box;		// 灯光模型
	Model m_nanosuit; // 角色模型
	std::vector<glm::vec3> m_objectPositions;
	std::vector<glm::vec3> m_lightPositions;
	std::vector<glm::vec3> m_lightColors;

};
START_TEST(deferredRendering)
