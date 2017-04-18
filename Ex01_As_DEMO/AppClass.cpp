#include "AppClass.h"
void AppClass::Update(void)
{
#pragma region DOES NOT NEED CHANGES
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
	static float fTimer = 0.0f;//creates a timer
	static uint uClock = m_pSystem->GenClock();//ask the system for a new clock id
	float fDeltaTime = m_pSystem->LapClock(uClock);//lap the selected clock
	fTimer += fDeltaTime;//add the delta time to the current clock
#pragma endregion

#pragma region YOUR CODE GOES HERE
	static vector3 v3Start(0.0f, 0.0f, 0.0f); //start point
	static vector3 v3End(0.0f, 3.0f, 0.0f); //end point
	float fTotalTime = 2.0f; //total Time the animation will last
	float fPercentage = MapValue(fTimer, 0.0f, fTotalTime, 0.0f, 1.0f); //percentage of the time used

	static quaternion q1 = glm::angleAxis(0.0f, vector3(0.0f, 0.0f, 1.0f)); //quaternion 1
	static quaternion q2 = glm::angleAxis(180.0f, vector3(0.0f, 0.0f, 1.0f)); //quaternion 2
	
	modelMatrix = glm::translate(glm::lerp(v3Start, v3End, fPercentage)) * 
		ToMatrix4(glm::mix(q1, q2, fPercentage * 2.0f)); //translate then rotate

	if (fPercentage > 1.0f) //when the animation is done reestart the conditions
	{
		std::swap(v3Start, v3End);
		fTimer = 0.0f;
	}
#pragma endregion

#pragma region DOES NOT NEED CHANGES
	m_pMeshMngr->SetModelMatrix(modelMatrix, 0);//Set the model matrix to the model

	m_pMeshMngr->AddSkyboxToRenderList();//Adds a skybox to the renderlist
	m_pMeshMngr->AddInstanceToRenderList("ALL"); //Adds all loaded instances to the render list

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("\n" + m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->PrintLine("Timer: ");
	m_pMeshMngr->PrintLine(std::to_string(fTimer), REGREEN);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

