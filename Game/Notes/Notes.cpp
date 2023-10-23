#include "Notes.h"
#include "Game/Character/Hero/Hero.h"
#include "Game/Character/Boss/Boss.h"

//静的メンバ変数実体
bool NoteLong::isHitStart_ = false;
std::unique_ptr<AudioInput> Notes::notesSE_[3][8] = { nullptr };
AudioInput* Notes::currentNotesSE_[8] = { nullptr };
int Notes::Combo = 0;

Notes::Notes() {

}

void Notes::Initialize() {

	worldTransformLine_.scale_ *= 0.0f;
	worldTransformLine_.translation_.y = 10.0f;
	/*worldTransformLine_.parent_ = &worldTransform_;*/
	worldTransformLine_.UpdateMatrix();

}

void Notes::UpdateFlag() {

	//プレイヤーに当たってから、キーを押す、又はスルーするまで処理
	if (!isHit_ || !isMiss_) {

		//ノーツに触れる前に早めに押してしまったらミス
		if (judgeFrame_ <= 15.0f && !isTouch_ && player_->GetIsTap()) {
			isMiss_ = true;
			Combo = 0;
			player_->IsMiss();
		}

		//タッチしている間にプレイヤーがキーを押したらヒット判定
		if (isTouch_ && player_->GetIsTap()) {
			isHit_ = true;
			Combo++;
			size_t num = rand() % 8;
			for (size_t i = 0; i < 8; i++) {

				currentNotesSE_[i]->SoundStop();

				if (i == num) {
					currentNotesSE_[i]->SoundPlayWave();
					currentNotesSE_[i]->SetVolume(0.2f);
				}
				
			}
		
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
		Combo = 0;
		player_->IsMiss();
	}

	//描画上のモデル座標更新
	if (judgeFrame_ <= 30.0f) {
		worldTransform_.translation_ = makeCatmullRom(startPosition_, startPosition_, goalPosition_, goalPosition_,
			float((40.0f - (judgeFrame_ + 10.0f)) / 40.0f));
	}

	if (judgeFrame_ <= 60.0f && !isAttack_) {

		boss_->SetFlag(true);
		boss_->Attack(this);
		isAttack_ = true;

	}

	//判定フレーム更新
	worldTransformInside_.translation_.x = judgeFrame_;
	worldTransformLine_.scale_ = { judgeFrame_, judgeFrame_, judgeFrame_ };
	worldTransformLine_.scale_ *= 1.0f / 2.0f;
	worldTransformLine_.translation_ = hero_->GetTransform().translation_;

	worldTransform_.UpdateMatrix();
	worldTransformLine_.UpdateMatrix();
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

	/*if (judgeFrame_ <= 30.0f) {
		model_->ModelDraw(worldTransform_, viewProjection, 0xffffffff, model_);
	}

	model_->ModelDraw(worldTransformInside_, viewProjection, 0xffffffff, model_);*/

	if (judgeFrame_ <= 30.0f && judgeFrame_ >= 0.0f) {
		modelList_[3]->ModelDraw(worldTransformLine_, viewProjection, 0xffffffff, modelList_[3]);
	}

}

void Notes::Draw2D(const Matrix4x4& viewProjection) {

	/*if (judgeFrame_ <= 20.0f && judgeFrame_ >= 0.0f) {
		lineTexture_->TextureDraw(worldTransformLine_, viewProjection, 0xffffffff, lineTexture_);
	}*/

}

void Notes::ModelLoad(std::vector<Model*> models) {

	modelList_ = models;
	
}

NoteNormal::NoteNormal() {

}

void NoteNormal::Initialize() {

	type_ = Normal;
	model_ = modelList_[0];
	worldTransformLine_.scale_ *= 0.0f;
	worldTransformLine_.translation_.y = 10.0f;
	/*worldTransformLine_.parent_ = &worldTransform_;*/
	worldTransformLine_.UpdateMatrix();

}

NoteLong::NoteLong() {

}

//静的初期化
void NoteLong::StaticInitialize() {
	isHitStart_ = false;
}

void NoteLong::Initialize() {

	type_ = Long;
	model_ = modelList_[1];
	worldTransformLine_.scale_ *= 0.0f;
	worldTransformLine_.translation_.y = 10.0f;
	/*worldTransformLine_.parent_ = &worldTransform_;*/
	worldTransformLine_.UpdateMatrix();

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
	worldTransformInside_.translation_.x = judgeFrame_;

	worldTransform_.UpdateMatrix();
	worldTransformLine_.UpdateMatrix();
	worldTransformInside_.UpdateMatrix();

}

NoteDamage::NoteDamage() {

}

void NoteDamage::Initialize() {

	type_ = Damage;
	model_ = modelList_[2];
	worldTransformLine_.scale_ *= 0.0f;
	worldTransformLine_.translation_.y = 10.0f;
	/*worldTransformLine_.parent_ = &worldTransform_;*/
	worldTransformLine_.UpdateMatrix();

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
	worldTransformInside_.translation_.x = judgeFrame_;

	worldTransform_.UpdateMatrix();
	worldTransformLine_.UpdateMatrix();
	worldTransformInside_.UpdateMatrix();

}

void Notes::StaticInitialize() {

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 8; x++) {
			notesSE_[y][x] = std::make_unique<AudioInput>();
		}
	}

	notesSE_[0][0]->SoundLoadWave("./Resources/notesSE/notes_p_c3.wav");
	notesSE_[0][1]->SoundLoadWave("./Resources/notesSE/notes_p_d3.wav");
	notesSE_[0][2]->SoundLoadWave("./Resources/notesSE/notes_p_e3.wav");
	notesSE_[0][3]->SoundLoadWave("./Resources/notesSE/notes_p_f3.wav");
	notesSE_[0][4]->SoundLoadWave("./Resources/notesSE/notes_p_g3.wav");
	notesSE_[0][5]->SoundLoadWave("./Resources/notesSE/notes_p_a3.wav");
	notesSE_[0][6]->SoundLoadWave("./Resources/notesSE/notes_p_b3.wav");
	notesSE_[0][7]->SoundLoadWave("./Resources/notesSE/notes_p_c4.wav");
	notesSE_[1][0]->SoundLoadWave("./Resources/notesSE/notes_s_c3.wav");
	notesSE_[1][1]->SoundLoadWave("./Resources/notesSE/notes_s_d3.wav");
	notesSE_[1][2]->SoundLoadWave("./Resources/notesSE/notes_s_e3.wav");
	notesSE_[1][3]->SoundLoadWave("./Resources/notesSE/notes_s_f3.wav");
	notesSE_[1][4]->SoundLoadWave("./Resources/notesSE/notes_s_g3.wav");
	notesSE_[1][5]->SoundLoadWave("./Resources/notesSE/notes_s_a3.wav");
	notesSE_[1][6]->SoundLoadWave("./Resources/notesSE/notes_s_b3.wav");
	notesSE_[1][7]->SoundLoadWave("./Resources/notesSE/notes_s_c4.wav");
	notesSE_[2][0]->SoundLoadWave("./Resources/notesSE/notes_v_c3.wav");
	notesSE_[2][1]->SoundLoadWave("./Resources/notesSE/notes_v_d3.wav");
	notesSE_[2][2]->SoundLoadWave("./Resources/notesSE/notes_v_e3.wav");
	notesSE_[2][3]->SoundLoadWave("./Resources/notesSE/notes_v_f3.wav");
	notesSE_[2][4]->SoundLoadWave("./Resources/notesSE/notes_v_g3.wav");
	notesSE_[2][5]->SoundLoadWave("./Resources/notesSE/notes_v_a3.wav");
	notesSE_[2][6]->SoundLoadWave("./Resources/notesSE/notes_v_b3.wav");
	notesSE_[2][7]->SoundLoadWave("./Resources/notesSE/notes_v_c4.wav");

	for (int i = 0; i < 8; i++) {
		currentNotesSE_[i] = notesSE_[2][i].get();
	}

}

