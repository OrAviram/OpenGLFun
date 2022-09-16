#include "Matrix4x4.h"
#include "Color.h"
#include "QuadraticCurve.h"

//const Matrix4x4 Matrix4x4::Zero(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
//const Matrix4x4 Matrix4x4::Identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

constexpr Matrix4x4 Matrix4x4::Identity = Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
constexpr Matrix4x4 Matrix4x4::Zero = Matrix4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);

constexpr Color Color::Black = Color(0, 0, 0, 1);

constexpr Color Color::Red = Color(1, 0, 0, 1);
constexpr Color Color::Green = Color(0, 1, 0, 1);
constexpr Color Color::Blue = Color(0, 0, 1, 1);

constexpr Color Color::Magenta = Color(1, 0, 1, 1);
constexpr Color Color::Yellow = Color(1, 1, 0, 1);
constexpr Color Color::Cyan = Color(0, 1, 1, 1);

constexpr Color Color::White = Color(1, 1, 1, 1);
constexpr Color Color::DarkGray = Color(0.25f, 0.25f, 0.25f, 1);
constexpr Color Color::MiddleGray = Color(0.5f, 0.5f, 0.5f, 1);
constexpr Color Color::LightGray = Color(0.75f, 0.75f, 0.75f, 1);