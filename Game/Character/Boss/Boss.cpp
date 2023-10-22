#include "Boss.h"
#include "Game/PartsEnum.h"
#include "externals/imgui/imgui.h"

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
}

void Boss::Update() {

	ImGui::Begin("Boss");
	ImGui::DragFloat3("trans", &transform_.translation_.x);
	ImGui::DragFloat3("scale", &transform_.scale_.x);
	ImGui::End();

	//	弾1つ1つの更新
	for (auto& i : bullets_) {
		i.Update();
	}

	//	playerが攻撃するフラグが立ったら
	if (bulletDieFlag_) {
		bullets_.erase(bullets_.begin());
		bulletDieFlag_ = false;
	}

	//	座標更新
	transform_.UpdateMatrix();
	for (auto& i : partsTransform_) {
		i.UpdateMatrix();
	}
}

void Boss::Attack() {
	//	攻撃フラグが立たれたとき
	if (attackFlag_) {

		// アニメーションの実行
		animeFlag_ = true;

		// 座標の設定
		// 弾の生成
		bullets_.push_back(BossBullet(bulletModel_));

		//フラグを折る
		attackFlag_ = false;

	}


	// 弾の移動 1.0f/30.0fでイージング


}
