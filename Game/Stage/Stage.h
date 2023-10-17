#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Texture/Model.h"

class Stage
{
public:
	Stage();
	~Stage() = default;

	void Update();

	void Draw3D(const Matrix4x4& viewProjectionMat);

	void ModelLoad();

private: // モデルデータ
	std::vector<std::shared_ptr<Model>> buildings;

	std::vector<WorldTransform> transforms_;

};
