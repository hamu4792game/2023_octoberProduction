#include "Boss.h"
#include "Game/PartsEnum.h"
#include "externals/imgui/imgui.h"

void Boss::Initialize() {
	partsTransform_.resize(parts_.size());
	partsTransform_[static_cast<uint8_t>(BossParts::Body)].parent_ = &transform_;
	for (int8_t i = static_cast<uint8_t>(BossParts::Body) + 1; i < static_cast<uint8_t>(BossParts::kMaxCount); i++) {
		partsTransform_[i].parent_ = &partsTransform_[static_cast<uint8_t>(BossParts::Body)];
	}

	transform_.translation_ = Vector3(2.0f, 2.0f, 35.0f);
	partsTransform_[static_cast<uint8_t>(BossParts::Head)].translation_.y = 2.0f;
}

void Boss::Update() {

	ImGui::Begin("Boss");
	ImGui::DragFloat3("trans", &transform_.translation_.x);
	ImGui::DragFloat3("scale", &transform_.scale_.x);
	ImGui::End();

	//	座標更新
	transform_.UpdateMatrix();
	for (auto& i : partsTransform_) {
		i.UpdateMatrix();
	}
}
