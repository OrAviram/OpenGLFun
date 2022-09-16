#include <Tests/Test.h>
#include <Tools/Graphics/Curve.h>
#include <Tools/Graphics/Shader.h>
#include <Tools/Graphics/Point.h>

struct CubicCurve2D
{
	Vector2f coefficient0;
	Vector2f coefficient1;
	Vector2f coefficient2;
	Vector2f coefficient3;

	CubicCurve2D()
	{
	}

	CubicCurve2D(const Vector2f& coefficient0, const Vector2f& coefficient1, const Vector2f& coefficient2, const Vector2f& coefficient3)
	{
		this->coefficient0 = coefficient0;
		this->coefficient1 = coefficient1;
		this->coefficient2 = coefficient2;
		this->coefficient3 = coefficient3;
	}

	Vector2f GetPoint(float t) const
	{
		float tSqr = t * t;
		return coefficient0 + t * coefficient1 + tSqr * coefficient2 + t * tSqr * coefficient3;
	}

	Vector2f GetPoint0() const { return coefficient0; }
	Vector2f GetTangent0() const { return coefficient1; }

	Vector2f GetPoint1() const { return coefficient0 + coefficient1 + coefficient2 + coefficient3; }
	Vector2f GetTangent1() const { return coefficient1 + 2.f * coefficient2 + 3.f * coefficient3; }

	Vector2f GetBezierControl1() { return coefficient0; }
	Vector2f GetBezierControl2() { return coefficient0 + coefficient1 / 3; }
	Vector2f GetBezierControl3() { return coefficient0 + coefficient1 + coefficient2 + coefficient3 - (coefficient1 + 2.f * coefficient2 + 3.f * coefficient3)/ 3; }
	Vector2f GetBezierControl4() { return coefficient0 + coefficient1 + coefficient2 + coefficient3; }

	inline static CubicCurve2D FromTangents(const Vector2f& point0, const Vector2f& tangent0, const Vector2f& point1, const Vector2f& tangent1)
	{
		return CubicCurve2D(
			point0,
			tangent0,
			-3.f * point0 - 2.f * tangent0 + 3.f * point1 - 1.f * tangent1,
			2.f * point0 + 1.f * tangent0 - 2.f * point1 + 1.f * tangent1);
	}

	inline static CubicCurve2D FromBezier(const Vector2f& control0, const Vector2f& control1, const Vector2f& control2, const Vector2f& control3)
	{
		return FromTangents(
			control0,
			3.f * (control1 - control0),
			control3,
			3.f * (control3 - control2));
	}
};


class CubicCurveTest : public Test
{
	Curve2D* curve;
	Shader* shader;

	Curve2D* t1;
	Curve2D* t2;

	int pointCount;
	CubicCurve2D curveData;
	Color curveColor = Color::Red;
	Color tangentColor = Color::Blue;

public:
	CubicCurveTest();
	virtual ~CubicCurveTest();

	virtual void Render(TimeSpan dt) override;
	virtual void RenderGUI(TimeSpan dt) override;

private:
	void Regenerate();
};