#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "math/Matrix4x4.h"
#include "Engine/Texture/Model.h"

class BossBullet
{
public:
	BossBullet(Model* model);
	~BossBullet() = default;

	void Update();

	void Draw3D(const Matrix4x4& viewProjection);

private:

	WorldTransform transform_;
	Model* model_;


public:
	void SetPosition(const Vector3& pos) { transform_.translation_ = pos; }

};
