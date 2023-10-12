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

	void Initialize(Vector3 pos);

	void Update(std::vector<Vector3> ControlPoints, int lastLinePass);

	void Draw(const Matrix4x4& viewProjection);

	void ModelLoad();

	Vector3 GetPosition() { return worldTransform_.translation_; }

	bool GetIsTap() { return isTap_; }

	bool GetIsHold() { return isHold_; }

	bool GetIsRelease() { return isRelease_; }

	void SetPosition(Vector3 position) { worldTransform_.translation_ = position; }

	WorldTransform* GetWorldTransformPtr() { return &worldTransform_; }

	//移動方向から向きを計算する
	void SetRotate(Vector3 velocity);

private:

	const float kSpeed = 10.0f;

	Vector3 velocity_{};

	//内部判定の位置
	Vector3 insidePos_{};

	//一フレーム前の位置
	Vector3 prePosition_{};

	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> arrowModel_;

	WorldTransform worldTransform_;
	WorldTransform worldTransformArrow_;

	//キーを押した瞬間
	bool isTap_ = false;

	//キーを長押ししている時
	bool isHold_ = false;

	//キーを離した瞬間
	bool isRelease_ = false;

	//線上移動用変数群
	const uint32_t divisionNumber = 2;
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

