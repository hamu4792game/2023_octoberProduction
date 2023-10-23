#include "Billboard.h"
#include <numbers>

Matrix4x4 Billboard::Update(const WorldTransform& transform, const Matrix4x4& viewProjection) {
	Matrix4x4 backToFrontMatrix = MakeRotateZMatrix(std::numbers::pi_v<float>);

	Matrix4x4 billboardMatrix = backToFrontMatrix * viewProjection;
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	Matrix4x4 worldTrans = transform.worldMatrix * billboardMatrix;
	return worldTrans;
}

