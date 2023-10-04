#pragma once

#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "math/Matrix4x4.h"
#include <memory>

class Player
{
public:

	Player();
	~Player() = default;

	enum MoveVec
	{
		Forward,
		Back
	};

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void LoadModel();

	Vector3 GetPosition() { return worldTransform_.translation_; }

	bool GetIsTouch() { return isTouch_; }

private:

	MoveVec moveVec_ = Forward;

	Vector3 velocity_{};

	std::unique_ptr<Model> model_;

	WorldTransform worldTransform_;

	bool isTouch_ = false;

};

