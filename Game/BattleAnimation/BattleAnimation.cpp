#include "BattleAnimation.h"

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
	for (uint8_t i = 0; i < 3; i++) {
		stage_.push_back(std::make_unique<Stage>(stageModel_));
		// 逆イテレーター z軸ステージサイズ * ステージの数
		(*stage_.rbegin())->Initialize(Vector3(0.0f, 0.0f, 140.0f * stageCount));
		stageCount++;
	}

}

void BattleAnimation::Update() {
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
