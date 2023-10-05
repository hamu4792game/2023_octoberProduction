#include "Notes.h"

Notes::Notes() {

}

void Notes::Initialize() {

}

void Notes::Update() {

	//プレイヤーに当たってから、キーを押す、又はスルーするまで
	if (!isHit_ || !isMiss_) {

		//プレイヤーが触れた瞬間にフラグを立たせる
		if (Distance(worldTransform_.translation_, player_->GetPosition()) <= 10.0f && !isTouch_) {
			isTouch_ = true;
		}

		//タッチしている間にプレイヤーがキーを押したらヒット判定
		if (player_->GetIsTap() && isTouch_) {
			isHit_ = true;
		}

		if (isTouch_ && Distance(worldTransform_.translation_, player_->GetPosition()) >= 10.0f) {
			isMiss_ = true;
		}

	}

	

	worldTransform_.UpdateMatrix();

}

void Notes::Draw(const Matrix4x4& viewProjection) {

	model_->ModelDraw(worldTransform_, viewProjection, 0xffffffff, model_);

}

void Notes::ModelLoad(Model* model) {

	model_ = model;
	
}

NoteNormal::NoteNormal() {

}

void NoteNormal::Initialize() {

	type_ = Normal;

}

NoteLong::NoteLong() {

}

void NoteLong::Initialize() {

	type_ = Long;

}

NoteDamage::NoteDamage() {

}

void NoteDamage::Initialize() {

	type_ = Damage;

}
