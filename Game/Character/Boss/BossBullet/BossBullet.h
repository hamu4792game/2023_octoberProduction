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

	const WorldTransform* parent_;
	WorldTransform transform_;
	Model* model_;
	Vector3 start_;
	float nowFrame_;


public:
	void SetTransform(const WorldTransform* trans);

};
