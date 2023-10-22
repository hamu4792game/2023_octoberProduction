#include "BossBullet.h"

BossBullet::BossBullet(Model* model) {
	model_ = model;
	transform_.translation_ = Vector3(3.0f, 0.0f, 0.0f);
	transform_.scale_ = Vector3(1.0f, 1.0f, 1.0f);
}

void BossBullet::Update() {
	transform_.UpdateMatrix();
}

void BossBullet::Draw3D(const Matrix4x4& viewProjection) {
	Model::ModelDraw(transform_, viewProjection, 0xffffffff, model_);
}

void BossBullet::SetTransform(const WorldTransform* trans) {
	transform_.parent_ = trans;
}
