#include "Hero.h"
#include "Game/PartsEnum.h"
#include "externals/imgui/imgui.h"

void Hero::Initialize() {
	partsTransform_.resize(parts_.size());
	partsTransform_[static_cast<uint8_t>(HeroParts::Body)].parent_ = &transform_;
	for (int8_t i = static_cast<uint8_t>(HeroParts::Body) + 1; i < static_cast<uint8_t>(HeroParts::kMaxCount); i++)	{
		partsTransform_[i].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::Body)];
	}

	transform_.translation_ = Vector3(-3.0f, 12.0f, -85.0f);
	partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.y = 2.0f;
}

void Hero::Update() {

	ImGui::Begin("Hero");

	if (ImGui::TreeNode("Transform")) {
		ImGui::DragFloat3("Translate", &transform_.translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &transform_.scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &transform_.rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
#pragma region Parts

	if (ImGui::TreeNode("Body")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::Body)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::Body)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::Body)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Head")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::Head)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::Head)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Waist")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("RightUpperArm")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("LeftUpperArm")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("RightBottomArm")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("LeftBottomArm")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("RightUpperLeg")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("LeftUpperLeg")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("RightBottomLeg")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("LeftBottomLeg")) {
		ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	
#pragma endregion

	ImGui::End();

	//	座標更新
	transform_.UpdateMatrix();
	for (uint8_t i = 0; i < partsTransform_.size(); i++) {
		partsTransform_[i].UpdateMatrix();
	}
}
