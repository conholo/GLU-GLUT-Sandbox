#include "Core/Geometry/Geometry.h"

#include <iostream>

namespace Core
{
#define PI 3.14159265359

	Geometry::Geometry(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
		:m_Vertices(vertices), m_Indices(indices) { }

	Geometry* Geometry::Create(PrimitiveType primitiveType)
	{
		switch (primitiveType)
		{
			case PrimitiveType::Sphere: return Sphere(1.0f);
			case PrimitiveType::Icosphere: return Icosphere();
			case PrimitiveType::Cube:	return Cube();
			case PrimitiveType::Plane:	return Plane();
			case PrimitiveType::Quad:	return Quad();
		}

		return nullptr;
	}

	Geometry* Geometry::Quad()
	{
		// LAYOUT: Position (3F) - TexCoords (2F) - Normals (3F)
		std::vector<Vertex> vertices =
		{
			Vertex{ {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
			Vertex{ { 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
			Vertex{ { 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
			Vertex{ {-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
		};

		std::vector<uint32_t> indices =
		{
			 0, 1, 2, 2, 3, 0
		};

		return new Geometry(vertices, indices);
	}


	Geometry* Geometry::Plane()
	{
		// LAYOUT: Position (3F) - TexCoords (2F) - Normals (3F)
		std::vector<Vertex> vertices =
		{
			Vertex{ {-0.5f,	 0.0f,  0.5f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
			Vertex{ { 0.5f,  0.0f,  0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
			Vertex{ { 0.5f,  0.0f, -0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
			Vertex{ {-0.5f,  0.0f, -0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
		};

		std::vector<uint32_t> indices =
		{
			 0, 1, 2, 2, 3, 0
		};

		return new Geometry(vertices, indices);
	}

	Geometry* Geometry::Triangle()
	{
		std::vector<Vertex> vertices =
		{
			Vertex{ {-0.5f,	-0.5f,  0.0f},	{0.0f, 0.0f},	{0.0f, 0.0f, 1.0f} },
			Vertex{ { 0.5f, -0.5f,  0.0f},	{1.0f, 0.0f},	{0.0f, 0.0f, 1.0f} },
			Vertex{ { 0.0f,  0.5f,  0.0f},	{0.5f, 1.0f},	{0.0f, 0.0f, 1.0f} },
		};

		std::vector<uint32_t> indices =
		{
			0, 1, 2
		};

		return new Geometry(vertices, indices);
	}


	Geometry* Geometry::Icosphere(float radius, uint32_t sectorCount, uint32_t stackCount)
	{
		if (radius <= 0)
			radius = 0.1f;

		std::vector<Vertex> vertices;

		glm::vec3 vertexPosition(0.0f);
		glm::vec3 normal(0.0f);
		glm::vec2 texCoords(0.0f);

		float xy;
		float lengthInverse = 1 / radius;

		float sectorStep = 2 * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		for (uint32_t i = 0; i <= stackCount; i++)
		{
			stackAngle = PI / 2 - i * stackStep;
			xy = radius * glm::cos(stackAngle);
			vertexPosition.z = radius * glm::sin(stackAngle);

			for (uint32_t j = 0; j <= sectorCount; j++)
			{
				sectorAngle = j * sectorStep;

				vertexPosition.x = xy * glm::cos(sectorAngle);
				vertexPosition.y = xy * glm::sin(sectorAngle);

				normal.x = vertexPosition.x * lengthInverse;
				normal.y = vertexPosition.y * lengthInverse;
				normal.z = vertexPosition.z * lengthInverse;

				texCoords.x = (float)j / sectorCount;
				texCoords.y = (float)i / stackCount;

				vertices.push_back({ vertexPosition, texCoords, normal });
			}
		}

		std::vector<uint32_t> indices;

		int k1, k2;

		for (uint32_t i = 0; i < stackCount; i++)
		{
			k1 = i * (sectorCount + 1);
			k2 = k1 + sectorCount + 1;

			for (uint32_t j = 0; j < sectorCount; j++, k1++, k2++)
			{
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				if (i != stackCount - 1)
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		return new Geometry(vertices, indices);
	}

	Geometry* Geometry::Sphere(float radius)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		constexpr float latitudeBands = 30;
		constexpr float longitudeBands = 30;

		for (float latitude = 0.0f; latitude <= latitudeBands; latitude++)
		{
			const float theta = latitude * (float)PI / latitudeBands;
			const float sinTheta = glm::sin(theta);
			const float cosTheta = glm::cos(theta);

			float texT = 1.0f - theta / PI;

			for (float longitude = 0.0f; longitude <= longitudeBands; longitude++)
			{
				const float phi = longitude * 2.0f * (float)PI / longitudeBands;
				const float sinPhi = glm::sin(phi);
				const float cosPhi = glm::cos(phi);

				float texS = 1.0f - (phi / (2 * PI));

				Vertex vertex;
				vertex.Normal = { cosPhi * sinTheta, cosTheta, sinPhi * sinTheta };
				vertex.TexCoord = { texS, texT };
				vertex.Position = { radius * vertex.Normal.x, radius * vertex.Normal.y, radius * vertex.Normal.z };
				vertices.push_back(vertex);
			}
		}

		for (uint32_t latitude = 0; latitude < (uint32_t)latitudeBands; latitude++)
		{
			for (uint32_t longitude = 0; longitude < (uint32_t)longitudeBands; longitude++)
			{
				const uint32_t first = (latitude * ((uint32_t)longitudeBands + 1)) + longitude;
				const uint32_t second = first + (uint32_t)longitudeBands + 1;

				indices.push_back(first); 
				indices.push_back(second);
				indices.push_back(first + 1);

				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}

		return new Geometry(vertices, indices);
	}

	Geometry* Geometry::Cube()
	{
		std::vector<Vertex> vertices =
		{
			// Front Face
			Vertex{ {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, { 0.0f,   0.0f,  1.0f } },	// 0 0 
			Vertex{ { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, { 0.0f,   0.0f,  1.0f } },	// 1 1
			Vertex{ { 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, { 0.0f,   0.0f,  1.0f } },	// 2 2
			Vertex{ {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, { 0.0f,   0.0f,  1.0f } },	// 3 3

			// Right Face									
			Vertex{ { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, { 1.0f,   0.0f,  0.0f } },	// 1 4 
			Vertex{ { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, { 1.0f,   0.0f,  0.0f } },	// 5 5
			Vertex{ { 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, { 1.0f,   0.0f,  0.0f } },	// 6 6
			Vertex{ { 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, { 1.0f,   0.0f,  0.0f } },	// 2 7

			// Back Face									
			Vertex{ { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, { 0.0f,   0.0f, -1.0f } },	// 4 8
			Vertex{ {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, { 0.0f,   0.0f, -1.0f } },	// 5 9
			Vertex{ {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, { 0.0f,   0.0f, -1.0f } },	// 6 10
			Vertex{ { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, { 0.0f,   0.0f, -1.0f } },	// 7 11

			// Left Face									
			Vertex{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, { -1.0f,  0.0f,  0.0f } },	// 0 12
			Vertex{ {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, { -1.0f,  0.0f,  0.0f } },	// 4 13
			Vertex{ {-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, { -1.0f,  0.0f,  0.0f } },	// 7 14
			Vertex{ {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, { -1.0f,  0.0f,  0.0f } },	// 3 15

			// Bottom Face									
			Vertex{ { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {  0.0f, -1.0f,  0.0f } },	// 0 16
			Vertex{ {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {  0.0f, -1.0f,  0.0f } },	// 1 17
			Vertex{ {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {  0.0f, -1.0f,  0.0f } },	// 5 18
			Vertex{ { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {  0.0f, -1.0f,  0.0f } },	// 4 19

			// Top Face										
			Vertex{ {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, {  0.0f,  1.0f,  0.0f } },	// 3 20
			Vertex{ { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {  0.0f,  1.0f,  0.0f } },	// 2 21
			Vertex{ { 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {  0.0f,  1.0f,  0.0f } },	// 6 22
			Vertex{ {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {  0.0f,  1.0f,  0.0f } }	// 7 23
		};

		std::vector<uint32_t> indices =
		{
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			4, 5, 6,
			6, 7, 4,
			// back
			8, 9, 10,
			10, 11, 8,
			// left
			12, 13, 14,
			14, 15, 12,
			// bottom
			16, 17, 18,
			18, 19, 16,
			// top
			20, 21, 22,
			22, 23, 20
		};

		return new Geometry(vertices, indices);
	}

}

