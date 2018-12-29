//////////////////////////////////////////////////////////////////////////
//@file: light.h
//@brief: �ƹ�ļ򵥷�װ,���ڻ��Ƶƹ�ģ��
//@author: szh
//@date: 2018/12/26

#ifndef RENDERRE_INCLUDE_RENDERSYSTEM_LIGHT_H
#define RENDERRE_INCLUDE_RENDERSYSTEM_LIGHT_H

#include <glm/vec3.hpp>

namespace RenderSystem {

	class Light
	{
	public:

		glm::vec3 m_positon;
		glm::vec3 m_color;

	};

}

#endif // RENDERRE_INCLUDE_RENDERSYSTEM_LIGHT_H