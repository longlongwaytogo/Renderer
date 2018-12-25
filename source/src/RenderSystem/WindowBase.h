//////////////////////////////////////////////////////////////////////////
//@file: WindowBase.h
//@brief: 窗口基类
//@author: szh
//@date: 2018/12/25

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_WINDOWBASE_H
#define RENDERER_INCLUDE_RENDERSYSTEM_WINDOWBASE_H

#include <string>
namespace RenderSystem {
	class WindowBase
	{
	public:
		WindowBase():m_width(800), m_height(600){}
		virtual ~WindowBase(){}

		virtual void  SetWindowSize(int w, int h) {
			m_width = w; m_height = h;
		}
		void SetWindowName(const std::string& name) { m_name = name; }
		std::string GetWindowName() { return m_name; }

		/*
		InitWindow 用于设置好几本参数后，创建窗口系统
		*/
		/*virtual bool InitWindow();*/
	protected:
		std::string m_name;
		int m_width;
		int m_height;
	};
}
#endif // RENDERER_INCLUDE_RENDERSYSTEM_WINDOWBASE_H