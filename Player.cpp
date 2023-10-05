#include "Player.h"
#include "Engine/Input/KeyInput/KeyInput.h"

Player::Player() {
	model_ = std::make_unique<Model>();
}

void Player::Initialize() {

	isTap_ = false;
	velocity_ = { 0.0f,0.0f,0.0f };

}

void Player::Update() {

	if (isTap_) {
		isTap_ = false;
	}

	if (KeyInput::PushKey(DIK_SPACE)) {
		isTap_ = true;
	}

	switch (moveVec_)
	{
	case Up:
		velocity_ = { 0.0f,kSpeed,0.0f };
		break;
	case Down:
		velocity_ = { 0.0f,-kSpeed,0.0f };
		break;
	case Left:
		velocity_ = { -kSpeed,0.0f,0.0f };
		break;
	case Right:
		velocity_ = { kSpeed,0.0f,0.0f };
		break;
	default:
		velocity_ = { 0.0f,0.0f,0.0f };
		break;
	}

	float deltaTime = 1.0f / 60.0f;

	velocity_ *= deltaTime;

	worldTransform_.translation_ += velocity_;

	if (worldTransform_.translation_.x >= 40.0f) {
		worldTransform_.translation_.x = 40.0f;
		moveVec_ = Left;
	}
	else if (worldTransform_.translation_.x <= -40.0f) {
		worldTransform_.translation_.x = -40.0f;
		moveVec_ = Right;
	}

	worldTransform_.UpdateMatrix();

}

void Player::Draw(const Matrix4x4& viewProjection) {

	model_->ModelDraw(worldTransform_, viewProjection, 0xffffffff, model_.get());

}

void Player::ModelLoad() {

	model_->Texture("Resources/player/player.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/player.png");

}
