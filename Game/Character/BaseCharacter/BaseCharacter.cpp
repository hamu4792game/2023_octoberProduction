#include "BaseCharacter.h"

void BaseCharacter::Initialize() {
	partsTransform_.resize(parts_.size());

}

void BaseCharacter::Update() {
	//	座標更新
	transform_.UpdateMatrix();
	for (uint8_t i = 0; i < partsTransform_.size(); i++) {
		partsTransform_[i].UpdateMatrix();
	}
}

void BaseCharacter::Draw3D(const Matrix4x4& viewProjectionMat) {
	//	描画
	for (uint8_t i = 0; i < parts_.size(); i++) {
		Model::ModelDraw(partsTransform_[i], viewProjectionMat, 0xffffffff, parts_[i].get());
	}
}

