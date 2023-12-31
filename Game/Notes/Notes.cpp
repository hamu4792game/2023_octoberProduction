#include "Notes.h"
#include "Game/Character/Hero/Hero.h"
#include "Game/Character/Boss/Boss.h"

//静的メンバ変数実体
bool NoteLong::isHitStart_ = false;
std::unique_ptr<AudioInput> Notes::notesSE_[3][8] = { nullptr };
AudioInput* Notes::currentNotesSE_[2][8] = { nullptr };
int Notes::Combo = 0;
bool Notes::isUpSE = false;

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

		//ノーツに触れる前に早めに押してしまったらミス(チュートリアルでは無効)
		if (!isTutorialNotes_ && judgeFrame_ <= 15.0f && !isTouch_ && player_->GetIsTap()) {
			isMiss_ = true;
			notesSE_[2][0]->SoundStop();
			notesSE_[2][0]->SoundPlayWave();
			notesSE_[2][0]->SetVolume(0.5f);
			Combo = 0;
			player_->IsMiss();
		}

		//タッチしている間にプレイヤーがキーを押したらヒット判定
		if (!isTutorialNotes_ && isTouch_ && player_->GetIsTap()) {
			isHit_ = true;
			Combo++;

			size_t num = rand() % 8;

			if (isUpSE) {

				size_t num2 = rand() % 2;

				for (size_t n = 0; n < 2; n++) {

					for (size_t i = 0; i < 8; i++) {

						currentNotesSE_[n][i]->SoundStop();

						if (i == num && n == num2) {
							currentNotesSE_[n][i]->SoundPlayWave();
							currentNotesSE_[n][i]->SetVolume(0.5f);
						}

					}

				}

				switch ((num + (num * num2)) % 15)
				{
				default:
				case 0:
					color_ = 0x000000ff;
					break;
				case 1:
					color_ = 0x883333ff;
					break;
				case 2:
					color_ = 0x33bb33ff;
					break;
				case 3:
					color_ = 0x333388ff;
					break;
				case 4:
					color_ = 0x888833ff;
					break;
				case 5:
					color_ = 0x883388ff;
					break;
				case 6:
					color_ = 0x883333ff;
					break;
				case 7:
					color_ = 0xaaffaaff;
					break;
				case 8:
					color_ = 0xcc7777ff;
					break;
				case 9:
					color_ = 0x77cc77ff;
					break;
				case 10:
					color_ = 0x7777ccff;
					break;
				case 11:
					color_ = 0xcccc77ff;
					break;
				case 12:
					color_ = 0xcc77ccff;
					break;
				case 13:
					color_ = 0x77ccccff;
					break;
				case 14:
					color_ = 0xffffffff;
					break;
				}

			}
			else {

				for (size_t i = 0; i < 8; i++) {

					currentNotesSE_[0][i]->SoundStop();

					if (i == num) {
						currentNotesSE_[0][i]->SoundPlayWave();
						currentNotesSE_[0][i]->SetVolume(0.5f);
					}

				}

				switch (num % 8)
				{
				default:
				case 0:
					color_ = 0x888888ff;
					break;
				case 1:
					color_ = 0xcc7777ff;
					break;
				case 2:
					color_ = 0x77cc77ff;
					break;
				case 3:
					color_ = 0x7777ccff;
					break;
				case 4:
					color_ = 0xcccc77ff;
					break;
				case 5:
					color_ = 0xcc77ccff;
					break;
				case 6:
					color_ = 0x77ccccff;
					break;
				case 7:
					color_ = 0xffffffff;
					break;
				}

			}

			player_->IsHit();
		}
		//タッチしている間にプレイヤーがキーを押したらヒット判定(チュートリアルではコンボ無効)
		else if (isTouch_ && player_->GetIsTap()) {
			isHit_ = true;
			size_t num = rand() % 8;
			for (size_t i = 0; i < 8; i++) {

				currentNotesSE_[0][i]->SoundStop();

				if (i == num) {
					currentNotesSE_[0][i]->SoundPlayWave();
					currentNotesSE_[0][i]->SetVolume(0.5f);
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
		notesSE_[2][0]->SoundStop();
		notesSE_[2][0]->SoundPlayWave();
		notesSE_[2][0]->SetVolume(0.5f);
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
	//	最小まで縮まらないようにclamp
	if (worldTransformLine_.scale_.x < 1.5f) {
		worldTransformLine_.scale_.x = 1.5f;
		worldTransformLine_.scale_.y = 1.5f;
		worldTransformLine_.scale_.z = 1.5f;
	}
	worldTransformLine_.translation_ = hero_->GetTransform().translation_;
	worldTransformLine_.translation_.y += 3.0f;
	worldTransformLine_.translation_.z += 5.0f;

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
		//modelList_[3]->ModelDraw(worldTransformLine_, viewProjection, 0xffffffff, modelList_[3]);
		Model::ModelDraw(worldTransformLine_, viewProjection, 0xffffffff, modelList_[3]);
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

	notesSE_[0][0]->SoundLoadWave("./Resources/notesSE/notes_e_c3.wav");
	notesSE_[0][1]->SoundLoadWave("./Resources/notesSE/notes_e_d3.wav");
	notesSE_[0][2]->SoundLoadWave("./Resources/notesSE/notes_e_me3.wav");
	notesSE_[0][3]->SoundLoadWave("./Resources/notesSE/notes_e_f3.wav");
	notesSE_[0][4]->SoundLoadWave("./Resources/notesSE/notes_e_g3.wav");
	notesSE_[0][5]->SoundLoadWave("./Resources/notesSE/notes_e_ma3.wav");
	notesSE_[0][6]->SoundLoadWave("./Resources/notesSE/notes_e_mb3.wav");
	notesSE_[0][7]->SoundLoadWave("./Resources/notesSE/notes_e_c4.wav");
	notesSE_[1][0]->SoundLoadWave("./Resources/notesSE/notes_e_d4.wav");
	notesSE_[1][1]->SoundLoadWave("./Resources/notesSE/notes_e_me4.wav");
	notesSE_[1][2]->SoundLoadWave("./Resources/notesSE/notes_e_f4.wav");
	notesSE_[1][3]->SoundLoadWave("./Resources/notesSE/notes_e_g4.wav");
	notesSE_[1][4]->SoundLoadWave("./Resources/notesSE/notes_e_ma4.wav");
	notesSE_[1][5]->SoundLoadWave("./Resources/notesSE/notes_e_mb4.wav");
	notesSE_[1][6]->SoundLoadWave("./Resources/notesSE/notes_e_c5.wav");
	notesSE_[1][7]->SoundLoadWave("./Resources/notesSE/notes_e_d5.wav");
	notesSE_[2][0]->SoundLoadWave("./Resources/notesSE/notes_e_f1.wav");
	notesSE_[2][1]->SoundLoadWave("./Resources/notesSE/notes_v_d3.wav");
	notesSE_[2][2]->SoundLoadWave("./Resources/notesSE/notes_v_e3.wav");
	notesSE_[2][3]->SoundLoadWave("./Resources/notesSE/notes_v_f3.wav");
	notesSE_[2][4]->SoundLoadWave("./Resources/notesSE/notes_v_g3.wav");
	notesSE_[2][5]->SoundLoadWave("./Resources/notesSE/notes_v_a3.wav");
	notesSE_[2][6]->SoundLoadWave("./Resources/notesSE/notes_v_b3.wav");
	notesSE_[2][7]->SoundLoadWave("./Resources/notesSE/notes_v_c4.wav");

	for (int n = 0; n < 2; n++) {

		for (int i = 0; i < 8; i++) {

			currentNotesSE_[n][i] = notesSE_[n][i].get();

		}

		
	}

}

