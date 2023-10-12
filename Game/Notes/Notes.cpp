#include "Notes.h"

//静的メンバ変数実体
bool NoteLong::isHitStart_ = false;

Notes::Notes() {

}

void Notes::Initialize() {

}

void Notes::UpdateFlag() {

	//プレイヤーに当たってから、キーを押す、又はスルーするまで処理
	if (!isHit_ || !isMiss_) {

		//ノーツに触れる前に早めに押してしまったらミス
		if (judgeFrame_ <= 15.0f && !isTouch_ && player_->GetIsTap()) {
			isMiss_ = true;
			player_->IsMiss();
		}

		//タッチしている間にプレイヤーがキーを押したらヒット判定
		if (isTouch_ && player_->GetIsTap()) {
			isHit_ = true;
			player_->IsHit();
		}

	}

}

void Notes::Update() {

	RestoreSize();

	//判定ラインに触れた瞬間にフラグを立たせる
	if (judgeFrame_ <= 10.0f && !isTouch_) {
		isTouch_ = true;
	}

	//プレイヤーがノーツをスルーしたらミス
	if (isTouch_ && judgeFrame_ <= -10.0f) {
		isMiss_ = true;
		player_->IsMiss();
	}

	//判定フレーム更新
	if (player_->GetIsMove()) {
		judgeFrame_ -= 1.0f;
		worldTransformInside_.translation_.x = judgeFrame_;
	}

	worldTransform_.UpdateMatrix();
	worldTransformInside_.UpdateMatrix();

}

void Notes::RestoreSize() {

	if (worldTransform_.scale_.x >= kNormalSize) {
		worldTransform_.scale_.x -= 0.1f;
	}

	if (worldTransform_.scale_.y >= kNormalSize) {
		worldTransform_.scale_.y -= 0.1f;
	}

	if (worldTransform_.scale_.z >= kNormalSize) {
		worldTransform_.scale_.z -= 0.1f;
	}

	if (worldTransform_.scale_.x < kNormalSize) {
		worldTransform_.scale_.x = kNormalSize;
	}

	if (worldTransform_.scale_.y < kNormalSize) {
		worldTransform_.scale_.y = kNormalSize;
	}

	if (worldTransform_.scale_.z < kNormalSize) {
		worldTransform_.scale_.z = kNormalSize;
	}

}

void Notes::Draw(const Matrix4x4& viewProjection) {

	model_->ModelDraw(worldTransform_, viewProjection, 0xffffffff, model_);

	model_->ModelDraw(worldTransformInside_, viewProjection, 0xffffffff, model_);

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

void NoteLong::UpdateFlag() {

	//プレイヤーに当たってから、キーを押す、又はスルーするまで処理
	if (!isHit_ || !isMiss_) {

		//ノーツに触れる前に早めに押してしまったらミス
		if (judgeFrame_ <= 15.0f && !isTouch_ && player_->GetIsTap()) {
			isMiss_ = true;
			player_->IsMiss();
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

	}

}

void NoteLong::Update() {

	RestoreSize();

	//判定ラインに触れた瞬間にフラグを立たせる
	if (judgeFrame_ <= 10.0f && !isTouch_) {
		isTouch_ = true;
	}

	//プレイヤーがノーツをスルーしたらミス
	if (isTouch_ && judgeFrame_ <= -10.0f) {
		isMiss_ = true;
		player_->IsMiss();
	}

	//判定フレーム更新
	if (player_->GetIsMove()) {
		judgeFrame_ -= 1.0f;
		worldTransformInside_.translation_.x = judgeFrame_;
	}

	worldTransform_.UpdateMatrix();
	worldTransformInside_.UpdateMatrix();

}

NoteDamage::NoteDamage() {

}

void NoteDamage::Initialize() {

	type_ = Damage;

}

void NoteDamage::UpdateFlag() {

	//プレイヤーに当たってから、キーを押す、又はスルーするまで処理
	if (!isHit_ || !isMiss_) {

		//タッチしている間にプレイヤーがキーを押したらミス判定
		if (isTouch_ && player_->GetIsTap()) {
			isMiss_ = true;
			player_->IsMiss();
		}

	}

}

void NoteDamage::Update() {

	RestoreSize();

	//判定ラインに触れた瞬間にフラグを立たせる
	if (judgeFrame_ <= 10.0f && !isTouch_) {
		isTouch_ = true;
	}

	//プレイヤーがノーツをスルーしたらヒット判定(暫定。今後変更する可能性あり)
	if (isTouch_ && judgeFrame_ <= -10.0f) {
		isHit_ = true;
		player_->IsHit();
	}

	//判定フレーム更新
	if (player_->GetIsMove()) {
		judgeFrame_ -= 1.0f;
		worldTransformInside_.translation_.x = judgeFrame_;
	}

	worldTransform_.UpdateMatrix();
	worldTransformInside_.UpdateMatrix();

}
