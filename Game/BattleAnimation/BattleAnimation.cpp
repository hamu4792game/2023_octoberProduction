#include "BattleAnimation.h"

BattleAnimation::BattleAnimation(std::shared_ptr<Camera> camera) {
	camera_ = camera;
	hero_ = std::make_unique<Hero>();
	boss_ = std::make_unique<Boss>();
}

void BattleAnimation::Initialize() {
	hero_->Initialize();
	boss_->Initialize();
}

void BattleAnimation::Update() {
	hero_->Update();
	boss_->Update();
}

void BattleAnimation::Draw3D(const Matrix4x4& viewProjectionMat) {
	hero_->Draw3D(viewProjectionMat);
	boss_->Draw3D(viewProjectionMat);
}
