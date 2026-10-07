#pragma once

class Rock
{
public:
	cpu_entity* m_pEntity;
	float m_angle;

public:
	void CreateRock(cpu_mesh* mesh, float angle);

};

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);

	static void MyPixelShader(cpu_ps_io& io);

private:
	inline static App* s_pApp = nullptr;

	//Ressources
	cpu_mesh m_rail;
	cpu_mesh m_circle;
	cpu_mesh m_Scircle;
	cpu_mesh m_sphere;
	cpu_mesh m_rock;

	cpu_font m_font;

	//3D
	cpu_entity* m_pRail;
	std::list<cpu_entity*> m_pRing;
	cpu_entity* m_pCircle;
	cpu_entity* m_pSCircle;
	cpu_entity* m_pSphere;
	std::list<Rock*> m_pRock;

	float m_spawnRate = 1.0f;
	float m_delay = 0.0f;
	float m_angle = 0.f;

	int m_score = 0;
	ui32 rand;


};
