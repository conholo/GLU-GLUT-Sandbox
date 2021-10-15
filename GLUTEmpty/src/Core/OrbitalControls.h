#pragma once

#include "Math/Vector.h"
#include "Core/KeyCode.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Core
{
	enum class ProjectionType { Perspective, Orthographic };

	class OrbitalControls
	{
	public:
		static void UpdateAspectRatio(int width, int height);
		static void UpdateViewProjection(ProjectionType projectionType, float deltaTime);
		static void UpdatePerspective(double fov, double aspect, double near, double far);
		static void UpdateOrthographic(double left, double right, double bottom, double top);
		static void LockLookAt(ProjectionType projectionType, const glm::vec3& position, const glm::vec3& lookAt);
		static void SetPosition(const glm::vec3& position) { s_Position = position; }
		static void SetLookAt(const glm::vec3& target);
		static void OnMouseWheel(int direction);
		static void Orbit(const glm::vec3& eyePosition, const glm::vec3& target, const glm::vec3& angleAxisDegrees);

		static glm::vec3 GetPosition() { return s_Position; }

	private:

		static glm::quat Orientation();
		static glm::vec3 Up();
		static glm::vec3 Right();
		static glm::vec3 Forward();

	private:

		// Projection Variables
		static ProjectionType s_ProjectionType;
		static float s_FOV;
		static float s_Aspect;
		static float s_NearClip;
		static float s_FarClip;

		static float s_Left;
		static float s_Right;
		static float s_Bottom;
		static float s_Top;

		static float s_MoveDamping;
		static float s_ZoomDamping;
		static float s_Angle;
		static float s_TestLookAtX;
		static float s_TestLookAtZ;
		static float s_RotationSpeed;

		static float s_PanSpeed;

		static glm::vec3 s_FocalPoint;
		static glm::vec3 s_Position;
		static glm::vec3 s_PositionDelta;
		static glm::vec2 s_CurrentMousePosition;

		static float s_Yaw;
		static float s_YawDelta;
		static float s_Pitch;
		static float s_PitchDelta;
		static float s_DistanceFromFocalPoint;
	};
}