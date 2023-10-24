#include "Battle.h"
#include "math/Rand.h"
#include "FrameCount.h"
#include "Engine/Easing/Ease.h"
#include "Engine/Input/KeyInput/KeyInput.h"

Battle::Battle(std::shared_ptr<Camera> camera)
{

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
	boxTrans_.cMono->pibot.x = 300.0f;
	boxTrans_.cMono->pibot.y = 360.0f;
	boxTrans_.cMono->rate = 300.0f;
	boxColor_ = 0x00000099;

	easeFrame_ = 0.0f;
	titleStartPos_ = titleTrans_.translation_;
	titleEndPos_ = Vector3(0.0f, 400.0f, 0.0f);

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
	ImGui::End();
#endif // _DEBUG
	if (KeyInput::PushKey(DIK_Z)) {
		startFlag_ = true;
	}

	if (startFlag_) {
		easeFrame_++;
		titleTrans_.translation_ = Ease::UseEase(titleStartPos_, titleEndPos_, easeFrame_, 180.0f, Ease::EaseInBounce);
		titleTrans_.UpdateMatrix();
		if (easeFrame_ >= 180.0f) {
			easeFrame_ = 0.0f;
			startFlag_ = false;
		}
	}

	//フラグを降ろす
	MusicScore::isUpdateFlag_ = false;

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

	else {
		for (int i = 0; i < 5; i++) {
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
					}

					//ミスした場合、カウントを多く下げる
					if (note->GetIsMiss()) {

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

			if (tutorialNotesCount_ == 0) {

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
				}

			}

		}
	}

	player_->Update(makeCatmull_->GetControlPoints(), makeCatmull_->GetLastLinePass());

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

	

	//makeCatmull_->Draw(viewProjection);

	//for (size_t i = 0; i < lines_.size(); i++){
	//	lines_[i]->DrawLine(ControlPoints_[i] - EndPos, ControlPoints_[i] + EndPos, viewProjection, 0xff0000ff);
	//}

	battleAnimation_->Draw3D(viewProjection);
}

void Battle::Draw2D(const Matrix4x4& viewProjection) {

	/*currentMusicScore_->Draw2D(viewProjection);*/
	if (isStop_) {
		Texture2D::TextureDraw(boxTrans_, viewProjection, boxColor_, boxtexture_);
	}
	Texture2D::TextureDraw(titleTrans_, viewProjection, 0xffffffff, titleTexture_);

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

			//レベルアップ
			currentStage_++;

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

	}
	//最終ステージの場合
	else if(currentStage_ >= 5) {

		//ノルマ達成でクリア
		if (currentNotesCount_ >= goalNotesCount_) {

			isGameClear_ = true;

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
		else {

			// 3/4の確率で難しいノーツの配置に移行
			int num = rand() % 4;

			if (num == 0) {
				musicScoreList_.back().get()->SetNotes(MusicScore::ScoreType(rand() % 5 + 5), makeCatmull_->GetControlPoints(), 3);
			}
			else {
				musicScoreList_.back().get()->SetNotes(MusicScore::ScoreType(rand() % 10 + ((currentStage_ - 2) * 5)), makeCatmull_->GetControlPoints(), 3);
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
