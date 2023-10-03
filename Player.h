#pragma once

#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "math/Matrix4x4.h"
#include <memory>

class Player
{
public:

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void LoadModel();

private:

	std::unique_ptr<Model> model_;

	WorldTransform worldTransform_;



};

