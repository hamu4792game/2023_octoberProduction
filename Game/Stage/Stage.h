#pragma once
#include "math/Matrix4x4.h"

class Stage
{
public:
	Stage();
	~Stage() = default;

	void Update();

	void Draw3D(const Matrix4x4& viewProjectionMat);

	void ModelLoad();

private:

};
