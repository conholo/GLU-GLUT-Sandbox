#include "TestLayer.h"
#include <iostream>

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"

#include "Core/Input.h"
#include "Core/OrbitalControls.h"


TestLayer::TestLayer()
{

}

TestLayer::~TestLayer()
{
}

void TestLayer::OnAttach()
{
	m_Pyramid = new Pyramid;
}

void TestLayer::OnDetach()
{
}

void TestLayer::OnDisplay(float dt)
{
	Core::OrbitalControls::UpdateViewProjection(Core::ProjectionType::Perspective, dt);

	glBegin(GL_TRIANGLES);
	for (const auto& triangle : m_Pyramid->GetTriangles())
	{
		glColor3f(triangle.A.Color.x, triangle.A.Color.y, triangle.A.Color.z);
		glVertex3f(triangle.A.Position.x, triangle.A.Position.y, triangle.A.Position.z);
		glColor3f(triangle.B.Color.x, triangle.B.Color.y, triangle.B.Color.z);
		glVertex3f(triangle.B.Position.x, triangle.B.Position.y, triangle.B.Position.z);
		glColor3f(triangle.C.Color.x, triangle.C.Color.y, triangle.C.Color.z);
		glVertex3f(triangle.C.Position.x, triangle.C.Position.y, triangle.C.Position.z);
	}
	glEnd();
}

void TestLayer::OnAnimate(float dt)
{
}

void TestLayer::OnEvent(Core::Event& event)
{
}

