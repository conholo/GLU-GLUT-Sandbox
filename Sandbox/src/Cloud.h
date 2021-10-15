#pragma once

#include "Core/Geometry/Geometry.h"
#include <glm/glm.hpp>

class Cloud
{
public:

	Cloud(const glm::vec3& position, float radius, uint32_t count);
	~Cloud();

	void Draw();

private:
	uint32_t m_BallCount = 10;
	int CloudDrawList = -1;
	float m_Radius = 1;
	glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };

	glm::vec3 m_DarkerColor{ .8f, .8f, .8f };
	glm::vec3 m_White{ 1.0f, 1.0f, 1.0f };
};