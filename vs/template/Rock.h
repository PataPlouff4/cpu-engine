#pragma once

class Rock
{
public:
	cpu_entity* m_pEntity;
	float m_angle;

public:

	void CreateRock(cpu_mesh* mesh, float angle);
	void CleanRock();

};

