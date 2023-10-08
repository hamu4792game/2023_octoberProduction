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

	void Initialize(Vector3 pos);

	void Update(std::vector<Vector3> ControlPoints, int lastLinePass);

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

	//線上移動用変数群
	const uint32_t divisionNumber = 8;
	float point = 0.0f;
	float t = 0.0f;

	int linePass = 0;

	const int firstLinePass = 0;
	int LastLinePass = 2;

	bool isMove = false;

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

