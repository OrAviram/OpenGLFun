#pragma once
#include "Vector.h"
#include "Angle.h"
#include <math.h>

struct Matrix4x4
{
	union
	{
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};

		struct
		{
			Vector4f row0;
			Vector4f row1;
			Vector4f row2;
			Vector4f row3;
		};

		float elements1D[16];
		float elements[4][4];
		Vector4f rows[4];
	};

	static const Matrix4x4 Identity;
	static const Matrix4x4 Zero;

	constexpr Matrix4x4() :
		m00(0), m01(0), m02(0), m03(0),
		m10(0), m11(0), m12(0), m13(0),
		m20(0), m21(0), m22(0), m23(0),
		m30(0), m31(0), m32(0), m33(0) { }

	constexpr Matrix4x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) :
		m00(m00), m01(m01), m02(m02), m03(m03),
		m10(m10), m11(m11), m12(m12), m13(m13),
		m20(m20), m21(m21), m22(m22), m23(m23),
		m30(m30), m31(m31), m32(m32), m33(m33) { }

	constexpr Matrix4x4(float elements[4][4]) :
		m00(elements[0][0]), m01(elements[0][1]), m02(elements[0][2]), m03(elements[0][3]),
		m10(elements[1][0]), m11(elements[1][1]), m12(elements[1][2]), m13(elements[1][3]),
		m20(elements[2][0]), m21(elements[2][1]), m22(elements[2][2]), m23(elements[2][3]),
		m30(elements[3][0]), m31(elements[3][1]), m32(elements[3][2]), m33(elements[3][3]) { }

	constexpr Matrix4x4(const Matrix4x4& other) :
		m00(other.m00), m01(other.m01), m02(other.m02), m03(other.m03),
		m10(other.m10), m11(other.m11), m12(other.m12), m13(other.m13),
		m20(other.m20), m21(other.m21), m22(other.m22), m23(other.m23),
		m30(other.m30), m31(other.m31), m32(other.m32), m33(other.m33) { }

	constexpr Matrix4x4(const Vector4f& row0, const Vector4f& row1, const Vector4f& row2, const Vector4f& row3) :
		row0(row0), row1(row1), row2(row2), row3(row3) { }

	inline Matrix4x4 Add(const Matrix4x4& other) const { return Matrix4x4(row0 + other.row0, row1 + other.row1, row2 + other.row2, row3 + other.row3); }
	inline Matrix4x4 operator+(const Matrix4x4& other) const { return Add(other); }

	inline Matrix4x4 Subtract(const Matrix4x4& other) const { return Matrix4x4(row0 - other.row0, row1 - other.row1, row2 - other.row2, row3 - other.row3); }
	inline Matrix4x4 operator-(const Matrix4x4& other) const { return Subtract(other); }

	inline Matrix4x4 Negate() const { return Matrix4x4(-row0, -row1, -row2, -row3); }
	inline Matrix4x4 operator-() const { return Negate(); }

	inline Matrix4x4 Multiply(float scalar) const { return Matrix4x4(scalar * row0, scalar * row1, scalar * row2, scalar * row3); }
	inline Matrix4x4 operator*(float scalar) const { return Multiply(scalar); }
	
	inline Matrix4x4 Divide(float scalar) const { return Matrix4x4(row0 / scalar, row1 / scalar, row2 / scalar, row3 / scalar); }
	inline Matrix4x4 operator/(float scalar) const { return Divide(scalar); }

	inline Vector4f Multiply(const Vector4f& vector) const { return Vector4f((vector, row0), (vector, row1), (vector, row2), (vector, row3)); }
	inline Vector4f operator*(const Vector4f& vector) const { return Multiply(vector); }

	inline Matrix4x4 Multiply(const Matrix4x4& other) const
	{
		float m[4][4];
		for (int row = 0; row < 4; row++)
		{
			for (int column = 0; column < 4; column++)
			{
				m[row][column] = 0;
				for (int i = 0; i < 4; i++)
					m[row][column] += elements[row][i] * other.elements[i][column];
			}
		}
		return m;
	}
	inline Matrix4x4 operator*(const Matrix4x4& other) const { return Multiply(other); }

	inline float operator()(int row, int column) const { return elements[row][column]; }
	inline float& operator()(int row, int column) { return elements[row][column]; }

	inline float operator[](int index) const { return elements1D[index]; }
	inline float& operator[](int index) { return elements1D[index]; }

	inline static Matrix4x4 CreateScale(float width, float height, float depth)
	{
		return Matrix4x4(
			width, 0, 0, 0,
			0, height, 0, 0,
			0, 0, depth, 0,
			0, 0, 0, 1);
	}
	inline static Matrix4x4 CreateScale(const Vector3f& scale) { return CreateScale(scale.x, scale.y, scale.z); }
	inline static Matrix4x4 CreateScale(float uniformScale) { return CreateScale(uniformScale, uniformScale, uniformScale); }

	inline static Matrix4x4 CreateScale(float width, float height, float depth, float originX, float originY, float originZ)
	{
		return Matrix4x4(
			width, 0, 0, (1 - width) * originX,
			0, height, 0, (1 - height) * originY,
			0, 0, depth, (1 - depth) * originZ,
			0, 0, 0, 1);
	}
	inline static Matrix4x4 CreateScale(const Vector3f& scale, float originX, float originY, float originZ) { return CreateScale(scale.x, scale.y, scale.z, originX, originY, originZ); }
	inline static Matrix4x4 CreateScale(const Vector3f& scale, const Vector3f& origin) { return CreateScale(scale.x, scale.y, scale.z, origin.x, origin.y, origin.z); }
	inline static Matrix4x4 CreateScale(float width, float height, float depth, const Vector3f& origin) { return CreateScale(width, height, depth, origin.x, origin.y, origin.z); }
	inline static Matrix4x4 CreateScale(float uniformScale, const Vector3f& origin) { return CreateScale(uniformScale, uniformScale, uniformScale, origin.x, origin.y, origin.z); }

	inline static Matrix4x4 CreateTranslation(float dx, float dy, float dz)
	{
		return Matrix4x4(
			1, 0, 0, dx,
			0, 1, 0, dy,
			0, 0, 1, dz,
			0, 0, 0, 1);
	}
	inline static Matrix4x4 CreateTranslation(const Vector3f& dr) { return CreateTranslation(dr.x, dr.y, dr.z); }

	inline static Matrix4x4 CreateRotationZ(Angle angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		return Matrix4x4(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}
	inline static Matrix4x4 CreateRotationZ(Angle angle, float originX, float originY)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		return Matrix4x4(
			c, -s, 0, (1 - c) * originX + originY * s,
			s, c, 0, -s * originX + (1 - c) * originY,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}
	inline static Matrix4x4 CreateRotationZ(Angle angle, const Vector2f& origin) { return CreateRotationZ(angle, origin.x, origin.y); }

	inline static Matrix4x4 CreateRotationX(Angle angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		return Matrix4x4(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1);
	}

	inline static Matrix4x4 CreateRotationY(Angle angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		return Matrix4x4(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1);
	}

	inline static Matrix4x4 CreateOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
	{
		// Right handed system ot left handed system
		float dx = right - left;
		float dy = top - bottom;
		float dz = far - near;
		return Matrix4x4(
			2 / dx, 0, 0, -(left + right) / dx,
			0, 2 / dy, 0, -(top + bottom) / dy,
			0, 0, -2 / dz, (far + near) / dz,
			0, 0, 0, 1);
	}

	inline static Matrix4x4 CreateOrthographicProjection(float width, float height, float near, float far)
	{
		// Right handed system ot left handed system
		float right = width / 2;
		float top = height / 2;
		return CreateOrthographicProjection(-right, right, -top, top, near, far);
	}

	inline static Matrix4x4 CreatePerspectiveProjection(Angle fieldOfView, float aspectRatio, float near, float far)
	{
		// Right handed system to left handed system.
		float t = 1 / tanf(fieldOfView * 0.5f);
		float fmn = far - near;
		return Matrix4x4(
			t, 0, 0, 0,
			0, aspectRatio * t, 0, 0,
			0, 0, -(far + near) / fmn, 2 * near * far / fmn,
			0, 0, 1, 0); // Interesting
	}

	inline static Matrix4x4 CreateView(const Vector3f& origin, const Vector3f& forward, const Vector3f& up)
	{
		// To right handed system.
		Vector3f right = forward.Cross(up);
		Vector3f upn = right.Cross(forward);

		float rs = right.SqrLength();
		float us = upn.SqrLength();
		float fs = forward.SqrLength();
		return Matrix4x4(
			right.x / rs, right.y / rs, right.z / rs, -origin.Dot(right) / rs,
			upn.x / us, upn.y / us, upn.z / us, -origin.Dot(upn) / us,
			forward.x / fs, forward.y / fs, forward.z / fs, -origin.Dot(forward) / fs,
			0, 0, 0, 1);
	}

	inline Matrix4x4& operator=(const Matrix4x4& other)
	{
		for (int i = 0; i < 16; i++)
			elements1D[i] = other.elements1D[i];
		
		return *this;
	}

	inline Matrix4x4& operator+=(const Matrix4x4& other) { return *this = *this + other; }
	inline Matrix4x4& operator-=(const Matrix4x4& other) { return *this = *this - other; }
	inline Matrix4x4& operator*=(const Matrix4x4& other) { return *this = *this * other; }
	inline Matrix4x4& operator*=(float scalar) { return *this = *this * scalar; }
	inline Matrix4x4& operator/=(float scalar) { return *this = *this / scalar; }

	inline Matrix4x4 Transpose() const
	{
		return Matrix4x4(
			m00, m10, m20, m30,
			m01, m11, m21, m31,
			m02, m12, m22, m32,
			m03, m13, m23, m33);
	}
};

inline Matrix4x4 operator*(float scalar, const Matrix4x4& matrix) { return matrix * scalar; }