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

	if (isMiss_ == false) {
		if (worldTransform_.translation_.y >= 50.0f) {
			isDead_ = true;
		}

		worldTransform_.rotation_.y += 3.14f / 30.0f;
		worldTransform_.translation_.x += sinf(worldTransform_.translation_.y) * 0.5f;
		worldTransform_.translation_.y += 0.1f;
		worldTransform_.translation_.z += 2.0f;
	}
	else {

		if (worldTransform_.translation_.y <= -10.0f) {
			isDead_ = true;
		}

		if (count_ > 0) {
			count_--;
			worldTransform_.translation_.y += float(count_) / 5.0f;
		}

		worldTransform_.rotation_.x += 3.14f / 30.0f;
		worldTransform_.rotation_.y += 3.14f / 60.0f;
		worldTransform_.rotation_.z += 3.14f / 45.0f;
		worldTransform_.translation_.x += sinf(worldTransform_.translation_.y) * 0.3f;
		worldTransform_.translation_.y -= 0.1f;
		worldTransform_.translation_.z += 1.3f;

	}


	worldTransform_.UpdateMatrix();

}

void NotesEffect::Draw(const Matrix4x4& viewProjection) {

	modelNotes_->ModelDraw(worldTransform_, viewProjection, color_, modelNotes_);

}


