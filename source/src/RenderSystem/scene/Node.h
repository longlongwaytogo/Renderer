//////////////////////////////////////////////////////////////////////////
//@file: Node.h
//@brief: 场景节点类，用于组织场景
//@author: szh
//@date: 2018/12/26

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_NODE_H
#define RENDERER_INCLUDE_RENDERSYSTEM_NODE_H

#include <memory>
#include <RenderSystem/Export.h>
#include <Comm/model.h>
#include <glm/matrix.hpp>

namespace RenderSystem {

	

	class RENDERSYSTEM_API Node
	{
	public:
		Node();

		struct Counter
		{
			Counter() :_num(0)
			{}

			int Plus()
			{
				return _num++;
			}

			int _num;
		};
		Model* GetModel() { return m_model.get(); }
		//bool Node::LoadModel(std::shared_ptr<Model> model, glm::mat4 m);

		bool SetModel(std::shared_ptr<Model> model, glm::mat4 m = glm::mat4());
		int GetID();
	public:
		static Counter m_counter;
	private:
		std::shared_ptr<Model> m_model;
		glm::mat4 m_matrix;
		bool m_bIsLoad;
	};
}
#endif // RENDERER_INCLUDE_RENDERSYSTEM_NODE_H