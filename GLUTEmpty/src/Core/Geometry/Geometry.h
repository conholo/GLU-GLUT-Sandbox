#pragma once

#include <stdint.h>
#include <vector>
#include "Core/Geometry/Vertex.h"


namespace Core
{
	enum class PrimitiveType { None = 0, Cube, Quad, Triangle, Icosphere, Sphere, Plane };

	class Geometry
	{
	public:
		static Geometry* Create(PrimitiveType primitiveType);
		static Geometry* Icosphere(float radius = 1.0f, uint32_t sectorCount = 36, uint32_t stackCount = 18);
		static Geometry* Sphere(float radius);
		static Geometry* Cube();
		static Geometry* Quad();
		static Geometry* Plane();
		static Geometry* Triangle();

		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		const std::vector<uint32_t>& GetIndices() const { return m_Indices; }

		std::vector<Vertex>& GetVertices() { return m_Vertices; }

	private:
		Geometry(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}

