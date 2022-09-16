#pragma once

struct QuadraticPolynomial
{
	float a0 = 0, a1 = 0, a2 = 0;

	QuadraticPolynomial() {}
	QuadraticPolynomial(float a0, float a1, float a2) : a0(a0), a1(a1), a2(a2) { }

	float Evaluate(float t) { return a0 + t * (a1 + t * a2); }
	float GetDerivative(float t) { return a1 + 2 * t * a2; }
};

template<typename TVec>
struct QuadraticCurve
{
	TVec p0, p1, p2;

	QuadraticCurve() {}
	QuadraticCurve(const TVec& p0, const TVec& p1, const TVec& p2) : p0(p0), p1(p1), p2(p2) { }

	TVec GetPoint(const QuadraticPolynomial& basis0, const QuadraticPolynomial& basis1, const QuadraticPolynomial& basis2, float t)
	{
		return p0 * basis0.Evaluate(t) + p1 * basis1.Evaluate(t) + p2 * basis2.Evaluate(t);
	}

	TVec GetPointStandardBasis(float t) { return p0 + (p1 + p2 * t) * t; }
};

typedef QuadraticCurve<Vector2f> QuadraticCurve2D;