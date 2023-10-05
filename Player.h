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
		Up,
		Down,
		Left,
		Right
	};

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void ModelLoad();

	Vector3 GetPosition() { return worldTransform_.translation_; }

	bool GetIsTap() { return isTap_; }

	MoveVec GetMoveVec() { return moveVec_; }

private:

	const float kSpeed = 10.0f;

	MoveVec moveVec_ = Right;

	Vector3 velocity_{};

	std::unique_ptr<Model> model_;

	WorldTransform worldTransform_;

	bool isTap_ = false;

};

