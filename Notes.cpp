#include "Notes.h"

//静的メンバ変数実体
bool NoteLong::isHitStart_ = false;

Notes::Notes() {

}

void Notes::Initialize() {

}

void Notes::Update() {

	//プレイヤーに当たってから、キーを押す、又はスルーするまで処理
	if (!isHit_ || !isMiss_) {

		//プレイヤーが触れた瞬間にフラグを立たせる
		if (Distance(worldTransform_.translation_, player_->GetPosition()) <= 10.0f && !isTouch_) {
			isTouch_ = true;
		}

		//タッチしている間にプレイヤーがキーを押したらヒット判定
		if (isTouch_ && player_->GetIsTap()) {
			isHit_ = true;
			player_->IsHit();
		}

		//プレイヤーがノーツをスルーしたらミス
		if (isTouch_ && Distance(worldTransform_.translation_, player_->GetPosition()) >= 10.0f) {
			isMiss_ = true;
			player_->IsMiss();
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

//静的初期化
void NoteLong::StaticInitialize() {
	isHitStart_ = false;
}

void NoteLong::Initialize() {

	type_ = Long;

}

void NoteLong::Update() {

	//プレイヤーに当たってから、キーを押す、又はスルーするまで処理
	if (!isHit_ || !isMiss_) {

		//プレイヤーが触れた瞬間にフラグを立たせる
		if (Distance(worldTransform_.translation_, player_->GetPosition()) <= 10.0f && !isTouch_) {
			isTouch_ = true;
		}

		//終点の場合
		if (longNoteType_ == End && !isTouch_) {

			//スタートをタッチしたら
			if (isHitStart_) {
				onHoldNote_ = true;
				isHitStart_ = false;
			}

			//プレイヤーがロングノーツを取っている時に終点前でキーを離したらミス判定
			if (onHoldNote_ && player_->GetIsHold() == false) {
				isMiss_ = true;
				player_->IsMiss();
			}

		}

		//タッチしている間にプレイヤーがキーを押したらヒット判定(始点の場合)
		if (isTouch_ && player_->GetIsTap() && longNoteType_ == Start) {
			isHit_ = true;
			isHitStart_ = true;
			player_->IsHit();
		}
		//プレイヤーがキーを押していたらヒット判定(中点の場合)
		else if (isTouch_ && player_->GetIsHold() && longNoteType_ == Middle) {
			isHit_ = true;
			player_->IsHit();
		}
		//プレイヤーがキーを離したらヒット判定(終点の場合)
		else if (isTouch_ && player_->GetIsRelease() && longNoteType_ == End) {
			isHit_ = true;
			player_->IsHit();
		}

		//プレイヤーがノーツをスルーしたらミス
		if (isTouch_ && Distance(worldTransform_.translation_, player_->GetPosition()) >= 10.0f) {
			isMiss_ = true;
			player_->IsMiss();
		}

	}



	worldTransform_.UpdateMatrix();

}

NoteDamage::NoteDamage() {

}

void NoteDamage::Initialize() {

	type_ = Damage;

}

void NoteDamage::Update() {

	//プレイヤーに当たってから、キーを押す、又はスルーするまで処理
	if (!isHit_ || !isMiss_) {

		//プレイヤーが触れた瞬間にフラグを立たせる
		if (Distance(worldTransform_.translation_, player_->GetPosition()) <= 10.0f && !isTouch_) {
			isTouch_ = true;
		}

		//タッチしている間にプレイヤーがキーを押したらミス判定
		if (isTouch_ && player_->GetIsTap()) {
			isMiss_ = true;
			player_->IsMiss();
		}

		//プレイヤーがノーツをスルーしたらヒット判定(暫定。今後変更する可能性あり)
		if (isTouch_ && Distance(worldTransform_.translation_, player_->GetPosition()) >= 10.0f) {
			isHit_ = true;
			player_->IsHit();
		}

	}

}
