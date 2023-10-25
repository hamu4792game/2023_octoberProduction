#include "NotesEffect.h"

NotesEffect::NotesEffect()
{
}

NotesEffect::~NotesEffect()
{
}



void NotesEffect::Initialize() {

}

void NotesEffect::Update() {

	if (worldTransform_.translation_.y >= 50.0f) {
		isDead_ = true;
	}

	worldTransform_.rotation_.y += 3.14f / 30.0f;
	worldTransform_.translation_.x += sinf(worldTransform_.translation_.y) * 0.5f;
	worldTransform_.translation_.y += 0.1f;
	worldTransform_.translation_.z += 2.0f;
	worldTransform_.UpdateMatrix();
}

void NotesEffect::Draw(const Matrix4x4& viewProjection) {

	modelNotes_->ModelDraw(worldTransform_, viewProjection, color_, modelNotes_);

}


