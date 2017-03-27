#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - Tyler Coppenbarger"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");
	m_pMeshMngr->LoadModel("Planets\\02_Venus.obj", "Venus");

	//Setting the days duration
	m_fDay = 1.0f;
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

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	matrix4 m4Sun = IDENTITY_M4;
	matrix4 m4Earth = IDENTITY_M4;
	matrix4 m4Moon = IDENTITY_M4;
	matrix4 m4Venus = IDENTITY_M4;

	static int nEarthOrbits = 0;
	static int nEarthRevolutions = 0;
	static int nMoonOrbits = 0;

	static int nVenusOrbits = 0;
	static int nVenusRevolutions = 0;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	double fVenusHalfOrbTime = 113.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fVenusHalfRevTime = 121.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops

	//Earth Orbit
	glm::quat q1 = glm::angleAxis(0.0f, vector3(0.0f, 1.0f, 0.0f));
	quaternion q2 = glm::angleAxis(359.0f, vector3(0.0f, 1.0f, 0.0f));
	float fPercentage = MapValue((float)fRunTime - (float)(fEarthHalfOrbTime * 2 * nEarthOrbits), 0.0f, (float)fEarthHalfOrbTime*2, 0.0f, 1.0f); //map the value in a percentage scale
	quaternion q3 = glm::mix(q1, q2, fPercentage); //slerp the quaternions

	//Earth Revolution
	glm::quat q4 = glm::angleAxis(0.0f, vector3(0.0f, 1.0f, 0.0f));
	quaternion q5 = glm::angleAxis(359.0f, vector3(0.0f, 1.0f, 0.0f));
	float fPercentage2 = MapValue((float)fRunTime - (fEarthHalfRevTime * 2 * nEarthRevolutions), 0.0f, (float)fEarthHalfRevTime*2, 0.0f, 1.0f); //map the value in a percentage scale
	quaternion q6 = glm::mix(q4, q5, fPercentage2); //slerp the quaternions

	//Moon Orbit
	glm::quat q7 = glm::angleAxis(0.0f, vector3(0.0f, 1.0f, 0.0f));
	quaternion q8 = glm::angleAxis(359.0f, vector3(0.0f, 1.0f, 0.0f));
	float fPercentage3 = MapValue((float)fRunTime - (fMoonHalfOrbTime * 2 * nMoonOrbits), 0.0f, (float)fMoonHalfOrbTime * 2, 0.0f, 1.0f); //map the value in a percentage scale
	quaternion q9 = glm::mix(q7, q8, fPercentage3); //slerp the quaternions

	//Venus Orbit
	glm::quat q10 = glm::angleAxis(0.0f, vector3(0.0f, 1.0f, 0.0f));
	quaternion q11 = glm::angleAxis(359.0f, vector3(0.0f, 1.0f, 0.0f));
	float fPercentage4 = MapValue((float)fRunTime - (float)(fVenusHalfOrbTime * 2 * nVenusOrbits), 0.0f, (float)fVenusHalfOrbTime * 2, 0.0f, 1.0f); //map the value in a percentage scale
	quaternion q12 = glm::mix(q1, q2, fPercentage4); //slerp the quaternions

	//Venus Revolution
	glm::quat q13 = glm::angleAxis(0.0f, vector3(0.0f, 1.0f, 0.0f));
	quaternion q14 = glm::angleAxis(359.0f, vector3(0.0f, 1.0f, 0.0f));
	float fPercentage5 = MapValue((float)fRunTime - (fVenusHalfRevTime * 2 * nVenusRevolutions), 0.0f, (float)fVenusHalfRevTime * 2, 0.0f, 1.0f); //map the value in a percentage scale
	quaternion q15 = glm::mix(q4, q5, fPercentage5); //slerp the quaternions

	m4Sun = glm::translate(sunPos);

	m4Earth *= m4Sun;
	m4Earth *= ToMatrix4(q3);
	m4Earth *= glm::translate(15.0f, 0.0f, 0.0f);
	m4Moon *= m4Earth;
	m4Moon *= ToMatrix4(q9);
	m4Moon *= glm::translate(3.0f, 0.0f, 0.0f);
	m4Earth *= ToMatrix4(q6);

	m4Venus *= m4Sun;
	m4Venus *= ToMatrix4(q12);
	m4Venus *= glm::translate(10.0f, 0.0f, 0.0f);
	m4Venus *= ToMatrix4(q15);

	m4Moon *= glm::scale(0.3f, 0.3f, 0.3f);
	m4Earth *= glm::scale(0.7f, 0.7f, 0.7f);
	m4Sun *= glm::scale(5.0f, 5.0f, 5.0f);
	m4Venus *= glm::scale(.5f, 0.5f, 0.5f);

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m4Moon, "Moon");
	m_pMeshMngr->SetModelMatrix(m4Venus, "Venus");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	if (fRunTime > (fEarthHalfOrbTime * 2)*(nEarthOrbits+1))
	{
		nEarthOrbits++;
	}
	if (fRunTime > (fEarthHalfRevTime * 2)*(nEarthRevolutions + 1))
	{
		nEarthRevolutions++;
	}
	if (fRunTime > (fMoonHalfOrbTime * 2)*(nMoonOrbits + 1))
	{
		nMoonOrbits++;
	}
	if (fRunTime > (fVenusHalfOrbTime * 2)*(nVenusOrbits + 1))
	{
		nVenusOrbits++;
	}
	if (fRunTime > (fVenusHalfRevTime * 2)*(nVenusRevolutions + 1))
	{
		nVenusRevolutions++;
	}

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("V_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nVenusOrbits));

	m_pMeshMngr->Print("V_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nVenusRevolutions));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	
	m_pMeshMngr->Render(); //renders the render list

	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}