#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("DSA2 Final: DEMO"); // Window Name
	m_pSystem->SetWindowWidth(800);
	m_pSystem->SetWindowHeight(800);
}

void AppClass::InitVariables(void)
{
	m_pBOMngr = MyBOManager::GetInstance();
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 0.0f, 45.0f),//Camera position
		vector3(0.0f, 0.0f, 0.0f),//What Im looking at
		REAXISY);//What is up
	//Load a model onto the Mesh manager
	vector3 v3Position = vector3(GenerateRandom(-10, 10), GenerateRandom(-10, 10), GenerateRandom(-10, 10));
	matrix4 m4Position = glm::translate(v3Position);
	quaternion qOrientation = quaternion(vector3(	glm::radians(GenerateRandom(0, 180)),
													glm::radians(GenerateRandom(0, 180)),
													glm::radians(GenerateRandom(0, 180))));
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper", false, m4Position * ToMatrix4(qOrientation) );
	m_pBOMngr->AddObject("Creeper");

	v3Position = vector3(GenerateRandom(-10, 10), GenerateRandom(-10, 10), GenerateRandom(-10, 10));
	m4Position = glm::translate(v3Position);
	qOrientation = quaternion(vector3(	glm::radians(GenerateRandom(0, 180)),
										glm::radians(GenerateRandom(0, 180)),
										glm::radians(GenerateRandom(0, 180))));
	m_pMeshMngr->LoadModel("Minecraft\\Pig.obj", "Pig", false, m4Position * ToMatrix4(qOrientation));
	m_pBOMngr->AddObject("Pig");

	v3Position = vector3(GenerateRandom(-10, 10), GenerateRandom(-10, 10), GenerateRandom(-10, 10));
	m4Position = glm::translate(v3Position);
	qOrientation = quaternion(vector3(	glm::radians(GenerateRandom(0, 180)),
										glm::radians(GenerateRandom(0, 180)),
										glm::radians(GenerateRandom(0, 180))));
	m_pMeshMngr->LoadModel("Minecraft\\Cow.obj", "Cow", false, m4Position * ToMatrix4(qOrientation));
	m_pBOMngr->AddObject("Cow");

	v3Position = vector3(GenerateRandom(-10, 10), GenerateRandom(-10, 10), GenerateRandom(-10, 10));
	m4Position = glm::translate(v3Position);
	qOrientation = quaternion(vector3(	glm::radians(GenerateRandom(0, 180)),
										glm::radians(GenerateRandom(0, 180)),
										glm::radians(GenerateRandom(0, 180))));
	m_pMeshMngr->LoadModel("Minecraft\\Zombie.obj", "Zombie", false, m4Position * ToMatrix4(qOrientation));
	m_pBOMngr->AddObject("Zombie");

	v3Position = vector3(GenerateRandom(-10, 10), GenerateRandom(-10, 10), GenerateRandom(-10, 10));
	m4Position = glm::translate(v3Position);
	qOrientation = quaternion(vector3(	glm::radians(GenerateRandom(0, 180)),
										glm::radians(GenerateRandom(0, 180)),
										glm::radians(GenerateRandom(0, 180))));
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve", false, m4Position * ToMatrix4(qOrientation));
	m_pBOMngr->AddObject("Steve");

	m_pOctreeRoot = new MyOctant();
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();
	
	//Adds all loaded instances to the render list
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	m_pBOMngr->DisplayOriented("ALL", REYELLOW);
	m_pBOMngr->DisplayReAlligned("ALL", RERED);

	m_pOctreeRoot->Display();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_pOctreeRoot);
	m_pBOMngr->ReleaseInstance();
	super::Release(); //release the memory of the inherited fields
}