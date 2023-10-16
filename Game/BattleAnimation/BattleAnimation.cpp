#include "BattleAnimation.h"

BattleAnimation::BattleAnimation(std::shared_ptr<Camera> camera)
{
	camera_ = camera;
	hero_ = std::make_unique<Hero>();
}

void BattleAnimation::Initialize()
{
	hero_->Initialize();
}

void BattleAnimation::Update()
{
	hero_->Update();
}

void BattleAnimation::Draw3D(const Matrix4x4& viewProjectionMat)
{
	hero_->Draw3D(viewProjectionMat);
}
