#include "Core/OrbitalControls.h"
#include "Core/Input.h"

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "Core/Window.h"

#include <glm/gtx/compatibility.hpp>
#include <iostream>


namespace Core
{
	ProjectionType OrbitalControls::s_ProjectionType = ProjectionType::Perspective;

	float OrbitalControls::s_FOV = 45.0f;
	float OrbitalControls::s_Aspect = 1.0f;
	float OrbitalControls::s_NearClip = 0.1f;
	float OrbitalControls::s_FarClip = 1000.0f;

	float OrbitalControls::s_Left = -1.0f;
	float OrbitalControls::s_Right = 1.0f;
	float OrbitalControls::s_Bottom = -1.0f;
	float OrbitalControls::s_Top = 1.0f;


	float OrbitalControls::s_MoveDamping = 1.0f;
	float OrbitalControls::s_ZoomDamping = 0.1f;
	float OrbitalControls::s_Angle = 0.0f;
	float OrbitalControls::s_TestLookAtX = 0.0f;
	float OrbitalControls::s_TestLookAtZ = 0.0f;
	float OrbitalControls::s_RotationSpeed = 180.0f;

	float OrbitalControls::s_PanSpeed = 3.0f;
	glm::vec3 OrbitalControls::s_FocalPoint{ 0.0f, 0.0f, 0.0f };
	glm::vec3 OrbitalControls::s_Position{ 0.0f, 0.0f, 5.0f };
	glm::vec3 OrbitalControls::s_PositionDelta;
	glm::vec2 OrbitalControls::s_CurrentMousePosition{ 0.0f, 0.0f };
	float OrbitalControls::s_Yaw = 0.0f;
	float OrbitalControls::s_YawDelta = 0.0f;
	float OrbitalControls::s_Pitch = 0.0f;
	float OrbitalControls::s_PitchDelta = 0.0f;
	float OrbitalControls::s_DistanceFromFocalPoint = 10.0f;

#define PI 3.14159265359

	void OrbitalControls::UpdatePerspective(double fov, double aspect, double nearClip, double farClip)
	{
		s_FOV = fov;
		s_Aspect = aspect;
		s_NearClip = nearClip;
		s_FarClip = farClip;
	}

	void OrbitalControls::UpdateOrthographic(double left, double right, double bottom, double top)
	{
		s_Left = left;
		s_Right = right;
		s_Top = top;
		s_Bottom = bottom;
	}

	void OrbitalControls::LockLookAt(ProjectionType projectionType, const glm::vec3& position, const glm::vec3& lookAt)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (projectionType == ProjectionType::Perspective)
			gluPerspective(s_FOV, s_Aspect, s_NearClip, s_FarClip);
		else
			gluOrtho2D(s_Left, s_Right, s_Bottom, s_Top);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		gluLookAt(position.x, position.y, position.z, lookAt.x, lookAt.y, lookAt.z, 0.0f, 1.0f, 0.0f);

		glEnable(GL_NORMALIZE);
	}

	void OrbitalControls::UpdateAspectRatio(int width, int height)
	{
		s_Aspect = (float)width / (float)height;
	}

	void OrbitalControls::UpdateViewProjection(ProjectionType projectionType, float deltaTime)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (projectionType == ProjectionType::Perspective)
			gluPerspective(s_FOV, s_Aspect, s_NearClip, s_FarClip);
		else
			gluOrtho2D(s_Left, s_Right, s_Bottom, s_Top);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		const glm::vec2 mousePosition = Input::GetMousePosition();
		const glm::vec2 mousePositionDelta = (mousePosition - s_CurrentMousePosition) * 0.002f;

		float yawSign = Up().y < 0.0f ? -1.0f : 1.0f;

		if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
		{
			if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::w))
				s_PositionDelta += Forward() * s_PanSpeed * deltaTime;
			if (Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::s))
				s_PositionDelta -= Forward() * s_PanSpeed * deltaTime;

			s_PitchDelta = mousePositionDelta.y * s_RotationSpeed * deltaTime;
			s_YawDelta = mousePositionDelta.x * yawSign * s_RotationSpeed * deltaTime;
		}
		else
		{
			if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::w))
				s_PositionDelta += Up() * s_PanSpeed * deltaTime;
			if (Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::s))
				s_PositionDelta -= Up() * s_PanSpeed * deltaTime;
		}

		if (Input::IsKeyPressed(Key::D) || Input::IsKeyPressed(Key::d))
			s_PositionDelta += Right() * s_PanSpeed * deltaTime;
		if (Input::IsKeyPressed(Key::A) || Input::IsKeyPressed(Key::a))
			s_PositionDelta -= Right() * s_PanSpeed * deltaTime;

		s_Position += s_PositionDelta;
		s_Yaw += s_YawDelta;
		s_Pitch += s_PitchDelta;

		s_FocalPoint = s_Position + Forward() * s_DistanceFromFocalPoint;
		s_DistanceFromFocalPoint = glm::distance(s_Position, s_FocalPoint);
		glm::vec3 lookAt = s_Position + Forward();
		s_YawDelta *= 0.6f;
		s_PitchDelta *= 0.6f;
		s_PositionDelta *= 0.8f;

		gluLookAt(s_Position.x, s_Position.y, s_Position.z, lookAt.x, lookAt.y, lookAt.z, 0.0f, yawSign, 0.0f);

		glEnable(GL_NORMALIZE);

		s_CurrentMousePosition = mousePosition;
	}

	glm::quat OrbitalControls::Orientation()
	{
		return glm::quat(glm::vec3(-s_Pitch, -s_Yaw, 0.0f));
	}

	glm::vec3 OrbitalControls::Up()
	{
		return glm::rotate(Orientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 OrbitalControls::Right()
	{
		return glm::rotate(Orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 OrbitalControls::Forward()
	{
		return glm::rotate(Orientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	void OrbitalControls::SetLookAt(const glm::vec3& target)
	{
		glm::vec3 directionToTarget = glm::normalize(s_Position - target);
		s_Pitch = glm::asin(directionToTarget.y);
		s_Yaw = std::atan2(directionToTarget.x, directionToTarget.z);

		s_PitchDelta = 0;
		s_YawDelta = 0;	
	}

	void OrbitalControls::OnMouseWheel(int direction)
	{
		s_DistanceFromFocalPoint -= direction;
		s_Position = s_FocalPoint - Forward() * s_DistanceFromFocalPoint;

		if (s_DistanceFromFocalPoint > 1.0f)
		{
			s_FocalPoint += Forward();
			s_DistanceFromFocalPoint = 1.0f;
		}

		s_PositionDelta += (float)direction * Forward() * s_ZoomDamping;

		glutSetWindow(Window::GetMainWindow());
		glutPostRedisplay();
	}

	void OrbitalControls::Orbit(const glm::vec3& eyePosition, const glm::vec3& target, const glm::vec3& angleAxisDegrees)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(s_FOV, s_Aspect, s_NearClip, s_FarClip);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glm::quat quaternion = glm::quat(glm::radians(angleAxisDegrees));

		glm::vec3 worldPosition = quaternion * (eyePosition - target) + target;

		float yawSign = angleAxisDegrees.y < 0.0 ? -1.0f : 1.0f;

		gluLookAt(worldPosition.x, worldPosition.y, worldPosition.z, target.x, target.y, target.z, 0.0f, yawSign, 0.0f);
	}
}