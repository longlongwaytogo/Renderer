#include <RenderSystem/scene/SceneManager.h>
#include <Comm/filesystem.h>
#include <glm/gtc/matrix_transform.hpp>

RenderSystem::SceneManager& RenderSystem::SceneManager::GetInstance()
{
	static SceneManager s_sceneMgr;
	return s_sceneMgr;
}

void RenderSystem::SceneManager::AddNode(Node* node, RenderOrder order /*= RENDER_NORMAL*/)
{
	int id = node->GetID();
	switch (order)
	{
	case RENDER_NORMAL:
		m_normalNodes.insert(std::make_pair(id, node));
		break;
	case RENDER_FIRST:
		m_firstNodes.insert(std::make_pair(id, node));
		break;
	case RENDER_LAST:
		m_lastNodes.insert(std::make_pair(id, node));
		break;
	/*case RENDER_NORMAL:*/
	default:
		m_normalNodes.insert(std::make_pair(id, node));
		break;
	}
}

void RenderSystem::SceneManager::CreateScene()
{

	std::shared_ptr<Model> model = std::make_shared<Model>(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));

	std::vector<glm::vec3> objectPositions;
	objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));

	for (int i = 0; i < objectPositions.size(); ++i)
	{
		Node* node = new Node;
		glm::mat4 matrix;
		node->SetModel(model, glm::translate(matrix,objectPositions[i]));
		AddNode(node);
	}
 
}


