#include "Project3Layer.h"
#include <glut.h>

#include <glm/glm.hpp>

Project3Layer::Project3Layer()
	:Layer("Project 3")
{

}

Project3Layer::~Project3Layer()
{
	delete m_Texture;
}

Project3Layer* Project3Layer::s_Instance = nullptr;

void Project3Layer::CreateGlobeList()
{
	m_Geometry = Core::Geometry::Create(Core::PrimitiveType::Sphere);
	m_GeometryList = glGenLists(1);

	glNewList(m_GeometryList, GL_COMPILE);

	glBegin(GL_TRIANGLES);

	for (auto index : m_Geometry->GetIndices())
	{
		Core::Vertex vertex = m_Geometry->GetVertices()[index];

		glTexCoord2f(vertex.TexCoord.s, vertex.TexCoord.t);
		glNormal3f(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);
		glVertex3f(vertex.Position.x, vertex.Position.y, vertex.Position.z);
	}

	glEnd();

	glEndList();
}


float random(const glm::vec2& st)
{
	return glm::fract(glm::sin(glm::dot(st, glm::vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise(const glm::vec2& st)
{
	float intPartX, fractPartX;
	float intPartY, fractPartY;
	fractPartX = modf(st.x, &intPartX);
	fractPartY = modf(st.y, &intPartY);

	glm::vec2 fractPart = { fractPartX, fractPartY };
	glm::vec2 intPart = { intPartX, intPartY };

	// Four corners in 2D of a tile
	float a = random(intPart);
	float b = random(intPart + glm::vec2(1.0, 0.0));
	float c = random(intPart + glm::vec2(0.0, 1.0));
	float d = random(intPart + glm::vec2(1.0, 1.0));

	glm::vec2 u = fractPart * fractPart * (glm::vec2(3.0) - glm::vec2(2.0) * fractPart);

	return Utility::Lerp(a, b, u.x) +
		(c - a) * u.y * (1.0 - u.x) +
		(d - b) * u.x * u.y;
}

float fbm(const glm::vec2& st, int octaves)
{
	float rotationMatrix[4] =
	{
		cos(0.5), -sin(0.5),
		sin(0.5), cos(0.5)
	};

	float value = 0.0;
	float amplitude = 1.0f;
	float frequency = 1.0f;

	float persistence = 0.2f;
	float lacunarity = 32.0f;

	glm::vec2 position = st;

	for (int i = 0; i < octaves; i++)
	{
		value += amplitude * noise(position);
		position *= frequency;

		frequency *= lacunarity;
		amplitude *= persistence;
	}
	return value;
}


void Project3Layer::DisplayDistortedGlobe()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_Texture->GetTextureID());
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	float rotationMatrix[4] =
	{
		cos(m_Counter), -sin(m_Counter),
		sin(m_Counter), cos(m_Counter)
	};

	for (auto& vertex : m_GeometryB->GetVertices())
	{
		float pctX = Utility::SmootherStep(sin(m_Counter * vertex.Position.x) * 0.5f + 0.5f, 0.0f, 1.0f);
		float pctY = Utility::SmootherStep(sin(m_Counter * vertex.Position.y) * 0.5f + 0.5f, 0.0f, 1.0f);
		vertex.TexCoord.s = fbm({ pctX, pctY }, 4);
		vertex.TexCoord.t = fbm({ pctY, pctX }, 4);;
	}

	glBegin(GL_TRIANGLES);

	for (auto index : m_GeometryB->GetIndices())
	{
		auto& vertex = m_GeometryB->GetVertices()[index];

		glTexCoord2f(vertex.TexCoord.s, vertex.TexCoord.t);
		glVertex3f(vertex.Position.x, vertex.Position.y, vertex.Position.z);
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Project3Layer::DisplayNormalGlobe()
{
	if (m_Textured)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Texture->GetTextureID());
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glMatrixMode(GL_MODELVIEW);
	}
	else
	{
		float r = Utility::Lerp(0.0f, 0.5f, sin(m_Counter) * 0.5 + 0.5);
		float g = Utility::Lerp(0.5f, 1.0f, sin(m_Counter) * 0.5 + 0.5);
		float b = Utility::Lerp(0.8, 1.0f, sin(m_Counter) * 0.5 + 0.5);
		glColor3f(r, g, b);
	}

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glCallList(m_GeometryList);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Project3Layer::OnAttach()
{
	s_Instance = this;
	m_Texture = new Core::Texture("src/assets/textures/worldTex.bmp");

	m_Geometry = Core::Geometry::Create(Core::PrimitiveType::Sphere);
	m_GeometryB = Core::Geometry::Create(Core::PrimitiveType::Sphere);
	m_GeometryList = glGenLists(1);

	CreateGlobeList();
	CreateGUI();
}


void Project3Layer::OnDetach()
{

}


void Project3Layer::DisplayGlobe()
{
	if (m_Distort)
		DisplayDistortedGlobe();
	else
		DisplayNormalGlobe();

}

void Project3Layer::OpenMainMenu(int mode)
{
	glutSetWindow(Core::Window::GetMainWindow());
	glutPostRedisplay();
}


void Project3Layer::OnDisplayModeChanged(int mode)
{
	if (mode == 0)
	{
		s_Instance->m_Textured = false;
		s_Instance->m_Distort = false;
	}
	else if (mode == 1)
	{
		s_Instance->m_Textured = true;
		s_Instance->m_Distort = false;
	}
	else if (mode == 2)
	{
		s_Instance->m_Textured = true;
		s_Instance->m_Distort = true;
		s_Instance->m_Counter = 0.0f;
	}

	glutSetWindow(Core::Window::GetMainWindow());
	glutPostRedisplay();
}

void Project3Layer::CreateGUI()
{
	glutSetWindow(Core::Window::GetMainWindow());

	int textureSettingsMenu = glutCreateMenu(OnDisplayModeChanged);
	glutAddMenuEntry("Flat Color", 0);
	glutAddMenuEntry("Textured", 1);
	glutAddMenuEntry("Distorted", 2);

	int mainMenu = glutCreateMenu(OpenMainMenu);
	glutAddSubMenu("Texture Settings", textureSettingsMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Project3Layer::OnDisplay(float deltaTime)
{
	//Core::OrbitalControls::Orbit({ 0.0f, 0.0f, 6.0f }, glm::vec3(0.0f), { 0.0f, m_CameraRotation, 0.0f });
	Core::OrbitalControls::UpdateViewProjection(Core::ProjectionType::Perspective, deltaTime);
	
	DisplayGlobe();

	glutSetWindow(Core::Window::GetMainWindow());
	glutPostRedisplay();
}

void Project3Layer::OnAnimate(float deltaTime)
{
	m_CameraRotation += deltaTime * 30.0f;
	m_Counter += deltaTime;
	m_DistortionAmount = sin(m_Counter * 0.2f);
	m_DistortionPercentA = Utility::SmootherStep(m_DistortionAmount, 0.0f, 1.0f);
	m_DistortionPercentB = Utility::SmootherStep(m_DistortionAmount, -0.1f, 0.1f);


	glutSetWindow(Core::Window::GetMainWindow());
	glutPostRedisplay();
}

void Project3Layer::OnEvent(Core::Event& e)
{

}
