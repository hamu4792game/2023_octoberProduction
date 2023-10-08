#include "Player.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "externals/imgui/imgui.h"

Player::Player() {
	model_ = std::make_unique<Model>();
}

void Player::Initialize() {

	isTap_ = false;
	velocity_ = { 0.0f,0.0f,0.0f };
	SetPosition({ -40.0f,0.0f,0.0f });

}

void Player::Update() {

#ifdef _DEBUG

	ImGui::Begin("player flag");
	ImGui::Text("isTrigger : %d", isTap_);
	ImGui::Text("isHold : %d", isHold_);
	ImGui::Text("isRelease : %d", isRelease_);
	ImGui::Text("isHit : %d", isHit_);
	ImGui::Text("isMiss : %d", isMiss_);
	ImGui::End();

#endif // _DEBUG


	//キーを押した瞬間の判定
	if (KeyInput::PushKey(DIK_SPACE)) {
		isTap_ = true;
	}
	else {
		isTap_ = false;
	}

	//キーを押している最中の判定
	if (KeyInput::GetKey(DIK_SPACE)) {
		isHold_ = true;
	}
	else {
		isHold_ = false;
	}

	//キーを離した瞬間の判定
	if (KeyInput::ReleaseKey(DIK_SPACE)) {
		isRelease_ = true;
	}
	else {
		isRelease_ = false;
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

	/*float deltaTime = 1.0f / 60.0f;

	velocity_ *= deltaTime;

	worldTransform_.translation_ += velocity_;

	if (worldTransform_.translation_.x >= 40.0f) {
		worldTransform_.translation_.x = -40.0f;
	}*/


	worldTransform_.UpdateMatrix();

}

void Player::Draw(const Matrix4x4& viewProjection) {

	model_->ModelDraw(worldTransform_, viewProjection, 0xffffffff, model_.get());

}

void Player::ModelLoad() {

	model_->Texture("Resources/player/player.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "player/player.png");

}
