#include <RenderSystem/scene/Node.h>

namespace RenderSystem
{

	Node::Node():m_bIsLoad(false)
	{

	}

	//bool Node::LoadModel(std::shared_ptr<Model> model, glm::mat4 m)
	//{
	//	m_matrix = m;
	//	m_model = model;

	//	m_bIsLoad = true;
	//	return true;
	//}

	bool Node::SetModel(std::shared_ptr<Model> model, glm::mat4 m /*= glm::mat4()*/)
	{
		m_matrix = m;
		m_model = model;

		m_bIsLoad = true;
		return true;
	}

	int Node::GetID()
	{
		return m_counter.Plus();
	}
	Node::Counter Node::m_counter;
}

