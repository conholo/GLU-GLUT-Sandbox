#include "Cloud.h"
#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include <iostream>

#include <glm/gtx/compatibility.hpp>
#include "Core/Geometry/Vertex.h"
#include "Core/Random.h"



Cloud::Cloud(const glm::vec3& position, float radius, uint32_t count)
	:m_Position(position), m_Radius(radius), m_BallCount(count)
{
	Core::Geometry* sphere = Core::Geometry::Create(Core::PrimitiveType::Icosphere);

	CloudDrawList = glGenLists(1);
	glNewList(CloudDrawList, GL_COMPILE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (uint32_t i = 0; i < m_BallCount; i++)
	{
		glm::vec3 randomPositionInSphere = m_Position + glm::vec3(
			Core::Random::RandomRange(-m_Radius, m_Radius), 
			Core::Random::RandomRange(-m_Radius, m_Radius), 
			Core::Random::RandomRange(-m_Radius, m_Radius));
		const float distance = glm::length(m_Position - randomPositionInSphere);

		// Bigger towards the center, lighter towards the center.
		glm::vec3 randomScale = glm::lerp(glm::vec3(.5f), glm::vec3(3.0f), 1.5f - distance / m_Radius);
		glm::vec3 randomColor = glm::lerp(m_DarkerColor, m_White, 1.0f - distance / m_Radius);


		glPushMatrix();
		glColor3f(randomColor.r, randomColor.g, randomColor.b);
		glTranslatef(randomPositionInSphere.x, randomPositionInSphere.y, randomPositionInSphere.z);
		glScalef(randomScale.x, randomScale.y, randomScale.z);

		glBegin(GL_TRIANGLES);
		for (auto index : sphere->GetIndices())
		{
			Core::Vertex vertex = sphere->GetVertices()[index];
			glVertex3f(vertex.Position.x, vertex.Position.y, vertex.Position.z);
			glNormal3f(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);
		}
		glEnd();
		glPopMatrix();
	}
	glEndList();

	delete sphere;
}

Cloud::~Cloud()
{

}

void Cloud::Draw()
{
	glCallList(CloudDrawList);
}

