#include "Player.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "externals/imgui/imgui.h"

Player::Player() {
	model_ = std::make_unique<Model>();
	arrowModel_ = std::make_unique<Model>();
	SE1_ = std::make_unique<AudioInput>();
	SE1_->SoundLoadWave("./Resources/loopBGM/tempo.wav");
}

void Player::Initialize(Vector3 pos) {

	isTap_ = false;
	velocity_ = { 0.0f,0.0f,0.0f };
	SetPosition(pos);
	SetInsidePosition({ 0.0f,0.0f,0.0f });
	prePosition_ = worldTransform_.translation_;
	worldTransformArrow_.parent_ = &worldTransform_;
	worldTransformArrow_.translation_.z = 3.0f;

}

void Player::Update(std::vector<Vector3> ControlPoints, int lastLinePass) {

#ifdef _DEBUG

	ImGui::Begin("player flag");
	ImGui::Checkbox("StartMove", &isMove);
	ImGui::Text("isTrigger : %d", isTap_);
	ImGui::Text("isHold : %d", isHold_);
	ImGui::Text("isRelease : %d", isRelease_);
	ImGui::Text("isHit : %d", isHit_);
	ImGui::Text("isMiss : %d", isMiss_);
	ImGui::End();

#endif // _DEBUG

	if (KeyInput::PushKey(DIK_Q)) {
		
		if (isMove) {
			isMove = false;
		}
		else {
			isMove = true;
		}

	}

	//キーを押した瞬間の判定
	if (KeyInput::PushKey(DIK_SPACE)) {
		isTap_ = true;
		SE1_->SoundStop();
		SE1_->SoundPlayWave();
		SE1_->SetVolume(0.2f);
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

	if (isMove) {
		point += 0.1f;
	}
	if (point > static_cast<float>(divisionNumber) - 0.1f) {
		point = 0.0f;
		linePass += 1;
		if (linePass > lastLinePass) {
			linePass = firstLinePass;
		}
	}

	t = point / static_cast<float>(divisionNumber);



	if (linePass == 0) {
		Vector3 p = makeCatmullRom(ControlPoints[0], ControlPoints[0], ControlPoints[1], ControlPoints[2], t);
		worldTransform_.translation_ = p;
	}
	if (linePass >= 1 && linePass != lastLinePass) {
		Vector3 p = makeCatmullRom(ControlPoints[linePass - 1], ControlPoints[linePass], ControlPoints[linePass + 1], ControlPoints[linePass + 2], t);
		worldTransform_.translation_ = p;
	}
	if (linePass == lastLinePass) {
		Vector3 p = makeCatmullRom(ControlPoints[lastLinePass - 1], ControlPoints[lastLinePass], ControlPoints[lastLinePass + 1], ControlPoints[lastLinePass + 1], t);
		worldTransform_.translation_ = p;
	}

	SetRotate(worldTransform_.translation_ - prePosition_);

	worldTransform_.UpdateMatrix();
	worldTransformArrow_.UpdateMatrix();
	worldTransformInside_.UpdateMatrix();

	prePosition_ = worldTransform_.translation_;

}

void Player::Draw(const Matrix4x4& viewProjection) {

	model_->ModelDraw(worldTransform_, viewProjection, 0xffffffff, model_.get());
	arrowModel_->ModelDraw(worldTransformArrow_, viewProjection, 0xffffffff, arrowModel_.get());
	model_->ModelDraw(worldTransformInside_, viewProjection, 0xffffffff, model_.get());

}

void Player::ModelLoad() {

	model_->Texture("Resources/player/player.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/player/player.png");
	arrowModel_->Texture("Resources/arrow/arrow.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/player/player.png");
}

void Player::SetRotate(Vector3 velocity) {

	//正規化
	Vector3 tmpVel = Normalize(velocity);

	
	worldTransform_.rotation_.y = float(std::atan2(double(tmpVel.x), double(tmpVel.z)));

	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);

	tmpVel = TransformNormal(tmpVel, rotateYMatrix);

	worldTransform_.rotation_.x = float(std::atan2(double(-tmpVel.y), double(tmpVel.z)));

	/*worldTransform_.rotation_.z = float(std::atan2(double(velocity.y), double(velocity.x)));*/

}
