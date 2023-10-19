#include "BattleAnimation.h"
#include "externals/imgui/imgui.h"

BattleAnimation::BattleAnimation(std::shared_ptr<Camera> camera) {
	camera_ = camera;
	hero_ = std::make_unique<Hero>();
	boss_ = std::make_unique<Boss>();
}

void BattleAnimation::Initialize() {

	hero_->Initialize();
	boss_->Initialize();

	camera_->transform.parent_ = &hero_->GetTransform();
	boss_->SetParent(&hero_->GetTransform());

	if (true) {
		camera_->transform.translation_ = Vector3(15.0f, 6.0f, -18.0f);
		camera_->transform.rotation_ = Vector3(0.105f, -0.472f, 0.0f);
	}

	stageCount = 0u;

	stage_.clear();
	for (uint8_t i = 0; i < 4; i++) {
		stage_.push_back(std::make_unique<Stage>(stageModel_));
		// 逆イテレーター z軸ステージサイズ * ステージの数
		(*stage_.rbegin())->Initialize(Vector3(0.0f, 0.0f, 140.0f * stageCount));
		stageCount++;
	}

}

void BattleAnimation::Update() {

	// ステージの先頭要素と主人公のベクトルを取得
	Vector3 dis = FindVector(hero_->GetTransform().translation_, stage_.front()->GetTransform().translation_);
	ImGui::Begin("Distance");
	ImGui::Text("%f", dis.z);
	ImGui::End();
	// z軸が指定以上離れている(見えなくなる)場合
	if (dis.z <= -140.0f * 2.0f) {
		stageCount = 0u;
		for (auto& i : stage_) {
			i->SetPosition(Vector3(0.0f, 0.0f, 140.0f * stageCount));
			stageCount++;
		}
		hero_->SetPosition(Vector3(7.5f, 0.0f, 0.0f));
	}

	hero_->Update();
	boss_->Update();
	for (auto& i : stage_) {
		i->Update();
	}
}

void BattleAnimation::Draw3D(const Matrix4x4& viewProjectionMat) {
	hero_->Draw3D(viewProjectionMat);
	boss_->Draw3D(viewProjectionMat);

	for (auto& i : stage_) {
		i->Draw3D(viewProjectionMat);
	}
}
