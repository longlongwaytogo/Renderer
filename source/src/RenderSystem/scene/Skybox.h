//////////////////////////////////////////////////////////////////////////////////////////////
//@file: Skybox.h
//@brief:
//@description:
//@author: szh
//@date:2018-12-29T16:37:25
//////////////////////////////////////////////////////////////////////////////////////////////
#ifndef RENDERER_INCLUDE_RENDERSYSTEM_SKYBOX_H
#define RENDERER_INCLUDE_RENDERSYSTEM_SKYBOX_H

#include <comm/stb_image.h>
#include <vector>
#include <string>

#define USE_SKYBOX_DATA 1
#if USE_SKYBOX_DATA
#include <RenderSystem/scene/skyboxData.h>
#endif 

/* OpenGL 坐标系下
GL_TEXTURE_CUBE_MAP_POSITIVE_X	右
GL_TEXTURE_CUBE_MAP_NEGATIVE_X	左
GL_TEXTURE_CUBE_MAP_POSITIVE_Y	上
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	下
GL_TEXTURE_CUBE_MAP_POSITIVE_Z	后
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	前

*/


namespace RenderSystem {

	class Skybox {

	public:
		void Init()
		{
			m_box.Init();
		}
		void loadCubemap(std::vector<std::string> faces)
		{
			m_textures_face = faces;
			glGenTextures(1, &m_cubemapID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);

			int width, height, nrChannels;
			unsigned char* data;
			for (unsigned int i = 0; i < m_textures_face.size(); i++)
			{
				data = stbi_load(m_textures_face[i].c_str(), &width, &height, &nrChannels, 0);
				if (data)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					stbi_image_free(data);
				}
				else
				{
					std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
					stbi_image_free(data);
				}
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			/*std::string shaderPath = FileSystem::getPath("resources/resources/shaders/comm/");
			m_skyboxShader.loadFile(shaderPath + "skybox.vs", shaderPath + "skybox.ps");
			m_skyboxShader.use();
			m_skyboxShader.setInt(")*/
		}

		void Draw()
		{
			glDepthFunc(GL_LEQUAL); 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);
			m_box.Draw();
			glDepthFunc(GL_LESS);
		}

		unsigned int getTextureID() { return m_cubemapID; }
public:
	
private:
	unsigned int m_cubemapID;
#if USE_SKYBOX_DATA
	SkyboxData m_box;
#else
	Box m_box;
#endif 
	std::vector<std::string> m_textures_face;
	//Shader m_skyboxShader;

	};
}


#endif // RENDERER_INCLUDE_RENDERSYSTEM_SKYBOX_H
