#pragma once

class Rock
{
public:
	cpu_entity* m_pEntity;
	float m_angle;

public:

	void CreateRock(cpu_mesh* mesh, float angle);
	void DeleteRock();

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

	void SpawnRock();
	void MoveRocks(float dt);
	void PurgeRock();

	static void MyPixelShader(cpu_ps_io& io);

private:
	inline static App* s_pApp = nullptr;
	cpu_rt* m_rts[1];

	//Ressources
	cpu_mesh m_rail;
	cpu_mesh m_circle;
	cpu_mesh m_Scircle;
	cpu_mesh m_sphere;
	cpu_mesh m_rock;
	cpu_mesh m_skybox;

	cpu_texture m_textureSkybox;

	cpu_font m_font;


	//3D
	cpu_entity* m_pRail;
	std::list<cpu_entity*> m_pRing;
	cpu_entity* m_pCircle;
	cpu_entity* m_pSCircle;
	cpu_entity* m_pSphere;
	cpu_entity* m_pSkybox;
	std::list<Rock*> m_pRock;

	float m_spawnRate = 1.0f;
	float m_delay = 0.0f;
	float m_angle = 0.f;

	int m_score = 0;
	ui32 rand;


};
