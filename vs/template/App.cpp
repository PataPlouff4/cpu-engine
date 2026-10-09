#include "pch.h"
#include "Rock.h"

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

void App::SpawnRock()
{
	Rock* pTmpRock;
	pTmpRock = new Rock();
	float randAngle = 2 *  XM_PI * cpu::Rand01(rand);

	pTmpRock->CreateRock(&m_rock, randAngle);
	pTmpRock->m_pEntity->transform.OrbitAroundAxis(m_pCircle->transform.pos, CPU_VEC3_UP, 0.9f, randAngle);
	pTmpRock->m_pEntity->transform.pos.y = 3.0f;

	m_pRock.push_back(pTmpRock);
}

void App::MoveRocks(float dt)
{
	for (auto it = m_pRock.begin(); it != m_pRock.end();)
	{
		Rock* pRock = *it;
		//float modF = 0.0f;
		pRock->m_pEntity->transform.pos.y -= dt * 2.0f;
		//modF = fmodf(pRock->m_pEntity->transform.pos.x, m_pSphere->transform.pos.x);

		if (pRock->m_pEntity->transform.pos.y <= m_pCircle->transform.pos.y)
		{
			pRock->CleanRock();
			it = m_pRock.erase(it);
			delete pRock;
			m_score--;
			continue;
		}
		if (cpu::SphereSphere(pRock->m_pEntity->transform.pos, pRock->m_pEntity->pMesh->radius, m_pSphere->transform.pos, m_pSphere->pMesh->radius))
		{
			pRock->CleanRock();
			it = m_pRock.erase(it);
			delete pRock;
			m_score++;
			continue;
		}
		++it;
	}
}

void App::PurgeRock()
{
	for (auto it = m_pRock.begin(); it != m_pRock.end(); )
	{
		if ((*it)->m_pEntity->dead)
			it = m_pRock.erase(it);
		else
			++it;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{
	rand = (ui32)timeGetTime();

	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);

	XMFLOAT3 outsideCircleColor = {0,255,0};
	XMFLOAT3 sphereColor = { 255,0,255 };

	//Camera
	cpuEngine.GetCamera()->transform.pos.y = -2.f;
	cpuEngine.GetCamera()->transform.AddYPR(0.0f, XM_PI * 0.2);


	//Mesh
	m_circle.CreateCircle(1.0f,32, outsideCircleColor);
	m_Scircle.CreateCircle(0.80f, 32);
	m_rock.CreateSphere(0.05f, 5.0f, 5.0f);
	m_skybox.CreateSkyBox(50.0f);
	m_sphere.CreateSphere(0.1f,10.f,10.f, sphereColor, sphereColor); 

	//Texture

	m_textureSkybox.Load("earth.png");
	m_materialSkybox.pTexture = &m_textureSkybox;

	//Entity
	
	m_pSkybox = cpuEngine.CreateEntity();
	m_pSkybox->pMesh = &m_skybox;
	m_pSkybox->pMaterial = &m_materialSkybox;
	

	m_pCircle = cpuEngine.CreateEntity();
	m_pCircle->pMesh = &m_circle;

	m_pSCircle = cpuEngine.CreateEntity();
	m_pSCircle->pMesh = &m_Scircle;
	m_pSCircle->transform.pos.y = 0.01f;
	
	m_pSphere = cpuEngine.CreateEntity();
	m_pSphere->pMesh = &m_sphere;
	m_pSphere->transform.pos.y = 0.1f;
	m_pSphere->transform.pos.z = - 0.9f;
}

void App::OnUpdate()
{
	//Time
	float dt = cpuTime.delta;
	float time = cpuTime.total;
	m_delay += dt;

	float vitesse = 0.0f;




	//Input
	if (cpuInput.IsLeft())
	{
		if (m_acceleration < 1.5 * XM_PI)
			m_acceleration += 0.5f;
		else
			m_acceleration = 1.5 * XM_PI;
	}
	if (cpuInput.IsRight())
	{
		if (m_acceleration > -1.5 * XM_PI)
			m_acceleration -= 0.5f;
		else
			m_acceleration = -1.5 * XM_PI;

	}
	if (cpuInput.IsLeft() == false && cpuInput.IsRight() == false)
	{
		if (m_acceleration > 0.3f)
		{
			m_acceleration -= 0.5f;
		}
		else if (m_acceleration < -0.3f)
		{
			m_acceleration += 0.5f;
		}
		else
		{
			m_acceleration = 0.0f;
		}
	}

	m_angle += dt * m_acceleration;


	//Spawn des cailloux
	if (m_delay >= 1.0)
	{
		SpawnRock();
		m_delay = 0.0f;
	}


	//Orbite de la sphere
	m_pSphere->transform.OrbitAroundAxis(m_pCircle->transform.pos, CPU_VEC3_UP, 0.9f, m_angle);
	m_pSphere->transform.pos.y = 0.1f;
	
	
	//Orbite de la caméra 
	XMFLOAT3 pos = { m_pSphere->transform.pos.x,				//Position de la sphere  en X
					 3.0f,										//Hauteur de la camera   en Y
					 m_pSphere->transform.pos.z };				//Position de la sphere  en Z
	cpuEngine.GetCamera()->transform.OrbitAroundAxis(pos, CPU_VEC3_UP, 2.0f, m_angle);
	cpuEngine.GetCamera()->transform.LookAt(pos.x, 1.5f, pos.z);
	

	//Mouvement des cailloux
	MoveRocks(dt);

	//Destruction des cailloux
	PurgeRock();

	if (cpuInput.IsBackPressed())
		cpuEngine.Quit();
}

void App::OnExit()
{
	// YOUR CODE HERE
	for (auto it = m_pRock.begin(); it != m_pRock.end();it++)
	{
		delete* it;
	}
	m_pRock.clear();

}

void App::OnRender(int pass)
{
	std::string stat = "Score: " + CPU_STR(m_score);

	XMFLOAT3 tint = { 1.0f, 1.0f, 0.8f };
	cpuDevice.DrawText(&m_font, stat.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 10, CPU_TEXT_CENTER, &tint);

}

void App::MyPixelShader(cpu_ps_io& io)
{
	// YOUR CODE HERE
	io.color = io.p.color;
}

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////