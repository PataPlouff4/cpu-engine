#include "pch.h"
#include "Rock.h"


void Rock::CreateRock(cpu_mesh* mesh, float angle)
{
	cpu_entity* pTmpEntity = cpuEngine.CreateEntity();
	pTmpEntity->pMesh = mesh;

	m_pEntity = pTmpEntity;

	m_angle = angle;
}

void Rock::CleanRock()
{
	if (m_pEntity)
	{
		cpuEngine.Release(m_pEntity);
		m_pEntity = nullptr;
	}
}
