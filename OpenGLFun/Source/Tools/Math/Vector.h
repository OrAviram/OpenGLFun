#pragma once
#include <math.h>

template<typename T>
struct Vector2
{
	union
	{
		struct { T x, y; };
		T elements[2];
	};

	constexpr Vector2() : x(0), y(0) { }
	constexpr Vector2(T x, T y) : x(x), y(y) { }
	constexpr Vector2(const Vector2& other) : x(other.x), y(other.y) { }

	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 One;
	static const Vector2 Zero;

	inline Vector2 Add(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
	inline Vector2 operator+(const Vector2& other) const { return Add(other); }

	inline Vector2 Subtract(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
	inline Vector2 operator-(const Vector2& other) const { return Subtract(other); }

	inline Vector2 Multiply(T scalar) const { return Vector2(scalar * x, scalar * y); }
	inline Vector2 operator*(T scalar) const { return Multiply(scalar); }
	
	inline Vector2 Divide(T scalar) const { return Vector2(x / scalar, y / scalar); }
	inline Vector2 operator/(T scalar) const { return Divide(scalar); }

	inline Vector2 Negate() const { return Vector2(-x, -y); }
	inline Vector2 operator-() const { return Negate(); }

	inline T Dot(const Vector2& other) const { return x * other.x + y * other.y; }
	inline T operator,(const Vector2& other) const { return Dot(other); }

	inline T SqrLength() { return Dot(*this); }
	inline T Length() { return sqrt(SqrLength()); }

	inline T operator[](int index) const { return elements[index]; }
	inline T& operator[](int index) { return elements[index]; }

	inline Vector2& operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	inline Vector2& operator+=(const Vector2& other) { return *this = *this + other; }
	inline Vector2& operator-=(const Vector2& other) { return *this = *this - other; }
	inline Vector2& operator*=(T scalar) { return *this = *this * scalar; }
	inline Vector2& operator/=(T scalar) { return *this = *this / scalar; }
};

template<typename T>
struct Vector3
{
	union
	{
		struct { T x, y, z; };
		T elements[3];
	};

	constexpr Vector3() : x(0), y(0), z(0) { }
	constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) { }
	constexpr Vector3(const Vector2<T>& xy, T z) : x(xy.x), y(xy.y), z(z) { }
	constexpr Vector3(T x, const Vector2<T>& yz) : x(x), y(yz.y), z(yz.z) { }
	constexpr Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) { }

	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 One;
	static const Vector3 Zero;

	inline Vector3 Add(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
	inline Vector3 operator+(const Vector3& other) const { return Add(other); }

	inline Vector3 Subtract(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
	inline Vector3 operator-(const Vector3& other) const { return Subtract(other); }

	inline Vector3 Multiply(T scalar) const { return Vector3(scalar * x, scalar * y, scalar * z); }
	inline Vector3 operator*(T scalar) const { return Multiply(scalar); }

	inline Vector3 Divide(T scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }
	inline Vector3 operator/(T scalar) const { return Divide(scalar); }

	inline Vector3 Negate() const { return Vector3(-x, -y, -z); }
	inline Vector3 operator-() const { return Negate(); }

	inline T Dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
	inline T operator,(const Vector3& other) const { return Dot(other); }

	inline T SqrLength() const { return Dot(*this); }
	inline T Length() const { return sqrt(SqrLength()); }

	inline Vector3 Cross(const Vector3& other) const { return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }

	inline T operator[](int index) const { return elements[index]; }
	inline T& operator[](int index) { return elements[index]; }

	inline Vector3& operator=(const Vector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	inline Vector3& operator+=(const Vector3& other) { return *this = *this + other; }
	inline Vector3& operator-=(const Vector3& other) { return *this = *this - other; }
	inline Vector3& operator*=(T scalar) { return *this = *this * scalar; }
	inline Vector3& operator/=(T scalar) { return *this = *this / scalar; }
};

template <typename T>
struct Vector4
{
	union
	{
		struct { T x, y, z, w; };
		T elements[4];
	};

	constexpr Vector4() : x(0), y(0), z(0), w(0) { }
	constexpr Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }
	constexpr Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }

	static const Vector4 UnitX;
	static const Vector4 UnitY;
	static const Vector4 UnitZ;
	static const Vector4 UnitW;
	static const Vector4 One;
	static const Vector4 Zero;

	inline Vector4 Add(const Vector4& other) const { return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
	inline Vector4 operator+(const Vector4& other) const { return Add(other); }

	inline Vector4 Subtract(const Vector4& other) const { return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
	inline Vector4 operator-(const Vector4& other) const { return Subtract(other); }

	inline Vector4 Multiply(T scalar) const { return Vector4(scalar * x, scalar * y, scalar * z, scalar * w); }
	inline Vector4 operator*(T scalar) const { return Multiply(scalar); }

	inline Vector4 Divide(T scalar) const { return Vector4(x / scalar, y / scalar, z / scalar, w / scalar); }
	inline Vector4 operator/(T scalar) const { return Divide(scalar); }

	inline Vector4 Negate() const { return Vector4(-x, -y, -z, -w); }
	inline Vector4 operator-() const { return Negate(); }

	inline T Dot(const Vector4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }
	inline T operator,(const Vector4& other) const { return Dot(other); }

	inline T SqrLength() { return Dot(*this); }
	inline T Length() { return sqrt(SqrLength()); }

	inline T operator[](int index) const { return elements[index]; }
	inline T& operator[](int index) { return elements[index]; }

	inline Vector4& operator=(const Vector4& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	inline Vector4& operator+=(const Vector4& other) { return *this = *this + other; }
	inline Vector4& operator-=(const Vector4& other) { return *this = *this - other; }
	inline Vector4& operator*=(T scalar) { return *this = *this * scalar; }
	inline Vector4& operator/=(T scalar) { return *this = *this / scalar; }
};

template<typename T>
inline Vector2<T> operator*(T scalar, const Vector2<T>& vector) { return vector * scalar; }

template<typename T>
inline Vector3<T> operator*(T scalar, const Vector3<T>& vector) { return vector * scalar; }

template<typename T>
inline Vector4<T> operator*(T scalar, const Vector4<T>& vector) { return vector * scalar; }

typedef Vector4<float> Vector4f;
typedef Vector3<float> Vector3f;
typedef Vector2<float> Vector2f;

typedef Vector4<int> Vector4i;
typedef Vector3<int> Vector3i;
typedef Vector2<int> Vector2i;

template<typename T> const Vector2<T> Vector2<T>::UnitX(1, 0);
template<typename T> const Vector2<T> Vector2<T>::UnitY(0, 1);
template<typename T> const Vector2<T> Vector2<T>::One(1, 1);
template<typename T> const Vector2<T> Vector2<T>::Zero(0, 0);

template<typename T> const Vector3<T> Vector3<T>::UnitX(1, 0, 0);
template<typename T> const Vector3<T> Vector3<T>::UnitY(0, 1, 0);
template<typename T> const Vector3<T> Vector3<T>::UnitZ(0, 0, 1);
template<typename T> const Vector3<T> Vector3<T>::One(1, 1, 1);
template<typename T> const Vector3<T> Vector3<T>::Zero(0, 0, 0);

template<typename T> const Vector4<T> Vector4<T>::UnitX(1, 0, 0, 0);
template<typename T> const Vector4<T> Vector4<T>::UnitY(0, 1, 0, 0);
template<typename T> const Vector4<T> Vector4<T>::UnitZ(0, 0, 1, 0);
template<typename T> const Vector4<T> Vector4<T>::UnitW(0, 0, 0, 1);
template<typename T> const Vector4<T> Vector4<T>::One(1, 1, 1, 1);
template<typename T> const Vector4<T> Vector4<T>::Zero(0, 0, 0, 0);