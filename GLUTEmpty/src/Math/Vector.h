#pragma once
#include <cmath>

namespace Math
{
	struct Vector4
	{
		float x, y, z, w;

		Vector4()
			:x(0), y(0), z(0), w(w)
		{
		}
		Vector4(float x, float y, float z ,float w)
			:x(x), y(y), z(z), w(w)
		{
		}
		Vector4(const Vector4&) = default;

		static Vector4 Zero()
		{
			return { 0.0f, 0.0f, 0.0f, 0.0 };
		}

		static Vector4 Right()
		{
			return { 1.0, 0.0, 0.0, 0.0 };
		}

		static Vector4 Up()
		{
			return { 0.0, 1.0, 0.0, 0.0 };
		}

		// OpenGL Handedness
		static Vector4 Forward()
		{
			return { 0.0, 0.0, -1.0, 0.0 };
		}

		void operator +=(const Vector4 & other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
		}

		void operator -=(const Vector4 & other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		Vector4 operator + (const Vector4 & other)
		{
			return { x + other.x, y + other.y, z + other.z, w + other.w };
		}

		Vector4 operator - (const Vector4 & other)
		{
			return { x - other.x, y - other.y, z - other.z, w - other.w };
		}

		Vector4 operator* (float scalar)
		{
			return { x * scalar, y * scalar, z * scalar, w * scalar };
		}

		Vector4& operator*= (float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}

		Vector4 operator/ (float scalar)
		{
			if (scalar < 0) scalar = 1;

			return { x / scalar, y / scalar, z / scalar, w / scalar };
		}

		Vector4& operator/= (float scalar)
		{
			if (scalar < 0) scalar = 1;

			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
			return *this;
		}

		bool operator == (const Vector4 & other)
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

		bool operator != (const Vector4 & other)
		{
			return !(*this == other);
		}

		float Magnitude()
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}

		void Normalize()
		{
			*this /= Magnitude();
		}

		Vector4& Normalized()
		{
			return *this /= Magnitude();
		}
	};
	struct Vector3
	{
		float x, y, z;

		Vector3()
			:x(0), y(0), z(0)
		{
		}
		Vector3(float x, float y, float z)
			:x(x), y(y), z(z)
		{
		}
		Vector3(const Vector3&) = default;

		static Vector3 Zero()
		{
			return { 0.0f, 0.0f, 0.0f };
		}

		static Vector3 Right()
		{
			return { 1.0, 0.0, 0.0 };
		}

		static Vector3 Up()
		{
			return { 0.0, 1.0, 0.0 };
		}

		// OpenGL Handedness
		static Vector3 Forward()
		{
			return { 0.0, 0.0, -1.0 };
		}

		void operator +=(const Vector3 & other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
		}

		void operator -=(const Vector3 & other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		Vector3 operator + (const Vector3 & other)
		{
			return { x + other.x, y + other.y, z + other.z };
		}

		Vector3 operator - (const Vector3 & other)
		{
			return { x - other.x, y - other.y, z - other.z };
		}

		Vector3 operator* (float scalar)
		{
			return { x * scalar, y * scalar, z * scalar };
		}

		Vector3& operator*= (float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		Vector3 operator/ (float scalar)
		{
			if (scalar < 0) scalar = 1;

			return { x / scalar, y / scalar, z / scalar };
		}

		Vector3& operator/= (float scalar)
		{
			if (scalar < 0) scalar = 1;

			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		bool operator == (const Vector3 & other)
		{
			return x == other.x && y == other.y && z == other.z;
		}

		bool operator != (const Vector3 & other)
		{
			return !(*this == other);
		}

		Vector3& Cross(const Vector3 & lhs)
		{
			x = y * lhs.z - z * lhs.y;
			y = z * lhs.x - x * lhs.z;
			z = x * lhs.y - y * lhs.x;

			return *this;
		}

		float Dot(const Vector3 & other)
		{
			return x * other.x + y * other.y + z * other.z;
		}

		float Magnitude()
		{
			return sqrt(x * x + y * y + z * z);
		}

		void Normalize()
		{
			*this /= Magnitude();
		}

		Vector3& Normalized()
		{
			return *this /= Magnitude();
		}
	};
	struct Vector2
	{
		float x, y;

		Vector2()
			:x(0), y(0)
		{
		}
		Vector2(float x, float y)
			:x(x), y(y)
		{
		}
		Vector2(const Vector2&) = default;

		static Vector2 Zero()
		{
			return { 0.0f, 0.0f };
		}

		static Vector2 Right()
		{
			return { 1.0, 0.0 };
		}

		static Vector2 Up()
		{
			return { 0.0, 1.0 };
		}

		void operator +=(const Vector2& other)
		{
			x += other.x;
			y += other.y;
		}

		void operator -=(const Vector2& other)
		{
			x -= other.x;
			y -= other.y;
		}

		Vector2 operator + (const Vector2& other)
		{
			return { x + other.x, y + other.y };
		}

		Vector2 operator - (const Vector2& other)
		{
			return { x - other.x, y - other.y };
		}

		Vector2 operator* (float scalar)
		{
			return { x * scalar, y * scalar };
		}

		Vector2& operator*= (float scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		Vector2 operator/ (float scalar)
		{
			if (scalar < 0) scalar = 1;

			return { x / scalar, y / scalar };
		}

		Vector2& operator/= (float scalar)
		{
			if (scalar < 0) scalar = 1;

			x /= scalar;
			y /= scalar;
			return *this;
		}

		bool operator == (const Vector2& other)
		{
			return x == other.x && y == other.y;
		}

		bool operator != (const Vector2& other)
		{
			return !(*this == other);
		}

		float Dot(const Vector2& other)
		{
			return x * other.x + y * other.y;
		}

		float Magnitude()
		{
			return sqrt(x * x + y * y );
		}

		void Normalize()
		{
			*this /= Magnitude();
		}

		Vector2& Normalized()
		{
			return *this /= Magnitude();
		}
	};

	static Vector2 operator+ (const Vector2& a, const Vector2& b)
	{
		return { a.x + b.x, a.y + b.y };
	}
	static Vector2 operator- (const Vector2& a, const Vector2& b)
	{
		return { a.x - b.x, a.y - b.y };
	}
	static Vector3 operator+ (const Vector3& a, const Vector3& b)
	{
		return { a.x + b.x, a.y + b.y, a.z + b.z };
	}
	static Vector3 operator- (const Vector3& a, const Vector3& b)
	{
		return { a.x - b.x, a.y - b.y, a.z - b.z };
	}
	static Vector4 operator+ (const Vector4& a, const Vector4& b)
	{
		return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
	}
	static Vector4 operator- (const Vector4& a, const Vector4& b)
	{
		return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
	}
}