#include "Stage.h"
#include "externals/imgui/imgui.h"

Stage::Stage(std::vector<std::shared_ptr<Model>> model) {
	models_ = model;
	transforms_.resize(models_.size());
}

void Stage::Initialize(const Vector3& position) {
	worldTrans_.translation_ = position;
	for (auto& i : transforms_) {
		i.parent_ = &worldTrans_;
	}
	transforms_[0].scale_ = Vector3(50.0f, 1.0f, 70.0f);

	transforms_[1].translation_ = { -23.0f,9.0f,0.0f };
	transforms_[1].scale_ = { 5.0f,8.0f,15.0f };
	transforms_[1].rotation_ = { 0.0f,3.14f,0.0f };
	transforms_[2].translation_ = { -27.3f,13.0f,113.0f };
	transforms_[2].scale_ = { 5.0f,12.5f,11.0f };
	transforms_[2].rotation_ = { 0.0f,3.14f,0.0f };
}

void Stage::Update() {

#pragma region ImGui

	ImGui::Begin("Stage");
	if (ImGui::TreeNode("transform")) {
		ImGui::DragFloat3("Translate",	&worldTrans_.translation_.x, 0.1f);
		ImGui::DragFloat3("Scale",		&worldTrans_.scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate",		&worldTrans_.rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("floor")) {
		ImGui::DragFloat3("Translate", &transforms_[0].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &transforms_[0].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &transforms_[0].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("buil1")) {
		ImGui::DragFloat3("Translate", &transforms_[1].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &transforms_[1].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &transforms_[1].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("buil2")) {
		ImGui::DragFloat3("Translate",	&transforms_[2].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale",		&transforms_[2].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate",		&transforms_[2].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("buil3")) {
		ImGui::DragFloat3("Translate",	&transforms_[3].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale",		&transforms_[3].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate",		&transforms_[3].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("buil4")) {
		ImGui::DragFloat3("Translate",	&transforms_[4].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale",		&transforms_[4].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate",		&transforms_[4].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("buil5")) {
		ImGui::DragFloat3("Translate",	&transforms_[5].translation_.x, 0.1f);
		ImGui::DragFloat3("Scale",		&transforms_[5].scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate",		&transforms_[5].rotation_.x, AngleToRadian(1.0f));
		ImGui::TreePop();
	}
	ImGui::End();

#pragma endregion

	worldTrans_.UpdateMatrix();
	for (auto& i : transforms_) {
		i.UpdateMatrix();
	}

}

void Stage::Draw3D(const Matrix4x4& viewProjectionMat) {
	for (uint8_t i = 0; i < transforms_.size(); i++) {
		Model::ModelDraw(transforms_[i], viewProjectionMat, 0xffffffff, models_[i].get());
	}
}

void Stage::SetPosition(const Vector3& position) {
	worldTrans_.translation_ = position;
}
