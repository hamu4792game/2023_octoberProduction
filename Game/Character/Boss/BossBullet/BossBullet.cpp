#include "BossBullet.h"
#include "Engine/Easing/Ease.h"
#include <externals/imgui/imgui.h>

BossBullet::BossBullet(Model* model) {
	model_ = model;
	//transform_.translation_ = Vector3(3.0f, 0.0f, 0.0f);
	transform_.translation_ = Vector3(RandNum(-3.0f, 3.0f), RandNum(-0.5f, 0.5f), 0.0f);
	transform_.scale_ = Vector3(1.0f, 1.0f, 1.0f);
	start_ = transform_.translation_;
	nowFrame_ = 0.0f;
}

void BossBullet::Update() {
	
	nowFrame_ += 1.0f / 60.0f;

	if (notes_->GetIsHit() || notes_->GetIsMiss()) {
		isDead_ = true;
	}
	Vector3 end = parent_->translation_ - (parent_->translation_ * 2.0f);
	end.y = 1.0f; end.z += 5.0f;
	ImGui::DragFloat("ending", &end.z, 0.1f);

	transform_.translation_ = Ease::UseEase(start_, end, nowFrame_, 1.0f, Ease::EaseType::EaseInSine);
	transform_.UpdateMatrix();
}

void BossBullet::Draw3D(const Matrix4x4& viewProjection) {

	if (isDead_ == false) {
		Model::ModelDraw(transform_, viewProjection, 0xffffffff, model_);
	}

}

void BossBullet::SetTransform(const WorldTransform* trans) {
	transform_.parent_ = trans;
	parent_ = trans;
}
