#pragma once

#include "Core/Layer.h"
#include "Math/Vector.h"
#include "Core/Event.h"

#include <functional>
#include <glm/glm.hpp>
#include <glut.h>

#include "Core/Geometry/Geometry.h"
#include "Cloud.h"

struct HelicopterBlade
{
	HelicopterBlade(
		std::initializer_list<Math::Vector3> translations,
		std::initializer_list<Math::Vector4> rotations,
		std::initializer_list<Math::Vector3> scales
		)
		:Translations(translations), Rotations(rotations), Scales(scales) { }
	HelicopterBlade(const HelicopterBlade&) = default;


	std::vector<Math::Vector3> Translations;
	std::vector<Math::Vector4> Rotations;
	std::vector<Math::Vector3> Scales;
	float AnimationRotation = 0.0f;
};

class DrawList
{
	using VertexUploadFn = void(*)();
public:
	DrawList() = default;

	DrawList(int drawType, 
		std::initializer_list<Math::Vector3> translations, 
		std::initializer_list<Math::Vector4> rotations, 
		std::initializer_list<Math::Vector3> scales, 
		const VertexUploadFn& fn)
	{
		m_ID = glGenLists(1);
		glNewList(m_ID, GL_COMPILE);
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		for(Math::Vector3 translation : translations)
			glTranslatef(translation.x, translation.y, translation.z);
		for (Math::Vector4 rotation : rotations)
			glRotatef(rotation.w, rotation.x, rotation.y, rotation.z);
		for (Math::Vector3 scale : scales)
			glScalef(scale.x, scale.y, scale.z);

		glBegin(drawType);
			fn();
		glEnd();
		glPopMatrix();
		glEndList();
	}

	operator int()
	{
		return m_ID;
	}

private:
	int m_ID = -1;
};


struct SphereGizmo
{
	SphereGizmo(const glm::vec3& initialPosition, const glm::vec3& scale, const glm::vec3& color)
		:Position(initialPosition)
	{
		Sphere = Core::Geometry::Create(Core::PrimitiveType::Icosphere);

		DrawListID = glGenLists(1);
		glNewList(DrawListID, GL_COMPILE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glPushMatrix();
		glTranslatef(initialPosition.x, initialPosition.y, initialPosition.z);
		glScalef(scale.x, scale.y, scale.z);
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_TRIANGLES);
		for (auto index : Sphere->GetIndices())
		{
			Core::Vertex vertex = Sphere->GetVertices()[index];
			glVertex3f(vertex.Position.x, vertex.Position.y, vertex.Position.z);
		}
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPopMatrix();

		glEndList();
	}

	~SphereGizmo()
	{
		delete Sphere;
		Sphere = nullptr;
	}

	void MoveTo(const glm::vec3& position) { Position = position; }
	
	int DrawListID = -1;
	Core::Geometry* Sphere = nullptr;
	glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
};

class Project2Layer : public Core::Layer
{
public:
	~Project2Layer();

	void OnAttach() override;
	void OnDetach() override;
	void OnDisplay(float deltaTime) override;
	void OnAnimate(float dt) override;
	void OnEvent(Core::Event& event) override;

private:

	void UpdateCamera(float deltaTime);

	void AnimateHelicopter(float dt);
	void DrawHelicopterBlade(const HelicopterBlade& blade);
	void CreateBladeList();
	void CreateSaucerList();
	void CreateClouds();

	void ApplyHelicopterTransformations();
	void DrawBlades();
	void DrawHelicopter();
	void DrawSaucer();
	void DrawClouds();

	bool OnKeyPressed(Core::KeyPressedEvent& keyPressedEvent);

private:

	void InitializeMenus();
	static void OnCameraModeMenuChanged(int mode);
	static void OpenMainMenu(int mode);

private:
	static Project2Layer* s_Instance;
	
	float m_AnimationPercent = 0.0f;

	std::vector<Cloud*> m_Clouds;

	glm::vec3 m_CameraStartPosition{ 10.0f, 12.0f, 25.0f };
	glm::vec3 m_HeliCenterViewpoint{0.0f, 0.0f, 0.0f};
	glm::vec3 m_HeliCockpitViewpoint{0.0f, 0.0f, 0.0f};
	glm::vec3 m_HelicopterRotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_HelicopterPosition{ 13.0f, 0.0f, 0.0f };
	glm::vec3 m_HeliCockpitLookat{ 0.0f };

	bool m_OutsideView = false;
	bool m_LockView = false;
	bool m_ForwardCockpitView = false;
	bool m_OutsideCinemaFollow = false;
	bool m_ShouldAnimateHelicopter = true;
	float m_HeliPitch = 0.0f;
	float m_HeliYaw = 0.0f;
	DrawList m_HeliList;
	HelicopterBlade m_RotaryOne{
		{ {1.6f, 3.7f, .3f} },
		{ {1.0f, 0.0f, 0.0f, 90.0f} },
		{ {8.0f, 1.0f, 1.0f} }
	};
	HelicopterBlade m_RotaryTwo{
		{ {1.6f, 3.7f, .3f} },
		{ {1.0f, 0.0f, 0.0f, 90.0f}, {0.0f, 0.0f, 1.0f, 90.0f} },
		{ {8.0f, 1.0f, 1.0f} }
	};
	HelicopterBlade m_TailRotary{
		{ {-7.5f, 2.3f, -1.0f} }, 
		{ {0.0f, 0.0f, 1.0f, 95.0f} }, 
		{ {1.0f, .5f, .5f} }
	};


	float m_SaucerRotation = 0.0f;
	glm::vec3 m_DimSaucerLightColor{ 0.2f, 0.0f, 0.0f };
	glm::vec3 m_BrightSaucerLightColor{ 1.0f, 0.0f, 0.0f };
	glm::vec3 m_CurrentSaucerColor{ 0.5f, 0.0f, 0.0f };
	float m_LightFlashInterval = 0.5f;

	int TestSaucerList = -1;
	int BladeList = -1;
	int SaucerBaseList = -1;
	int SaucerCockpitList = -1;
	int SaucerLightList = -1;
};