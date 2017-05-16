#include "MyOctant.h"
using namespace ReEng;
//  MyOctant
void MyOctant::Init(void)
{
	m_v3Position = vector3(0.0f);
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_fSize = 0.0f;
	m_pBOMngr = MyBOManager::GetInstance();

	int nObjectCont = m_pBOMngr->GetObjectCount();
	//YOUR CODE GOES HERE
}
void MyOctant::Release(void)
{
}
//The big 3
MyOctant::MyOctant(){Init();}
MyOctant::~MyOctant(){Release();};

void MyOctant::Display(void)
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(m_v3Position) * glm::scale(vector3(m_fSize)), REPURPLE, WIRE);
}


