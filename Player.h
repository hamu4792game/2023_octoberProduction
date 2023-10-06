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

	bool GetIsHold() { return isHold_; }

	bool GetIsRelease() { return isRelease_; }

	MoveVec GetMoveVec() { return moveVec_; }

	void SetPosition(Vector3 position) { worldTransform_.translation_ = position; }

private:

	const float kSpeed = 10.0f;

	MoveVec moveVec_ = Right;

	Vector3 velocity_{};

	std::unique_ptr<Model> model_;

	WorldTransform worldTransform_;

	//キーを押した瞬間
	bool isTap_ = false;

	//キーを長押ししている時
	bool isHold_ = false;

	//キーを離した瞬間
	bool isRelease_ = false;

public:

	//デバッグ用の一時変数
	bool isHit_ = false;
	bool isMiss_ = false;

	void IsHit() {
		isHit_ = true;
		isMiss_ = false;
	}

	void IsMiss() {
		isHit_ = false;
		isMiss_ = true;
	}

};

