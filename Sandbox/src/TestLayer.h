#pragma once

#include "Core/Layer.h"
#include "Math/Vector.h"
#include "Core/Event.h"
#include <vector>

struct Vertex
{
	Math::Vector3 Position;
	Math::Vector3 Color;
};

struct Triangle
{
	Triangle() = default;
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c)
		:A(a), B(b), C(c) { }

	Vertex A;
	Vertex B;
	Vertex C;
};


struct Pyramid
{
	Pyramid()
	{
		Vertex tip{ {0.0f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f} };

		Math::Vector3 frontColor{ 1.0f, 0.0f, 0.0f };
		Vertex frontA{ {-0.5f, -0.5f, 0.5f}, frontColor };
		Vertex frontC{ { 0.5f, -0.5f, 0.5f }, frontColor };
		Triangle front( frontA, tip, frontC);
		m_Triangles.push_back(front);

		Math::Vector3 rightColor{ 0.0f, 1.0f, 0.0f };
		Vertex rightA{ {0.5f, -0.5f,  0.5f}, rightColor };
		Vertex rightC{ {0.5f, -0.5f, -0.5f}, rightColor };
		Triangle right(rightA, tip, rightC);
		m_Triangles.push_back(right);

		Math::Vector3 leftColor{ 0.0f, 0.0f, 1.0f };
		Vertex leftA{ {-0.5f, -0.5f,  0.5f}, leftColor };
		Vertex leftC{ {-0.5f, -0.5f, -0.5f}, leftColor };
		Triangle left(leftA, tip, leftC);
		m_Triangles.push_back(left);

		Math::Vector3 backColor{ 1.0f, 1.0f, 0.0f };
		Vertex backA{ {0.5f,  -0.5f, -0.5f}, backColor };
		Vertex backC{ {-0.5f, -0.5f, -0.5f}, backColor };
		Triangle back(backA, tip, backC);
		m_Triangles.push_back(back);

		Math::Vector3 bottomColor{ 1.0f, 0.0f, 1.0f };

		Vertex bottomQuadA{ {-0.5f, -0.5f, 0.5f}, bottomColor };
		Vertex bottomQuadB{ {-0.5f, -0.5f,-0.5f}, bottomColor };
		Vertex bottomQuadC{ { 0.5f, -0.5f, 0.5f}, bottomColor };

		Triangle bottomA{ bottomQuadA, bottomQuadB, bottomQuadC };

		Vertex bottomQuadD{ {-0.5f, -0.5f, -0.5f}, bottomColor };
		Vertex bottomQuadE{ {0.5f, -0.5f,  -0.5f}, bottomColor };
		Vertex bottomQuadF{ {0.5f, -0.5f,   0.5f}, bottomColor };

		Triangle bottomB{ bottomQuadD, bottomQuadE, bottomQuadF };

		m_Triangles.push_back(bottomA);
		m_Triangles.push_back(bottomB);
	}

	const std::vector<Triangle>& GetTriangles() const { return m_Triangles; }
	void SetScale(const Math::Vector3& scale) { m_Scale = scale; }
	void SetPosition(const Math::Vector3& position) { m_Position = position; }
	void SetAngleAxes(const Math::Vector3& axes) { m_RotationAxes = axes; }
	void SetAngle(float angle) { m_RotationAngle = angle; }

private:
	Math::Vector3 m_Position;
	float m_RotationAngle = 0.0f;
	Math::Vector3 m_RotationAxes;
	Math::Vector3 m_Scale;

	std::vector<Triangle> m_Triangles;
};

class TestLayer : public Core::Layer
{
public:
	TestLayer();
	~TestLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnDisplay(float dt) override;
	void OnAnimate(float dt) override;
	void OnEvent(Core::Event& event) override;

private:
	Pyramid* m_Pyramid = nullptr;
};