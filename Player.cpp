#include "Player.h"

Player::Player() {
	model_ = std::make_unique<Model>();
}

void Player::Initialize() {

}

void Player::Update() {

	worldTransform_.UpdateMatrix();

}

void Player::Draw(const Matrix4x4& viewProjection) {

	model_->ModelDraw(worldTransform_, viewProjection, 0xffffffff, model_.get());

}

void Player::LoadModel() {

	model_->Texture("Resources/player/player.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "player/player.png");

}