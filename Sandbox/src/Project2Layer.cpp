#include "Project2Layer.h"
#include "Core/OrbitalControls.h"
#include "Core/Window.h"
#include "Core/Random.h"
#include "heli550.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>


#define BLADE_RADIUS	 1.0
#define BLADE_WIDTH		 0.4
#define PI 3.14159265359


Project2Layer* Project2Layer::s_Instance = nullptr;

static void UploadHelicopterVertices()
{
	int i;
	struct edge* ep;
	struct point* p0, * p1;

	for (i = 0, ep = Heliedges; i < Helinedges; i++, ep++)
	{
		p0 = &Helipoints[ep->p0];
		p1 = &Helipoints[ep->p1];
		glVertex3f(p0->x, p0->y, p0->z);
		glVertex3f(p1->x, p1->y, p1->z);
	}
}

Project2Layer::~Project2Layer()
{
}

void Project2Layer::OnAttach()
{
	s_Instance = this;
	m_HeliList = DrawList(
		GL_LINES,
		{ {0.0f, 0.0f, 0.0f} },
		{ {0.0f, 0.0f, 1.0f, -15.0f} },
		{ {1.0f, 1.0f, 1.0f} }, 
		UploadHelicopterVertices
	);

	CreateClouds();
	CreateBladeList();
	CreateSaucerList();
	InitializeMenus();

	Core::OrbitalControls::SetPosition(m_CameraStartPosition);
	Core::OrbitalControls::LockLookAt(Core::ProjectionType::Perspective, m_CameraStartPosition, { 0.0f, 0.0f, 0.0f });
}

void Project2Layer::OnDetach()
{

}

void Project2Layer::OnDisplay(float deltaTime)
{
	UpdateCamera(deltaTime);

	DrawClouds();
	DrawSaucer();
	DrawHelicopter();
	DrawBlades();


	glutSetWindow(Core::Window::GetMainWindow());
	glutPostRedisplay();
}

void Project2Layer::UpdateCamera(float deltaTime)
{
	if (m_LockView)
	{
		if (m_OutsideView)
		{
			if(m_OutsideCinemaFollow)
				Core::OrbitalControls::LockLookAt(Core::ProjectionType::Perspective, Core::OrbitalControls::GetPosition(), m_HelicopterPosition);
			else
				Core::OrbitalControls::LockLookAt(Core::ProjectionType::Perspective, glm::vec3(0.0f, 1.9f, 0.0f), m_HelicopterPosition);
		}
		else
		{
			if (m_ForwardCockpitView)
				Core::OrbitalControls::LockLookAt(Core::ProjectionType::Perspective, m_HeliCockpitViewpoint, m_HelicopterPosition + m_HeliCockpitLookat * 10.0f);
			else
				Core::OrbitalControls::LockLookAt(Core::ProjectionType::Perspective, m_HeliCenterViewpoint, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
	else
		Core::OrbitalControls::UpdateViewProjection(Core::ProjectionType::Perspective, deltaTime);
}

void Project2Layer::AnimateHelicopter(float dt)
{
	// Add a bit of delta to the position on each axis.
	float deltaX = cos(glutGet(GLUT_ELAPSED_TIME) * 0.0015f) * .7f;
	float deltaY = cos(glutGet(GLUT_ELAPSED_TIME) * 0.0015f) * .05f;
	float deltaZ = sin(glutGet(GLUT_ELAPSED_TIME) * 0.0015f) * .8f;

	// This frame's new position.
	glm::vec3 deltaPosition = m_HelicopterPosition + glm::vec3(deltaX, deltaY, deltaZ);

	// Calculate pitch/yaw using direction vector.
	glm::vec3 heading = glm::normalize(m_HelicopterPosition - deltaPosition);
	m_HeliPitch = glm::asin(heading.y);
	m_HeliYaw = std::atan2(heading.x, heading.z);

	glm::quat heliOrientation = glm::quat(glm::vec3(-m_HeliPitch, m_HeliYaw, 0.0f));
	// Model's relative forward is world Vector3::Right.
	glm::vec3 heliForward = glm::rotate(heliOrientation, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec3 heliRight = glm::rotate(heliOrientation, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 heliUp = glm::rotate(heliOrientation, glm::vec3(0.0f, 1.0f, 0.0f));

	// Get eulers -- this is used for glRotate3f.  Each component gets it's own glRotate3f.
	glm::vec3 eulers = glm::eulerAngles(heliOrientation);
	m_HelicopterRotation = eulers;

	// Moving the lookat just a tad to the right.
	m_HeliCockpitLookat = heliForward + heliRight * 0.1f;
	m_HelicopterPosition += heliForward * dt * 20.0f;
	m_HeliCenterViewpoint = m_HelicopterPosition + heliUp * 1.5f + heliForward * 2.0f;
	m_HeliCockpitViewpoint = m_HelicopterPosition + heliUp * 1.3f + heliForward * 5.3f + heliRight * 0.5f;
}

void Project2Layer::OnAnimate(float dt)
{
	const int MS_IN_THE_ANIMATION_CYCLE = 10000;	// milliseconds in the animation loop
	int ms = glutGet(GLUT_ELAPSED_TIME);			// milliseconds since the program started
	ms %= MS_IN_THE_ANIMATION_CYCLE;				// milliseconds in the range 0 to MS_IN_THE_ANIMATION_CYCLE-1
	m_AnimationPercent = (float)ms / (float)MS_IN_THE_ANIMATION_CYCLE;        // [ 0., 1. )
	
	m_RotaryOne.AnimationRotation = m_AnimationPercent * 360.0f * 20.0f;
	m_RotaryTwo.AnimationRotation = m_AnimationPercent * 360.0f * 20.0f;
	m_TailRotary.AnimationRotation = m_AnimationPercent * 360.0f * 40.0f;

	if (m_ShouldAnimateHelicopter)
		AnimateHelicopter(dt);

	// Ping Pong light intensity.
	float theta = glutGet(GLUT_ELAPSED_TIME) / 100.0f;
	float lightIntensity = glm::sin(theta);
	m_CurrentSaucerColor = glm::lerp(m_DimSaucerLightColor, m_BrightSaucerLightColor, lightIntensity * 0.5f + 0.5f);

	// Ping Pong saucer rotation.
	float spinPercent = glm::sin(theta / 10.0f);
	m_SaucerRotation = glm::lerp(0.0f, 360.0f, spinPercent * 0.5f + 0.5f);

	glutSetWindow(Core::Window::GetMainWindow());
	glutPostRedisplay();
}

void Project2Layer::OnEvent(Core::Event& event)
{
	Core::EventDispatcher dispatcher(event);

	dispatcher.Dispatch<Core::KeyPressedEvent>(BIND_EVENT_FN(Project2Layer::OnKeyPressed));
}


bool Project2Layer::OnKeyPressed(Core::KeyPressedEvent& keyPressedEvent)
{
	if (keyPressedEvent.GetKey() == Core::Key::F || keyPressedEvent.GetKey() == Core::Key::f)
	{
		m_OutsideView = !m_OutsideView;

		return true;
	}

	if (keyPressedEvent.GetKey() == Core::Key::v || keyPressedEvent.GetKey() == Core::Key::V)
	{
		m_OutsideCinemaFollow = !m_OutsideCinemaFollow;

		return true;
	}

	if (keyPressedEvent.GetKey() == Core::Key::C || keyPressedEvent.GetKey() == Core::Key::c)
	{
		m_ForwardCockpitView = !m_ForwardCockpitView;

		return true;
	}

	if (keyPressedEvent.GetKey() == Core::Key::U || keyPressedEvent.GetKey() == Core::Key::u)
	{
		m_LockView = !m_LockView;
		if (!m_LockView)
		{
			Core::OrbitalControls::SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
			Core::OrbitalControls::SetLookAt(m_HeliCenterViewpoint);
		}
		return true;
	}

	if (keyPressedEvent.GetKey() == Core::Key::Space)
	{
		glm::vec3 position = Core::OrbitalControls::GetPosition();
	}

	return false;
}


//------------------------- MENUS ---------------------------//

void Project2Layer::InitializeMenus()
{
	glutSetWindow(Core::Window::GetMainWindow());

	int cameraMenu = glutCreateMenu(OnCameraModeMenuChanged);
	glutAddMenuEntry("Toggle Free Camera", 0);
	glutAddMenuEntry("Saucer Cockpit View", 1);
	glutAddMenuEntry("Cinema Outside Follow View", 2);
	glutAddMenuEntry("Helicopter Carriage View", 3);
	glutAddMenuEntry("Helicopter Cockpit View", 4);

	int mainMenu = glutCreateMenu(OpenMainMenu);
	glutAddSubMenu("Camera Settings", cameraMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Project2Layer::OpenMainMenu(int id)
{
	glutSetWindow(Core::Window::GetMainWindow());
	glutPostRedisplay();
}


void Project2Layer::OnCameraModeMenuChanged(int mode)
{
	if (mode == 0)
		s_Instance->m_LockView = !s_Instance->m_LockView;
	if (mode == 1)
	{
		s_Instance->m_LockView = true;
		s_Instance->m_OutsideView = true;
		s_Instance->m_OutsideCinemaFollow = false;
	}
	if (mode == 2)
	{
		s_Instance->m_LockView = true;
		s_Instance->m_OutsideView = true;
		s_Instance->m_OutsideCinemaFollow = true;
	}
	if (mode == 3)
	{
		s_Instance->m_LockView = true;
		s_Instance->m_OutsideView = false;
		s_Instance->m_ForwardCockpitView = false;
	}
	if (mode == 4)
	{
		s_Instance->m_LockView = true;
		s_Instance->m_OutsideView = false;
		s_Instance->m_ForwardCockpitView = true;
	}

	glutSetWindow(Core::Window::GetMainWindow());
	glutPostRedisplay();
}

//------------------------- CREATE ---------------------------//

void Project2Layer::CreateBladeList()
{
	BladeList = glGenLists(1);

	glNewList(BladeList, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	glVertex2f(BLADE_RADIUS, BLADE_WIDTH / 2.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(BLADE_RADIUS, -BLADE_WIDTH / 2.0f);

	glVertex2f(-BLADE_RADIUS, -BLADE_WIDTH / 2.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(-BLADE_RADIUS, BLADE_WIDTH / 2.0f);
	glEnd();
	glEndList();
}

static void RestrictValueOutsideOfRadius(float& value, float max)
{
	// If the absolute value of the given value is within the range, scale it so that it's out of the range.
	if (glm::abs(value) < max)
	{
		float offset = max - glm::abs(value);
		value += glm::sign(value) * offset;
	}
}

void Project2Layer::CreateClouds()
{
	for (uint32_t i = 0; i < 30; i++)
	{
		glm::vec3 randomPositionInSphere = glm::vec3(Core::Random::RandomRange(-35, 35), Core::Random::RandomRange(-35, 35), Core::Random::RandomRange(-35, 35));

		// Prevent clouds from being drawn at the center of the scene.
		RestrictValueOutsideOfRadius(randomPositionInSphere.x, 12.0f);
		RestrictValueOutsideOfRadius(randomPositionInSphere.y, 12.0f);
		RestrictValueOutsideOfRadius(randomPositionInSphere.z, 12.0f);

		Cloud* cloud = new Cloud(randomPositionInSphere, Core::Random::RandomRange(3.0f, 5.0f), 20);
		m_Clouds.push_back(cloud);
	}
}

void Project2Layer::CreateSaucerList()
{
	Core::Geometry* saucer = Core::Geometry::Create(Core::PrimitiveType::Icosphere);

	TestSaucerList = glGenLists(1);

	glNewList(TestSaucerList, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for (auto index : saucer->GetIndices())
	{
		const Core::Vertex vertex = saucer->GetVertices()[index];
		glVertex3f(vertex.Position.x, vertex.Position.y, vertex.Position.z);
		glNormal3f(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);

	}
	glEnd();
	glEndList();

	delete saucer;
}

void Project2Layer::ApplyHelicopterTransformations()
{
	glTranslatef(m_HelicopterPosition.x, m_HelicopterPosition.y, m_HelicopterPosition.z);
	glRotatef(glm::degrees(m_HelicopterRotation.x), 1.0f, 0.0f, 0.0f);
	glRotatef(glm::degrees(m_HelicopterRotation.y), 0.0f, 1.0f, 0.0f);
	glRotatef(glm::degrees(m_HelicopterRotation.z), 0.0f, 0.0f, 1.0f);
}

//------------------------- DRAW ---------------------------//

void Project2Layer::DrawBlades()
{
	glPushMatrix();

	ApplyHelicopterTransformations();
	DrawHelicopterBlade(m_RotaryOne);
	DrawHelicopterBlade(m_RotaryTwo);
	DrawHelicopterBlade(m_TailRotary);

	glPopMatrix();
}

void Project2Layer::DrawHelicopterBlade(const HelicopterBlade& blade)
{
	glPushMatrix();
	for (Math::Vector3 translation : blade.Translations)
		glTranslatef(translation.x, translation.y, translation.z);
	for (Math::Vector4 rotation : blade.Rotations)
		glRotatef(rotation.w, rotation.x, rotation.y, rotation.z);
	glRotatef(blade.AnimationRotation, 0.0f, 0.0, 1.0f);
	for (Math::Vector3 scale : blade.Scales)
		glScalef(scale.x, scale.y, scale.z);

	glCallList(BladeList);
	glPopMatrix();
}

void Project2Layer::DrawHelicopter()
{
	glPushMatrix();
	ApplyHelicopterTransformations();

	glCallList(m_HeliList);
	glPopMatrix();
}

void Project2Layer::DrawSaucer()
{
	// Draw the base and rotate it.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScalef(5.0f, 1.0f, 5.0f);
	glColor3f(0.1f, 0.8f, 0.8f);
	glRotatef(m_SaucerRotation, 0.0f, 1.0f, 0.0f);
	glCallList(TestSaucerList);
	glPopMatrix();


	// Draw the cockpit.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
	glTranslatef(0.0f, 1.0f, 0.0f);
	glScalef(2.0f, 2.0f, 2.0f);
	glColor3f(0.8f, 0.8f, 0.1f);
	glCallList(TestSaucerList);
	glPopMatrix();


	// Draw the lights.
	// Position the lights in a circle.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (float theta = 0; theta < 2 * PI; theta += PI / 4)
	{
		// Hard-coded values make the lights look nice on the saucer base. 
		glm::vec3 position{ glm::cos(theta) * 3.5f, .8f, glm::sin(theta) * 3.5f };

		glPushMatrix();
		glColor3f(m_CurrentSaucerColor.r, m_CurrentSaucerColor.g, m_CurrentSaucerColor.b);
		glRotatef(m_SaucerRotation, 0.0f, 1.0f, 0.0f);
		glTranslatef(position.x, position.y, position.z);
		glScalef(0.4f, 0.4f, 0.4f);
		glCallList(TestSaucerList);
		glPopMatrix();
	}
}

void Project2Layer::DrawClouds()
{
	for (auto cloud : m_Clouds)
		cloud->Draw();
}
