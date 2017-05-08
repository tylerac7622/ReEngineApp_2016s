#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Render Tagets (Render to texture and applies it to a model)"); // Window Name
	m_v4ClearColor = vector4(RECORNFLOWERBLUE, 0.0f);
	//m_pSystem->SetWindowFullscreen(RESOLUTIONS::C_1920x1080_16x9_FULLHD);
}

void AppClass::InitVariables(void)
{
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 0.0f, 15.0f),//Camera position
		vector3(0.0f, 0.0f, 0.0f),//What Im looking at
		REAXISY);//What is up

	//Create a new camera to look at the scene rendered onto a texture
	m_nSecondaryCamera = m_pCameraMngr->AddCamera(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What Im looking at
		REAXISY);

	//Load a models onto the Mesh manager
	m_pMeshMngr->LoadModel("Lego\\Unikitty.bto", "Unikitty");
	m_pMeshMngr->LoadModel("Sorted\\Monitor.fbx", "Monitor");
	m_pMeshMngr->LoadModel("Sorted\\Monitor.fbx", "Monitor2");
	m_pMeshMngr->LoadModel("Sorted\\Monitor.fbx", "Monitor3");
	
	//Generate a new render target (the texture I will render into)
	m_pGLSystem->GenerateRenderTarget(m_nFrameBuffer1, m_nDepthBuffer1, m_nTexture1);

	/*
		I need to instance (read duplicate) the materials on each model instance I want to modify
		if I don't the changes I do to a single material will happen to all of them
	*/
	m_pMeshMngr->DuplicateMaterialsOnInstance("Monitor");
	m_pMeshMngr->DuplicateMaterialsOnInstance("Monitor2");
	m_pMeshMngr->DuplicateMaterialsOnInstance("Monitor3");
	/*
		Once I instance the materials I want to change the textures associated with them, I can get the material
		from the instance OR I can just notify the Mesh manager I want to change the diffuse on the material, this
		also creates a new copy to the material manager with the original in case I want to go back to it, the word
		Original (capitalized) is a wildcard to maintain the current texture associated with it
	*/
	m_pMeshMngr->ModifyMaterialOnInstance("Monitor", "M_Display", "M_Display2", "3_Earth.png", "BrickNormal.png", "Original");
	m_pMeshMngr->ModifyMaterialOnInstance("Monitor2", "M_Display", "M_Display2", "cloud.png");
	

	/*
		This one is a bit tricky, I'm getting a MaterialManager pointer and then getting the material named M_Display from
		the instance Monitor3 that material pointer is later modified with the buffer I created as a render target. 
		Notice that if I changed the material using ModifyMaterial the name should have changed as well.
	*/
	MaterialManagerSingleton* pMaterialMngr = MaterialManagerSingleton::GetInstance();
	MaterialClass* pMaterial = m_pMeshMngr->GetMaterialOnInstance("Monitor3", "M_Display");
	//Making sure the material exists
	if(pMaterial != nullptr)
		pMaterial->SetDiffuseMapID(m_nTexture1);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//Call the arcball method
	ArcBall();

	/*
		Set the model matrix for the objects, one of them is right at the center, notice
		The unikitty is located at the center of the world no matter where monitor3 is 
		located, and if I transform it it will move in the relation to that coordinate system.
	*/
	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), "Unikitty");
	m_pMeshMngr->SetModelMatrix(glm::translate(vector3(-5, 0, 0)), "Monitor2");
	m_pMeshMngr->SetModelMatrix(glm::translate(vector3(5, 0, 0)), "Monitor3");
}

void AppClass::Display(void)
{
	/*
		Display will render the scene twice in this project, one for the Unikitty scene and 
		one for the scene with the 3 monitors (one displaying the Unikitty on it)
		First step is to set the render target, to write to a texture I need a fram buffer, a depth buffer
		and the texture ID.
		Right after setting the render target I need to clear the screen(or active target)
		and then select the camera I want to use to project, It can be the same camera, but that will look
		funny, as both of the scenes are controlled by the same movements
	*/
	m_pMeshMngr->SetRenderTarget(m_nFrameBuffer1, m_nDepthBuffer1, m_nTexture1);
	ClearScreen();
	m_pCameraMngr->SetActiveCamera(m_nSecondaryCamera);

	//After that I say what I want to be displayed on my scene and clean the render list so I can use it again
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->AddInstanceToRenderList("Unikitty");
	m_pMeshMngr->Render();
	m_pMeshMngr->ClearRenderList();
	
	/*
		The second Scene is pretty much the same thing, I send to arguments to SetRenderTarget because the
		defaults are to render onto the screen. I still need to clean the target (screen) and set the camera active
	*/
	m_pMeshMngr->SetRenderTarget();
	ClearScreen();
	m_pCameraMngr->SetActiveCamera();
	
	//To have a FP control
	if (m_bFPC == true) 
		CameraRotation();
	
	//Render the Content of the second scene, notice that I do not need to change the content of Monitor 3 again
	m_pMeshMngr->AddInstanceToRenderList("Monitor");
	m_pMeshMngr->AddInstanceToRenderList("Monitor2");
	m_pMeshMngr->AddInstanceToRenderList("Monitor3");
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//Some debugging information
	int nFPS = m_pSystem->GetFPS();
	//m_pMeshMngr->PrintLine("");
	//m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	//m_pMeshMngr->Print("FPS:");
	//m_pMeshMngr->Print(std::to_string(nFPS), RERED);
	
	//Once again render the scene and clean the render list
	m_pMeshMngr->Render();
	m_pMeshMngr->ClearRenderList();
	
	m_pGLSystem->GLSwapBuffers(); //You only swaps the OpenGL buffers once
}

void AppClass::Release(void)
{
	glDeleteFramebuffers(1, &m_nFrameBuffer1);
	glDeleteTextures(1, &m_nDepthBuffer1);
	glDeleteRenderbuffers(1, &m_nTexture1);
	super::Release(); //release the memory of the inherited fields
}