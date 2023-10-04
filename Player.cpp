#include "Player.h"
#include "Engine/Input/KeyInput/KeyInput.h"

Player::Player() {
	model_ = std::make_unique<Model>();
}

void Player::Initialize() {

	isTouch_ = false;
	velocity_ = { 0.0f,0.0f,0.0f };

}

void Player::Update() {

	if (isTouch_) {
		isTouch_ = false;
	}

	if (KeyInput::PushKey(DIK_SPACE)) {
		isTouch_ = true;
	}

	switch (moveVec_)
	{
	case Forward:
		velocity_ = { 0.0f,0.0f,1.0f };
		break;
	case Back:
		velocity_ = { 0.0f,0.0f,-1.0f };
		break;
	default:
		velocity_ = { 0.0f,0.0f,0.0f };
		break;
	}

	worldTransform_.translation_ += velocity_;

	if (worldTransform_.translation_.z >= 100.0f) {
		worldTransform_.translation_.z = 100.0f;
		moveVec_ = Back;
	}
	else if (worldTransform_.translation_.z <= 0.0f) {
		worldTransform_.translation_.z = 0.0f;
		moveVec_ = Forward;
	}

	worldTransform_.UpdateMatrix();

}

void Player::Draw(const Matrix4x4& viewProjection) {

	model_->ModelDraw(worldTransform_, viewProjection, 0xffffffff, model_.get());

}

void Player::LoadModel() {

	model_->Texture("Resources/player/player.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/player.png");

}
