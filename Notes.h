#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include <memory>
#include "math/Matrix4x4.h"

class Notes
{
public:

	Notes();
	~Notes() = default;

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void LoadModel();

	void SetPosition(const Vector3& position) { 
		worldTransform_.translation_ = position;
		worldTransform_.UpdateMatrix();
	}

private:

	std::unique_ptr<Model> model_;

	WorldTransform worldTransform_;

	bool isHit_ = false;

	bool isTouch_ = false;

	bool isMiss_ = false;

};

