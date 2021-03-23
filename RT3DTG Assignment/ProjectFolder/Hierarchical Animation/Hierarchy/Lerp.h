#pragma once

#include "Application.h"
//#include <DirectXMath.h>
#include <cassert>


XMFLOAT3 sub(XMFLOAT3& a, XMFLOAT3& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

XMFLOAT3 mul(XMFLOAT3& a, float b)
{
	return { a.x * b, a.y * b, a.z * b };
}

XMFLOAT3 add(XMFLOAT3& a, XMFLOAT3& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

// a + (t * ( b-a ) );
XMFLOAT3 lerp3f(XMFLOAT3& a, XMFLOAT3& b, float t)
{
	assert(t <= 1 && t >= 0);
	//return add(mul(sub(b, a), t), a);
	XMFLOAT3 subf3 = sub(b, a); //(b-a)
	XMFLOAT3 mulf3 = mul(subf3, t); //(t * sub)
	XMFLOAT3 addf3 = add(mulf3, a); // (a + mul)
	return addf3;
}

std::pair<XMFLOAT3, XMFLOAT3> lerpPair(std::pair<XMFLOAT3, XMFLOAT3>& a, std::pair<XMFLOAT3, XMFLOAT3>& b, float t)
{
	std::pair<XMFLOAT3, XMFLOAT3> c;
	c.first = lerp3f(a.first, b.first, t);
	c.second = lerp3f(a.second, b.second, t);
	return c;
}