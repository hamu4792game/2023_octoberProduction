#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"

class Billboard
{
public:
	Billboard() = default;
	~Billboard() = default;

	Matrix4x4 Update(const WorldTransform& transform, const Matrix4x4& viewProjection);

private:

};

