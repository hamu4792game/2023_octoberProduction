#include "Hero.h"
#include "Game/PartsEnum.h"
#include "externals/imgui/imgui.h"

Hero::Hero() {
}

void Hero::Initialize()
{
	partsTransform_.resize(parts_.size());
	partsTransform_[static_cast<uint8_t>(HeroParts::Body)].parent_ = &transform_;
	for (int8_t i = static_cast<uint8_t>(HeroParts::Body) + 1; i < static_cast<uint8_t>(HeroParts::kMaxCount); i++)	{
		partsTransform_[i].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::Body)];
	}

	transform_.translation_.z = -80.0f;
}

void Hero::Update() {

	ImGui::Begin("Hero");
	ImGui::DragFloat3("trans", &transform_.translation_.x);
	ImGui::DragFloat3("scale", &transform_.scale_.x);
	ImGui::End();

	//	座標更新
	transform_.UpdateMatrix();
	for (uint8_t i = 0; i < partsTransform_.size(); i++) {
		partsTransform_[i].UpdateMatrix();
	}
}
