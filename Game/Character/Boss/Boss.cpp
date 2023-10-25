#include "Boss.h"
#include "Game/PartsEnum.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"

Boss::Boss() {
	back_ = std::make_unique<Model>();
	back_->Texture("Resources/floor/floor.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/hud/test.png");
}

void Boss::Initialize() {
	partsTransform_.resize(parts_.size());
	partsTransform_[static_cast<uint8_t>(BossParts::Body)].parent_ = &transform_;
	for (int8_t i = static_cast<uint8_t>(BossParts::Body) + 1; i < static_cast<uint8_t>(BossParts::kMaxCount); i++) {
		partsTransform_[i].parent_ = &partsTransform_[static_cast<uint8_t>(BossParts::Body)];
	}

	// 親子関係
	partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)];

	partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::Waist)];
	partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::Waist)];

	partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)];
	partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)];

	transform_.translation_ = Vector3(2.0f, 2.0f, 35.0f);
	partsTransform_[static_cast<uint8_t>(BossParts::Head)].translation_.y = 2.0f;

	backTrans_.parent_ = &transform_;
	backTrans_.translation_.z = 3.0f;
	backTrans_.scale_ = Vector3(5.0f, 1.0f, 5.0f);
	backTrans_.rotation_.x = AngleToRadian(-90.0f);
	back_->blendType = BlendMode::Subtract;

	bullets_.clear();

}

void Boss::Update() {

	ImGui::Begin("Boss");
	ImGui::DragFloat3("trans", &transform_.translation_.x);
	ImGui::DragFloat3("scale", &transform_.scale_.x);
	ImGui::DragFloat3("backtrans", &backTrans_.translation_.x);
	ImGui::DragFloat3("backscale", &backTrans_.scale_.x);
	ImGui::DragFloat3("backrotate", &backTrans_.rotation_.x);
	ImGui::End();

	/*if (KeyInput::PushKey(DIK_SPACE)) {
		attackFlag_ = true;
	}

	Attack();*/

	if (backTrans_.rotation_.z >= AngleToRadian(90.0f)) {
		backTrans_.rotation_.z = AngleToRadian(0.0f);
	}
	else {
		backTrans_.rotation_.z += AngleToRadian(1.0f);
	}
	

	//	弾1つ1つの更新
	for (auto& i : bullets_) {
		i->Update();
	}

	//	playerが攻撃するフラグが立ったら
	bullets_.remove_if([](std::shared_ptr<BossBullet> bullet) {

		if (bullet->GetIsDead()) {
			return true;
		}

		return false;

	});

	//	座標更新
	transform_.UpdateMatrix();
	for (auto& i : partsTransform_) {
		i.UpdateMatrix();
	}
	backTrans_.UpdateMatrix();

}

void Boss::Draw3D(const Matrix4x4& viewProjectionMat) {
	//	描画
	for (uint8_t i = 0; i < parts_.size(); i++) {
		Model::ModelDraw(partsTransform_[i], viewProjectionMat, 0xffffffff, parts_[i].get());
	}
	//	弾の描画
	for (auto& i : bullets_) {
		i->Draw3D(viewProjectionMat);
	}
	Model::ModelDraw(backTrans_, viewProjectionMat, 0xffffff99, back_.get());
}

void Boss::Attack(Notes* notes) {
	//	攻撃フラグが立たれたとき
	if (attackFlag_) {

		// アニメーションの実行
		animeFlag_ = true;

		// 座標の設定
		// 弾の生成
		std::shared_ptr<BossBullet> tmpBullet = std::make_shared<BossBullet>(bulletModel_);
		tmpBullet->SetNotes(notes);

		bullets_.push_back(tmpBullet);
		(*bullets_.rbegin())->SetTransform(&transform_);

		//フラグを折る
		attackFlag_ = false;

	}
}
