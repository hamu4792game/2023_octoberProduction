#include "BossBullet.h"

BossBullet::BossBullet(Model* model)
{
	model_ = model;
	transform_.translation_ = Vector3();
}

void BossBullet::Update() {
	
}

void BossBullet::Draw3D(const Matrix4x4& viewProjection) {
	Model::ModelDraw(transform_, viewProjection, 0xffffffff, model_);
}
