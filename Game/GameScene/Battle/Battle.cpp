#include "Battle.h"
#include "math/Rand.h"
#include "FrameCount.h"

Battle::Battle(std::shared_ptr<Camera> camera)
{

	player_ = std::make_unique<Player>();
	player_->ModelLoad();

	skydome_ = std::make_unique<Skydome>();
	skydome_->ModelLoad();

	makeCatmull_ = std::make_unique<MakeCatmull>();

	makeCatmull_->Initialize();

	battleAnimation_ = std::make_unique<BattleAnimation>(camera);
	
	for (int i = 0; i < 5; i++) {
		loopBGMs_[i] = std::make_unique<AudioInput>();
	}

	//	ロード
	loopBGMs_[0]->SoundLoadWave("./Resources/loopBGM/tempo_01.wav");
	loopBGMs_[1]->SoundLoadWave("./Resources/loopBGM/tempo_02.wav");
	loopBGMs_[2]->SoundLoadWave("./Resources/loopBGM/tempo_03.wav");
	loopBGMs_[3]->SoundLoadWave("./Resources/loopBGM/tempo_04.wav");
	loopBGMs_[4]->SoundLoadWave("./Resources/loopBGM/tempo_05.wav");
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
			tmpScore->SetNotes(MusicScore::Easy_04, makeCatmull_->GetControlPoints(), i);
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

}

void Battle::Update() {

#ifdef _DEBUG

	ImGui::Begin("testNotes");
	ImGui::Text("tutorialNotesCount : %d", tutorialNotesCount_);
	ImGui::End();

#endif // _DEBUG


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
			for (int i = 0; i < 5; i++) {
				loopBGMs_[i]->SoundStop();

				if (Notes::Combo >= i * 10) {
					loopBGMs_[i]->SoundPlayWave();
					loopBGMs_[i]->SetVolume(0.2f);
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

}

void Battle::ModelLoad(std::vector<Model*> models, std::vector<Texture2D*> textures) {

	notesModels_ = models;
	noteTextures_ = textures;
	/*currentMusicScore_->ModelLoad(notesModels_, noteTextures_);*/
}

void Battle::UpdateScores() {

	//空でない場合処理
	if (!musicScoreList_.empty() && endTutorial_) {
		//最前列を後列に追加
		musicScoreList_.push_back(std::move(musicScoreList_.front()));
		musicScoreList_.pop_front();
		musicScoreList_.back().get()->SetNotes(MusicScore::ScoreType(rand() % 10), makeCatmull_->GetControlPoints(), 3);
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
