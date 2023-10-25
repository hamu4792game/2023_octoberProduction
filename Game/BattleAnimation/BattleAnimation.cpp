#include "BattleAnimation.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Base/MultipathRendering/MultipathRendering.h"

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

	stageCount = 0u;

	stage_.clear();
	for (uint8_t i = 0; i < 6; i++) {
		stage_.push_back(std::make_unique<Stage>(stageModel_));
		// 逆イテレーター z軸ステージサイズ * ステージの数
		(*stage_.rbegin())->Initialize(Vector3(0.0f, 0.0f, 140.0f * stageCount));
		stageCount++;
	}

}

void BattleAnimation::Update() {

	// ステージの先頭要素と主人公のベクトルを取得
	Vector3 dis = FindVector(hero_->GetTransform().translation_, stage_.front()->GetTransform().translation_);

	//if (KeyInput::PushKey(DIK_SPACE)) {
	//	uint8_t handle = static_cast<uint32_t>(movepattern_);
	//	handle++;
	//	movepattern_ = static_cast<MovePattern>(handle);
	//	if (movepattern_ == MovePattern::kMaxCount) {
	//		movepattern_ = MovePattern::Run;
	//	}
	//	cameraMoveFlag = true;
	//	SetCameraMove();
	//}


	// z軸が指定以上離れている(見えなくなる)場合
	if (dis.z <= -140.0f * 2.0f) {
		stageCount = 0u;
		for (auto& i : stage_) {
			i->SetPosition(Vector3(0.0f, 0.0f, 140.0f * stageCount));
			i->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			stageCount++;
		}
		hero_->SetPosition(Vector3(7.5f, 0.0f, 0.0f));
		isLoop_ = true;
	}

	
	hero_->Move();

	hero_->Update();
	boss_->Update();
	for (auto& i : stage_) {
		i->Update();
	}
}

void BattleAnimation::Draw3D(const Matrix4x4& viewProjectionMat) {
	for (auto& i : stage_) {
		i->Draw3D(viewProjectionMat);
	}
	hero_->Draw3D(viewProjectionMat);
	boss_->Draw3D(viewProjectionMat);

}
