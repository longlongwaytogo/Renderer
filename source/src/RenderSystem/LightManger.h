//////////////////////////////////////////////////////////////////////////
//@file LightManager.h 
//@brief: 管理灯光对象
//@author: szh
//@date: 2018/12/26

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_LIGHTMANAGER_H
#define RENDERER_INCLUDE_RENDERSYSTEM_LIGHTMANAGER_H

#include <vector>
#include <RenderSystem/Light.h>

namespace RenderSystem {
	class LightManager
	{
	public:
		void AddLight(Light light);

		void DrawLights();

	private:
		std::vector<Light> m_lights;
	};
}

#endif // RENDERER_INCLUDE_RENDERSYSTEM_LIGHTMANAGER_H