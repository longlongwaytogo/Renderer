//////////////////////////////////////////////////////////////////////////
//@file: SceneManager.h
//@brief: 场景管理类
//@author: szh
//@date: 2018/12/26

#ifndef RENDER_INCLUDE_RENDERSYTEM_SCENEMANAGER_H
#define RENDER_INCLUDE_RENDERSYTEM_SCENEMANAGER_H

#include <RenderSystem/Export.h>
#include <map>
#include <memory>
#include <RenderSystem/scene/Node.h>
namespace RenderSystem
{
	class RENDERSYSTEM_API SceneManager
	{
	public:
		enum RenderOrder {
			RENDER_NORMAL = 0,
			RENDER_FIRST = 1,
			RENDER_LAST = 2,
		};

		static SceneManager& GetInstance();

		void AddNode(Node* node, RenderOrder order = RENDER_NORMAL);

		void CreateScene();

		std::map<int, std::shared_ptr<Node> > m_normalNodes;
		std::map<int, std::shared_ptr<Node> > m_firstNodes;
		std::map<int, std::shared_ptr<Node> > m_lastNodes;

	private:

	};
}
#endif // RENDER_INCLUDE_RENDERSYTEM_SCENEMANAGER_H