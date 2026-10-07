#include "pch.h"

App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);
}

App::~App()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{

	XMFLOAT3 outsideCircleColor = {0,255,0};
	XMFLOAT3 sphereColor = { 255,0,255 };

	//Camera
	cpuEngine.GetCamera()->transform.pos.y = -2.f;
	cpuEngine.GetCamera()->transform.AddYPR(0.0f, XM_PI * 0.2);


	//Mesh
	m_circle.CreateCircle(1.0f,32, outsideCircleColor);
	m_Scircle.CreateCircle(0.80f, 32);
	m_rock.CreateSphere(0.05f, 5.0f, 5.0f);

	m_sphere.CreateSphere(0.1f,10.f,10.f, sphereColor, sphereColor);


	//Entity
	
	m_pCircle = cpuEngine.CreateEntity();
	m_pCircle->pMesh = &m_circle;

	m_pSCircle = cpuEngine.CreateEntity();
	m_pSCircle->pMesh = &m_Scircle;
	m_pSCircle->transform.pos.y = 0.01f;
	
	m_pSphere = cpuEngine.CreateEntity();
	m_pSphere->pMesh = &m_sphere;
	m_pSphere->transform.pos.y = 0.1f;
	m_pSphere->transform.pos.z = - 0.9f;



	/*
	m_rail.CreateCylinder(railLength * 0.5f);

	

	int railCount = 8;
	float step = railLength / railCount;

	for (int i = 0; i < railCount; i++)
	{
		m_pRail = cpuEngine.CreateEntity();
		m_pRail->pMesh = &m_rail;
		m_pRail->transform.Move(1.5f);
		m_pRail->transform.AddYPR(XM_PI * step, 0.0f, XM_PI * 0.5f);
	}

	*/
}

void App::OnUpdate()
{
	float dt = cpuTime.delta;
	float time = cpuTime.total;
	

	if (cpuInput.IsDown())
		cpuEngine.GetCamera()->transform.Move(dt * -1.0f);
	if (cpuInput.IsUp())
		cpuEngine.GetCamera()->transform.Move(dt * 1.0f);
	if (cpuInput.IsLeft())
		m_angle += dt * XM_PI;
	if (cpuInput.IsRight())
		m_angle += -dt * XM_PI;

	m_delay += dt;
	if (m_delay >= 1.0)
	{
		cpu_entity* pTmpRock = cpuEngine.CreateEntity();
		pTmpRock->pMesh = &m_rock;
		pTmpRock->transform.OrbitAroundAxis(m_pCircle->transform.pos, CPU_VEC3_UP, 0.9f,  -m_angle);
		pTmpRock->transform.pos.y = 3.0f;

		m_pRock.push_back(pTmpRock);
		m_delay = 0.0f;
	}


	//Orbite de la sphere
	m_pSphere->transform.OrbitAroundAxis(m_pCircle->transform.pos, CPU_VEC3_UP, 0.9f, m_angle);
	m_pSphere->transform.pos.y = 0.1f;
	
	//Orbite de la caméra 
	XMFLOAT3 pos = { m_pSphere->transform.pos.x,				//Position de la sphere  en X
					 2.0f,										//Hauteur de la camera   en Y
					 m_pSphere->transform.pos.z };				//Position de la sphere  en Z
	cpuEngine.GetCamera()->transform.OrbitAroundAxis(pos, CPU_VEC3_UP, 2.0f, m_angle);
	cpuEngine.GetCamera()->transform.LookAt(pos.x, 1.0f, pos.z);
	
	for (auto i = m_pRock.begin(); i != m_pRock.end(); ++i)
	{
		cpu_entity* pRock = *i;
		pRock->transform.pos.y -= dt * 2.0f;
		if (pRock->lifetime > 10.0f)
			cpuEngine.Release(pRock);
	}

	for (auto it = m_pRock.begin(); it != m_pRock.end(); )
	{
		if ((*it)->dead)
			it = m_pRock.erase(it);
		else
			++it;
	}

	if (cpuInput.IsBackPressed())
		cpuEngine.Quit();

}

void App::OnExit()
{
	// YOUR CODE HERE
}

void App::OnRender(int pass)
{
	// YOUR CODE HERE
}

void App::MyPixelShader(cpu_ps_io& io)
{
	// YOUR CODE HERE
	io.color = io.p.color;
}
