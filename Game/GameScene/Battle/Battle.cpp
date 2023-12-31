#include "Battle.h"
#include "math/Rand.h"
#include "FrameCount.h"
#include "Engine/Easing/Ease.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Base/MultipathRendering/MultipathRendering.h"
#include "Game/GameScene/GameScene.h"

Battle::Battle(std::shared_ptr<Camera> camera)
{
	camera_ = camera.get();

	player_ = std::make_unique<Player>();
	player_->ModelLoad();

	skydome_ = std::make_unique<Skydome>();
	skydome_->ModelLoad();

	makeCatmull_ = std::make_unique<MakeCatmull>();

	makeCatmull_->Initialize();

	battleAnimation_ = std::make_unique<BattleAnimation>(camera);
	
	for (int i = 0; i < 9; i++) {
		loopBGMs_[i] = std::make_unique<AudioInput>();
	}

	//	ロード
	loopBGMs_[0]->SoundLoadWave("./Resources/loopBGM/tempo_01.wav");
	loopBGMs_[1]->SoundLoadWave("./Resources/loopBGM/tempo_02.wav");
	loopBGMs_[2]->SoundLoadWave("./Resources/loopBGM/tempo_03.wav");
	loopBGMs_[3]->SoundLoadWave("./Resources/loopBGM/tempo_04.wav");
	loopBGMs_[4]->SoundLoadWave("./Resources/loopBGM/tempo_05.wav");
	loopBGMs_[5]->SoundLoadWave("./Resources/loopBGM/tempo_06.wav");
	loopBGMs_[6]->SoundLoadWave("./Resources/loopBGM/tempo_07.wav");
	loopBGMs_[7]->SoundLoadWave("./Resources/loopBGM/tempo_08.wav");
	loopBGMs_[8]->SoundLoadWave("./Resources/loopBGM/tempo_09.wav");
	//	音の再生
	Notes::StaticInitialize();

	//	音量の設定
	
	//	天球の親設定
	skydome_->SetParent(battleAnimation_->GetHeroTransform());

}

Battle::~Battle() {



}

void Battle::Initialize() {

	EndPos = { 0.0f,2.0f,0.0f };

	player_->Initialize(makeCatmull_->GetFirstControlPoint());

	tutorialMusicScoreList_.clear();

	for (int32_t i = 0; i < 2; i++) {
		std::unique_ptr<MusicScore> tmpScore = std::make_unique<MusicScore>();
		tmpScore->ModelLoad(notesModels_, noteTextures_);
		tmpScore->SetPlayer(player_.get());
		tmpScore->SetBoss(battleAnimation_->GetBoss());
		tmpScore->SetHero(battleAnimation_->GetHero());
		tmpScore->SetBPM(BPM_);
		if (i < 1) {
			tmpScore->SetNotes(MusicScore::Rest, makeCatmull_->GetControlPoints(), i);
		}
		else {
			tmpScore->SetNotes(MusicScore::TUTORIAL_01, makeCatmull_->GetControlPoints(), i);
		}
		tmpScore->Initialize();
		tutorialMusicScoreList_.push_back(std::move(tmpScore));
	}

	musicScoreList_.clear();

	for (int32_t i = 0; i < 4; i++) {
		std::unique_ptr<MusicScore> tmpScore = std::make_unique<MusicScore>();
		tmpScore->ModelLoad(notesModels_, noteTextures_);
		tmpScore->SetPlayer(player_.get());
		tmpScore->SetBoss(battleAnimation_->GetBoss());
		tmpScore->SetHero(battleAnimation_->GetHero());
		tmpScore->SetBPM(BPM_);
		if (i <= 1) {
			tmpScore->SetNotes(MusicScore::Rest, makeCatmull_->GetControlPoints(), i);
		}
		else {
			tmpScore->SetNotes(MusicScore::ScoreType(rand() % 5), makeCatmull_->GetControlPoints(), i);
		}
		tmpScore->Initialize();
		musicScoreList_.push_back(std::move(tmpScore));
	}

	//battleAnimation_->Initialize();
	battleAnimation_->Initialize();

	worldTransformLine_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransformLine_.scale_ *= 5.0f;
	worldTransformLine_.translation_.y = 10.0f;

	countMeasure_ = maxCountMeasure_;

	loopCount_ = 8;
	frameDifference_ = 0.0f;
	isUpdateScore_ = false;
	frameCounter_ = 0;
	endTutorial_ = false;
	isStop_ = false;
	tutorialNotesCount_ = 0;
	//フラグを降ろす
	MusicScore::isUpdateFlag_ = false;
	//現在の進行度リセット
	currentStage_ = 0;
	//デバッグ用の変数リセット
	testScoreCount_ = 0;
	goalNotesCount_ = 25;
	currentNotesCount_ = 0;
	//遷移フラグリセット
	isGameClear_ = false;
	isGameOver_ = false;
	//	最初の一回
	boxTrans_.scale_ = Vector3(80.0f, 45.0f, 1.0f);
	boxTrans_.cMono->pibot.x = 470.0f;
	boxTrans_.cMono->pibot.y = 360.0f;
	boxTrans_.cMono->rate = 140.0f;
	boxColor_ = 0x000000bb;

	easeFrame_ = 0.0f;
	titleTrans_.translation_ = Vector3(0.0f, 0.0f, 0.0f);
	titleTrans_.scale_ = Vector3(1.5f, 1.5f, 0.0f);
	titleStartPos_ = titleTrans_.translation_;
	titleEndPos_ = Vector3(0.0f, 500.0f, 0.0f);
	startFlag_ = false;
	titleFlag_ = false;

	for (int i = 0; i < 6; i++) {
		
		stageTrans_[i].translation_ = { -250.0f + i * 100.0f,-200.0f,0.0f };
	}

	gaugeTrans_.translation_ = { 0.0f,-300.0f,0.0f };
	gaugeFrameTrans_.translation_ = { 0.0f,-300.0f,0.0f };
	gaugeFrameTrans_.scale_.x = 5.0f;

	//ボタンテクスチャ関連の初期化
	changeCount_ = 0;
	BButtomTrans_.scale_ = { 1.5f,1.5f,0.0f };
	BButtomTrans_.translation_ = { 0.0f,-165.0f,0.0f };

	MultipathRendering::GetInstance()->cEffectParameters->centerPosition = Vector2(0.0f, 0.0f);
	MultipathRendering::GetInstance()->cEffectParameters->parameterRate = 0.0f;
	MultipathRendering::GetInstance()->cEffectParameters->type = 0;

	//	カメラの初期化
	if (true) {
		camera_->transform.translation_ = Vector3(36.0f, 14.0f, -11.0f);
		camera_->transform.rotation_ = Vector3(AngleToRadian(12.0f), AngleToRadian(-54.0f), 0.0f);

		cameraMoveFlag = false;
		cameraT_ = 0.0f;
		cameraTspeed_ = 0.0f;
		movepattern_ = MovePattern::Run;

		quadFrame_ = 15;
		quadFlag_ = 0;
		quadMoveFlag = false;

		isBoxFlag = false;
	}



}

void Battle::Update() {

#ifdef _DEBUG

	ImGui::Begin("ingame");
	ImGui::Text("tutorialNotesCount : %d", tutorialNotesCount_);
	ImGui::Text("currentStage : %d", currentStage_);
	ImGui::Text("currentNotesCount : %d", currentNotesCount_);
	ImGui::Text("goalNotesCount : %d", goalNotesCount_);
	ImGui::Text("clear : %d", isGameClear_);
	ImGui::Text("gameover : %d", isGameOver_);
	ImGui::End();

	ImGui::Begin("box");
	ImGui::DragFloat2("translate", &boxTrans_.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &boxTrans_.rotation_.x, AngleToRadian(1.0f));
	ImGui::DragFloat3("scale", &boxTrans_.scale_.x, 0.1f);
	ImGui::DragFloat2("pibot", &boxTrans_.cMono->pibot.x, 1.0f);
	ImGui::DragFloat("rate", &boxTrans_.cMono->rate, 1.0f);

	ImGui::DragFloat3("titlescale", &titleTrans_.scale_.x, 0.1f);

	ImGui::DragFloat3("buttomTranslate", &BButtomTrans_.translation_.x, 1.0f);
	ImGui::End();
#endif // _DEBUG
	//	タイトル遷移用のフラグ
	if ((KeyInput::PushKey(DIK_SPACE) || KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B)) && !startFlag_) {
		titleFlag_ = true;
		startFlag_ = true;
		//	カメラのセット
		cameraMoveFlag = true;
		movepattern_ = MovePattern::Run;
		SetCameraMove();
		cameraTspeed_ = 0.02f;
		loopBGMs_[2]->SoundStop();
		selectSE_->SoundStop();
		selectSE_->SoundPlayWave();
		selectSE_->SetVolume(0.3f);
		//	エフェクトの設定
		MultipathRendering::GetInstance()->cEffectParameters->parameterRate = 360.0f;
		MultipathRendering::GetInstance()->cEffectParameters->type = 5;
	}
	//	タイトルの遷移
	if (titleFlag_) {
		easeFrame_++;
		titleTrans_.translation_ = Ease::UseEase(titleStartPos_, titleEndPos_, easeFrame_, 120.0f, Ease::EaseInOut);
		titleTrans_.UpdateMatrix();
		MultipathRendering::GetInstance()->cEffectParameters->centerPosition.x = 640.0f + titleTrans_.translation_.x;
		MultipathRendering::GetInstance()->cEffectParameters->centerPosition.y = 360.0f - titleTrans_.translation_.y;
		if (easeFrame_ >= 120.0f) {
			easeFrame_ = 0.0f;
			titleFlag_ = false;
			player_->SetIsMove(true);
			MultipathRendering::GetInstance()->cEffectParameters->parameterRate = 0.0f;
		}
	}

	if (!startFlag_) {
		loopBGMs_[2]->SoundPlayWave(true);
		loopBGMs_[2]->SetVolume(0.3f);
	}
	
	//フラグを降ろす
	MusicScore::isUpdateFlag_ = false;

	notesEffects_.remove_if([](auto& effect) {

		if (effect->GetIsDead()) {
			return true;
		}

		return false;

	});

	if (isStop_) {
		MusicScore::isStopAll = true;
	}
	else if(tutorialNotesCount_ < 10) {
		MusicScore::isStopAll = false;
	}

	//チュートリアルノーツ十回叩いて本ゲーム開始
	if (!endTutorial_ && tutorialNotesCount_ >= 10 && tutorialMusicScoreList_.empty()) {
		endTutorial_ = true;
		countMeasure_ = maxCountMeasure_;
		frameDifference_ = 0.0f;
		isUpdateScore_ = false;
		//チュートリアル音源停止
		loopBGMs_[0]->SoundStop();
	}

	if (endTutorial_ && countMeasure_ <= 0) {
		loopCount_++;
		isUpdateScore_ = true;
		player_->SetLinePass(0);
		player_->SetDivisionNumber(float(maxCountMeasure_ / float(makeCatmull_->GetControlPoints().size() - 1) / 10.0f));
		
		countMeasure_ = maxCountMeasure_;
		
		//目標ノーツ数を達成している場合、演出に向けての準備
		if (loopCount_ == 7) {
			if (currentNotesCount_ >= goalNotesCount_) {
				for (size_t i = 0; const auto & score : musicScoreList_) {

					//次に流れるノーツをリセット
					if (i == 2) {
						score->ClearNotes();
					}

					i++;

				}
			}
		}

		//音楽ループ継続
		if (loopCount_ >= 8) {

			//次の目標をセット。届いてなかったら現在のノーツ獲得数に応じて処理を変化
			SetNextGoalNotes();

			for (int i = 0; i < 9; i++) {
				loopBGMs_[i]->SoundStop();

				if (currentStage_ > i - 4) {
					loopBGMs_[i]->SoundPlayWave();
					loopBGMs_[i]->SetVolume(0.3f);
				}

			}
			loopCount_ = 0;
			frameCounter_ = 0;
		}

	}
	else if (!endTutorial_ && countMeasure_ <= 0) {

		isUpdateScore_ = true;
		countMeasure_ = maxCountMeasure_;

		//チュートリアル音源再生
		loopBGMs_[0]->SoundStop();
		loopBGMs_[0]->SoundPlayWave();
		loopBGMs_[0]->SetVolume(0.2f);

	}

	UpdateObjects();

	if (isUpdateScore_) {
		UpdateScores();
		isUpdateScore_ = false;
	}

	if (player_->GetIsMove() && endTutorial_) {

		//現在フレームが60fpsでない場合、フレームの差分を貯める
		frameDifference_ += float(FrameCount::currentFrameTime - (1.0f / 60.0f));

		countMeasure_--;
		frameCounter_++;

		//フレームの差分が1フレーム分超えたら更新処理を挟む
		if (frameDifference_ >= float(1.0f / 60.0f) && countMeasure_ != 0) {

			UpdateObjects();
			countMeasure_--;
			frameCounter_++;
			//1フレーム分の時間削る
			frameDifference_ -= float(1.0f / 60.0f);

		}

	}
	else if (player_->GetIsMove() && !endTutorial_ && !isStop_) {

		//現在フレームが60fpsでない場合、フレームの差分を貯める
		frameDifference_ += float(FrameCount::currentFrameTime - (1.0f / 60.0f));

		countMeasure_--;

		//フレームの差分が1フレーム分超えたら更新処理を挟む
		if (frameDifference_ >= float(1.0f / 60.0f) && countMeasure_ != 0) {
			UpdateObjects();
			countMeasure_--;
			//1フレーム分の時間削る
			frameDifference_ -= float(1.0f / 60.0f);

		}

	}

	//	カメラ移動フラグが立ったら、移動
	if (cameraMoveFlag) {
		cameraT_ += cameraTspeed_;
		//	移動が終わったら
		if (!camera_->CameraWork(cameraMoveStart, cameraMoveEnd, cameraRotateStart, cameraRotateEnd, cameraT_)) {
			cameraT_ = 0.0f;
			cameraMoveFlag = false;
			if (isBoxFlag) {
				isBoxFlag = false;
				cameraMoveFlag = true;
				cameraTspeed_ = 0.01f;
				camera_->transform.translation_ = Vector3(2.0f, 6.0f, 23.0f);
				camera_->transform.rotation_ = Vector3(AngleToRadian(8.0f), 0.0f, 0.0f);
				movepattern_ = MovePattern::Zoom;
				SetCameraMove();
			}
		}
	}
	else if (quadMoveFlag) {
		QuadRangle();
	}

	//	クリアフラグ
	if (isGameClear_) {
		GameScene::GetInstance()->sceneChangeFlag = true;
		MultipathRendering::GetInstance()->Initialize();
		for (int i = 0; i < 9; i++) {
			loopBGMs_[i]->SoundStop();
		}

	}else if (isGameOver_) {
		GameScene::GetInstance()->sceneChangeFlag = true;
		MultipathRendering::GetInstance()->Initialize();
		for (int i = 0; i < 9; i++) {
			loopBGMs_[i]->SoundStop();
		}
	}

}

void Battle::UpdateObjects() {

	if (endTutorial_) {
		for (const auto& score : musicScoreList_) {
			score->Update(makeCatmull_->GetControlPoints());

			for (Notes* note : score->GetNotes()) {

				if (!isGameClear_ && !isGameOver_) {
					//叩いたらノーツカウント+
					if (note->GetIsHit()) {
						currentNotesCount_++;

						std::unique_ptr<NotesEffect> newEffect = std::make_unique<NotesEffect>();
						newEffect->ModelLoad(notesEffectModel_);
						newEffect->Initialize();
						newEffect->SetPosition(battleAnimation_->GetHero()->GetTransform().translation_);
						newEffect->SetColor(note->GetColor());
						notesEffects_.push_back(std::move(newEffect));

					}

					//ミスした場合、カウントを多く下げる
					if (note->GetIsMiss()) {

						std::unique_ptr<NotesEffect> newEffect = std::make_unique<NotesEffect>();
						newEffect->ModelLoad(notesEffectModel_);
						newEffect->Initialize();
						newEffect->SetPosition(battleAnimation_->GetHero()->GetTransform().translation_);
						newEffect->SetColor(0x000000ff);
						newEffect->SetIsMiss(true);
						notesEffects_.push_back(std::move(newEffect));

						//初期段階のみ緩く設定
						if (currentStage_ == 0) {
							currentNotesCount_ -= 1;
						}
						else {
							currentNotesCount_ -= 3;
						}

					}

				}
				
			}

		}
	}
	else {
		for (const auto& score : tutorialMusicScoreList_) {
			score->Update(makeCatmull_->GetControlPoints());

			if (tutorialNotesCount_ <= 2) {

				for (Notes* note : score->GetNotes()) {

					if (note->GetJudgeFrame() <= 1.0f) {
						isStop_ = true;
					}

				}

			}

			for (Notes* note : score->GetNotes()) {

				if (note->GetIsHit()) {
					isStop_ = false;
					tutorialNotesCount_++;
					loopBGMs_[0]->SoundPlayWave();
					loopBGMs_[0]->SetVolume(0.2f);
					std::unique_ptr<NotesEffect> newEffect = std::make_unique<NotesEffect>();
					newEffect->ModelLoad(notesEffectModel_);
					newEffect->Initialize();
					newEffect->SetPosition(battleAnimation_->GetHero()->GetTransform().translation_);
					newEffect->SetColor(note->GetColor());
					notesEffects_.push_back(std::move(newEffect));

				}

			}

		}
	}

	player_->Update(makeCatmull_->GetControlPoints(), makeCatmull_->GetLastLinePass());

	for (auto& effect : notesEffects_) {

		if (battleAnimation_->isLoop_) {
			effect->SubPosition({ 0.0f,0.0f,140.0f * 2.0f });
		}
		effect->Update();
	}

	battleAnimation_->isLoop_ = false;

	ControlPoints_ = makeCatmull_->GetControlPoints();

	if (lines_.size() < makeCatmull_->GetControlPoints().size()) {
		lines_.push_back(std::make_unique<Line>());
	}
	else if (lines_.size() > makeCatmull_->GetControlPoints().size()) {
		lines_.pop_back();
	}

	//ストップしていたら更新しない
	if (!isStop_) {
		battleAnimation_->Update();
	}
	
	for (size_t i = 0; i < 6; i++) {

		if (stageTrans_[i].scale_.x > 1.0f) {
			stageTrans_[i].scale_.x -= 0.05f;
		}

		if (stageTrans_[i].scale_.y > 1.0f) {
			stageTrans_[i].scale_.y -= 0.05f;
		}

		if (stageTrans_[i].scale_.x < 1.0f) {
			stageTrans_[i].scale_.x = 1.0f;
		}

		if (stageTrans_[i].scale_.y < 1.0f) {
			stageTrans_[i].scale_.y = 1.0f;
		}

		stageTrans_[i].UpdateMatrix();
	}
	
	//ゲージ処理

	if (endTutorial_) {

		if (currentNotesCount_ < 0) {

			float count = float(-currentNotesCount_);
			float goal = float(goalNotesCount_ / 2);

			if (goalNotesCount_ != 0) {
				gaugeTrans_.scale_.x = count / goal * 5.0f;
			}
			else {
				gaugeTrans_.scale_.x = 0.0f;
			}

			if (currentNotesCount_ < int(-goalNotesCount_ / 2)) {
				gaugeTrans_.scale_.x = 5.0f;
			}

		}
		else if (currentNotesCount_ <= goalNotesCount_) {

			if (currentNotesCount_ > 0) {

				float count = float(currentNotesCount_);
				float goal = float(goalNotesCount_);

				if (goalNotesCount_ != 0) {
					gaugeTrans_.scale_.x = count / goal * 5.0f;
				}
				else {
					gaugeTrans_.scale_.x = 0.0f;
				}

			}
			else {
				gaugeTrans_.scale_.x = 0.0f;
			}

		}
		else {
			gaugeTrans_.scale_.x = 5.0f;
		}

	}
	//チュートリアル時の処理
	else {

		if (tutorialNotesCount_ <= 10) {

			if (tutorialNotesCount_ > 0) {

				float count = float(tutorialNotesCount_);

				if (goalNotesCount_ != 0) {
					gaugeTrans_.scale_.x = count / 10.0f * 5.0f;
				}
				else {
					gaugeTrans_.scale_.x = 0.0f;
				}

			}
			else {
				gaugeTrans_.scale_.x = 0.0f;
			}

		}
		else {
			gaugeTrans_.scale_.x = 5.0f;
		}

	}

	

	gaugeTrans_.UpdateMatrix();
	gaugeFrameTrans_.UpdateMatrix();
	worldTransformLine_.UpdateMatrix();

}

void Battle::Draw3D(const Matrix4x4& viewProjection) {

	skydome_->Draw(viewProjection);

	//player_->Draw(viewProjection);

	if (endTutorial_) {
		for (const auto& score : musicScoreList_) {
			score->Draw(viewProjection);
		}
	}
	else {
		for (const auto& score : tutorialMusicScoreList_) {
			score->Draw(viewProjection);
		}
	}

	for (auto& effect : notesEffects_) {
		effect->Draw(viewProjection);
	}

	//makeCatmull_->Draw(viewProjection);

	//for (size_t i = 0; i < lines_.size(); i++){
	//	lines_[i]->DrawLine(ControlPoints_[i] - EndPos, ControlPoints_[i] + EndPos, viewProjection, 0xff0000ff);
	//}

	battleAnimation_->Draw3D(viewProjection);
}

void Battle::Draw2D(const Matrix4x4& viewProjection) {

	/*currentMusicScore_->Draw2D(viewProjection);*/
	if (isStop_ || isBoxFlag) {
		Texture2D::TextureDraw(boxTrans_, viewProjection, boxColor_, boxtexture_);

		if (changeCount_<30){
			Texture2D::TextureDraw(BButtomTrans_, viewProjection, 0xffffffff, BButtomTexture_);
		}
		else {
			Texture2D::TextureDraw(BButtomTrans_, viewProjection, 0xffffffff, pushBButtomTexture_);
			if (changeCount_>60){
				changeCount_ = 0;
			}
		}
		changeCount_++;
	}
	// ゲームが始まっていなければタイトルの表示
	if (!player_->GetIsMove()) {
		Texture2D::TextureDraw(titleTrans_, viewProjection, 0xffffffff, titleTexture_);
		if (!titleFlag_) {
			if (changeCount_ < 30) {
				Texture2D::TextureDraw(BButtomTrans_, viewProjection, 0xffffffff, BButtomTexture_);
			}
			else {
				Texture2D::TextureDraw(BButtomTrans_, viewProjection, 0xffffffff, pushBButtomTexture_);
				if (changeCount_ > 60) {
					changeCount_ = 0;
				}
			}
			changeCount_++;
		}
	}
	//チュートリアル中のUI
	else if (!endTutorial_) {

		Texture2D::TextureDraw(gaugeFrameTrans_, viewProjection, 0xffffffff, UITextures_[6]);

		if (tutorialNotesCount_ >= 0) {

			if (tutorialNotesCount_ >= 10) {
				Texture2D::TextureDraw(gaugeTrans_, viewProjection, 0xffffffff, UITextures_[3]);
			}
			else {
				Texture2D::TextureDraw(gaugeTrans_, viewProjection, 0xffffffff, UITextures_[2]);
			}

		}

	}
	//ゲームが始まったらUI表示
	else if (endTutorial_) {

		for (int i = 0; i < 6; i++) {

			if (currentStage_ - 1 >= i) {
				Texture2D::TextureDraw(stageTrans_[i], viewProjection, 0xffffffff, UITextures_[0]);
			}
			else {
				Texture2D::TextureDraw(stageTrans_[i], viewProjection, 0xffffffff, UITextures_[1]);
			}
			
		}

		Texture2D::TextureDraw(gaugeFrameTrans_, viewProjection, 0xffffffff, UITextures_[6]);

		if (currentNotesCount_ >= 0) {

			if (currentNotesCount_ >= goalNotesCount_) {
				Texture2D::TextureDraw(gaugeTrans_, viewProjection, 0xffffffff, UITextures_[3]);
			}
			else {
				Texture2D::TextureDraw(gaugeTrans_, viewProjection, 0xffffffff, UITextures_[2]);
			}

		}
		else {

			if (currentNotesCount_ < int(-goalNotesCount_ / 2)) {
				Texture2D::TextureDraw(gaugeTrans_, viewProjection, 0xffffffff, UITextures_[5]);
			}
			else {
				Texture2D::TextureDraw(gaugeTrans_, viewProjection, 0xffffffff, UITextures_[4]);
			}

		}

	}

	if (!startFlag_) {
		
		if (changeCount_ < 30) {
			Texture2D::TextureDraw(BButtomTrans_, viewProjection, 0xffffffff, BButtomTexture_);
		}
		else {
			Texture2D::TextureDraw(BButtomTrans_, viewProjection, 0xffffffff, pushBButtomTexture_);
			if (changeCount_ > 60) {
				changeCount_ = 0;
			}
		}
		changeCount_++;

	}

}

void Battle::ModelLoad(std::vector<Model*> models, std::vector<Texture2D*> textures) {

	notesModels_ = models;
	noteTextures_ = textures;
	/*currentMusicScore_->ModelLoad(notesModels_, noteTextures_);*/
}

void Battle::SetNextGoalNotes() {

	if (currentStage_ < 5) {
		
		//ノルマを達成したら次のレベルに移行
		if (currentNotesCount_ >= goalNotesCount_ && goalNotesCount_ != 0) {

			//
			//レベルアップの処理ここ
			//

			//現在のノーツカウントリセット
			currentNotesCount_ = 0;

			//それぞれのレベルの目標を設定
			switch (currentStage_)
			{
			default:
			case 0:
				goalNotesCount_ = 25;
				quadMoveFlag = true;
				//	エフェクトの設定
				MultipathRendering::GetInstance()->cEffectParameters->centerPosition = Vector2(1400.0f, 0.0f);
				MultipathRendering::GetInstance()->cEffectParameters->parameterRate = 900.0f;
				MultipathRendering::GetInstance()->cEffectParameters->type = 2;
				break;
			case 1:
				goalNotesCount_ = 35;
				//	カメラのセット
				quadMoveFlag = false;
				cameraMoveFlag = true;
				movepattern_ = MovePattern::LeftSide;
				SetCameraMove();
				cameraTspeed_ = 0.02f;

				//	エフェクトの設定
				MultipathRendering::GetInstance()->cEffectParameters->centerPosition = Vector2(640.0f, 360.0f);
				MultipathRendering::GetInstance()->cEffectParameters->parameterRate = 900.0f;
				MultipathRendering::GetInstance()->cEffectParameters->type = 6;
				break;
			case 2:
				goalNotesCount_ = 50;

				boxTrans_.cMono->pibot.x = 0.0f;
				boxTrans_.cMono->pibot.y = 0.0f;
				boxTrans_.cMono->rate = 0.0f;
				boxColor_ = 0x000000ff;
				isBoxFlag = true;
				cameraMoveFlag = true;
				//	カメラのセット
				
				cameraTspeed_ = 0.1f;

				//	エフェクトの設定
				MultipathRendering::GetInstance()->cEffectParameters->centerPosition = Vector2(640.0f, 360.0f);
				MultipathRendering::GetInstance()->cEffectParameters->parameterRate = 900.0f;
				MultipathRendering::GetInstance()->cEffectParameters->type = 5;
				
				break;
			case 3: 
				goalNotesCount_ = 65;
				//	エフェクトの設定
				MultipathRendering::GetInstance()->cEffectParameters->centerPosition = Vector2(640.0f, 360.0f);
				MultipathRendering::GetInstance()->cEffectParameters->parameterRate = 900.0f;
				MultipathRendering::GetInstance()->cEffectParameters->type = 9;
				break;
			case 4:
				goalNotesCount_ = 80;
				//	カメラのセット
				cameraMoveFlag = true;
				movepattern_ = MovePattern::Stop;
				SetCameraMove();
				cameraTspeed_ = 0.03f;

				//	エフェクトの設定
				MultipathRendering::GetInstance()->cEffectParameters->centerPosition = Vector2(640.0f, 360.0f);
				MultipathRendering::GetInstance()->cEffectParameters->parameterRate = 900.0f;
				MultipathRendering::GetInstance()->cEffectParameters->type = 7;
				break;

			}

			//レベルアップ
			currentStage_++;

			for (size_t i = 0; i < 6; i++) {

				if (currentStage_ - 1 >= i) {
					stageTrans_[i].scale_ = { 1.5f,1.5f,1.0f };
				}

			}

		}
		//ゴールカウントが正常にリセットされていなかったとき用
		else if (goalNotesCount_ == 0) {

			//現在のノーツカウントリセット
			currentNotesCount_ = 0;

			//それぞれのレベルの目標を設定
			switch (currentStage_)
			{
			default:
			case 0:
				goalNotesCount_ = 25;
				break;
			case 1:
				goalNotesCount_ = 35;
				break;
			case 2:
				goalNotesCount_ = 50;
				break;
			case 3:
				goalNotesCount_ = 65;
				break;
			case 4:
				goalNotesCount_ = 80;
				break;

			}

		}
		//ノーツカウントが0以下で更に一定値を下回ったらゲーム終了
		else if (currentNotesCount_ < int(-goalNotesCount_ / 2)) {
				isGameOver_ = true;
		}
		//現在のノーツが0以下ならステージを下げる
		else if (currentNotesCount_ <= 0 && currentStage_ != 0) {

			currentStage_--;

			//現在のノーツカウントリセット
			currentNotesCount_ = 0;

			//それぞれのレベルの目標を設定
			switch (currentStage_)
			{
			default:
			case 0:
				goalNotesCount_ = 25;
				break;
			case 1:
				goalNotesCount_ = 35;
				break;
			case 2:
				goalNotesCount_ = 50;
				break;
			case 3:
				goalNotesCount_ = 65;
				break;
			case 4:
				goalNotesCount_ = 80;
				break;

			}

		}
		//それ以外は現在の状態を継続

		//ステージの状態に応じてノーツの音域を変化
		if (currentStage_ >= 2) {
			Notes::isUpSE = true;
		}
		else {
			Notes::isUpSE = false;
		}

	}
	//最終ステージの場合
	else if(currentStage_ >= 5) {

		//ノルマ達成でクリア
		if (currentNotesCount_ >= goalNotesCount_) {

			isGameClear_ = true;

			//レベルアップ
			currentStage_++;

			for (size_t i = 0; i < 6; i++) {

				if (currentStage_ - 1 >= i) {
					stageTrans_[i].scale_ = { 1.5f,1.5f,1.0f };
				}

			}


		}
		//ノーツカウントが0以下で更に一定値を下回ったらゲーム終了
		else if (currentNotesCount_ < int(-goalNotesCount_ / 2)) {
			isGameOver_ = true;
		}
		//現在のノーツが0以下ならステージを下げる
		else if (currentNotesCount_ <= 0 && currentStage_ != 0) {

			currentStage_--;

			//現在のノーツカウントリセット
			currentNotesCount_ = 0;

			goalNotesCount_ = 65;

		}

	}

}

void Battle::QuadRangle() {
	quadFrame_++;
	if (quadFrame_ >= 30.0f) {
		quadFrame_ = 0;
		quadFlag_ ++;
		if (quadFlag_ >= 4) {
			quadFlag_ = 0;
		}
	}

	if (quadFlag_ == 0) {
		camera_->transform.translation_.x += 0.01f;
	}
	else if (quadFlag_ == 1) {
		camera_->transform.translation_.y -= 0.01f;
	}
	else if (quadFlag_ == 2) {
		camera_->transform.translation_.x -= 0.01f;
	}
	else if (quadFlag_ == 3) {
		camera_->transform.translation_.y += 0.01f;
	}

}

void Battle::SetCameraMove() {
	cameraMoveStart = camera_->transform.translation_;
	cameraRotateStart = camera_->transform.rotation_;
	switch (movepattern_) {
	case MovePattern::Run: // 通常
		cameraMoveEnd = Vector3(15.0f, 6.0f, -18.0f);
		cameraRotateEnd = Vector3(0.105f, -0.472f, 0.0f);
		break;
	case MovePattern::LeftSide: // 左側視点
		cameraMoveEnd = Vector3(-16.0f, 8.0f, -10.0f);
		cameraRotateEnd = Vector3(AngleToRadian(12.0f), AngleToRadian(39.0f), 0.0f);
		break;
	case MovePattern::Zoom: // zoom視点?
		cameraMoveEnd = Vector3(7.0f, 7.0f, -10.0f);
		cameraRotateEnd = Vector3(AngleToRadian(8.0f), AngleToRadian(-18.0f), 0.0f);
		break;
	case MovePattern::Stop: // playerの前側視点
		cameraMoveEnd = Vector3(-27.0f, 19.0f, 35.0f);
		cameraRotateEnd = Vector3(AngleToRadian(26.0f), AngleToRadian(124.0f), 0.0f);
		break;
	}
}

void Battle::UpdateScores() {

	//空でない場合処理
	if (!musicScoreList_.empty() && endTutorial_) {

		/*if (++testScoreCount_ >= 20) {
			testScoreCount_ = 0;
		}*/

		//最前列を後列に追加
		musicScoreList_.push_back(std::move(musicScoreList_.front()));
		musicScoreList_.pop_front();

		if (currentStage_ < 2) {
			musicScoreList_.back().get()->SetNotes(MusicScore::ScoreType(rand() % ((currentStage_ + 1) * 5)), makeCatmull_->GetControlPoints(), 3);
		}
		else if(currentStage_ == 2){
			musicScoreList_.back().get()->SetNotes(MusicScore::ScoreType(rand() % 15), makeCatmull_->GetControlPoints(), 3);
		}
		else if(currentStage_ < 5){

			// 3/4の確率で難しいノーツの配置に移行
			int num = rand() % 4;

			if (num == 0) {
				musicScoreList_.back().get()->SetNotes(MusicScore::ScoreType(rand() % 5 + 5), makeCatmull_->GetControlPoints(), 3);
			}
			else {
				musicScoreList_.back().get()->SetNotes(MusicScore::ScoreType(rand() % 10 + ((currentStage_ - 2) * 5)), makeCatmull_->GetControlPoints(), 3);
			}

		}
		else {

			// 4/5の確率で難しいノーツの配置に移行
			int num = rand() % 5;

			if (num == 0) {
				musicScoreList_.back().get()->SetNotes(MusicScore::ScoreType(rand() % 5 + 5), makeCatmull_->GetControlPoints(), 3);
			}
			else {
				musicScoreList_.back().get()->SetNotes(MusicScore::ScoreType(rand() % 10 + 10), makeCatmull_->GetControlPoints(), 3);
			}

		}
		
	}
	else if (!tutorialMusicScoreList_.empty() && !endTutorial_) {

		if (tutorialNotesCount_ < 10) {
			//最前列を後列に追加
			tutorialMusicScoreList_.push_back(std::move(tutorialMusicScoreList_.front()));
			tutorialMusicScoreList_.pop_front();
			//三回叩いたらノーツパターン変更
			if (tutorialNotesCount_ < 3) {
				tutorialMusicScoreList_.back().get()->SetNotes(MusicScore::TUTORIAL_01, makeCatmull_->GetControlPoints(), 1);
			}
			//十回叩いたらチュートリアル終了
			else {
				tutorialMusicScoreList_.back().get()->SetNotes(MusicScore::TUTORIAL_02, makeCatmull_->GetControlPoints(), 1);
			}

		}
		else {
			tutorialMusicScoreList_.pop_front();
		}
		
	}

}

void Battle::SetUI(std::vector<Texture2D*> textures) {

	UITextures_ = textures;

}
