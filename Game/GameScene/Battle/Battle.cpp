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
	

	drumLoop_ = std::make_unique<AudioInput>();

	for (int i = 0; i < 5; i++) {
		loopBGMs_[i] = std::make_unique<AudioInput>();
	}

	//	ロード
	drumLoop_->SoundLoadWave("./Resources/loopBGM/drumloop2.wav");
	loopBGMs_[0]->SoundLoadWave("./Resources/loopBGM/tempo_01.wav");
	loopBGMs_[1]->SoundLoadWave("./Resources/loopBGM/tempo_02.wav");
	loopBGMs_[2]->SoundLoadWave("./Resources/loopBGM/tempo_03.wav");
	loopBGMs_[3]->SoundLoadWave("./Resources/loopBGM/tempo_04.wav");
	loopBGMs_[4]->SoundLoadWave("./Resources/loopBGM/tempo_05.wav");
	//	音の再生
	Notes::StaticInitialize();

	//	音量の設定
	

}

Battle::~Battle() {



}

void Battle::Initialize() {

	EndPos = { 0.0f,2.0f,0.0f };

	player_->Initialize(makeCatmull_->GetFirstControlPoint());

	musicScoreList_.clear();

	for (int32_t i = 0; i < 4; i++) {
		std::unique_ptr<MusicScore> tmpScore = std::make_unique<MusicScore>();
		tmpScore->ModelLoad(notesModels_, noteTextures_);
		tmpScore->SetPlayer(player_.get());
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

	worldTransformLine_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransformLine_.scale_ *= 5.0f;
	worldTransformLine_.translation_.y = 10.0f;

}

void Battle::Update() {

	//フラグを降ろす
	MusicScore::isUpdateFlag_ = false;

	if (countMeasure_ <= 0/* && player_->GetLinePass() == 0*/) {
		loopCount_++;
		isUpdateScore_ = true;
		player_->SetLinePass(0);
		player_->SetDivisionNumber(float(maxCountMeasure_ / float(makeCatmull_->GetControlPoints().size() - 1) / 10.0f));
		
		if (loopCount_ == 7) {
			countMeasure_ = maxCountMeasure_ ;
		}
		else {
			countMeasure_ = maxCountMeasure_;
		}
		

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

	UpdateObjects();

	if (isUpdateScore_) {
		UpdateScores();
		isUpdateScore_ = false;
	}

	if (player_->GetIsMove()) {

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
	else {
		for (int i = 0; i < 5; i++) {
			loopBGMs_[i]->SoundStop();
		}
	}

}

void Battle::UpdateObjects() {

	for (const auto& score : musicScoreList_) {
		score->Update(makeCatmull_->GetControlPoints());
	}

	player_->Update(makeCatmull_->GetControlPoints(), makeCatmull_->GetLastLinePass());

	ControlPoints_ = makeCatmull_->GetControlPoints();

	if (lines_.size() < makeCatmull_->GetControlPoints().size()) {
		lines_.push_back(std::make_unique<Line>());
	}
	else if (lines_.size() > makeCatmull_->GetControlPoints().size()) {
		lines_.pop_back();
	}

	//makeCatmull_->Update();
	//battleAnimation_->Update();

	worldTransformLine_.UpdateMatrix();

}

void Battle::Draw3D(const Matrix4x4& viewProjection) {

	skydome_->Draw(viewProjection);

	player_->Draw(viewProjection);

	for (const auto& score : musicScoreList_) {
		score->Draw(viewProjection);
	}

	makeCatmull_->Draw(viewProjection);

	for (size_t i = 0; i < lines_.size(); i++){
		lines_[i]->DrawLine(ControlPoints_[i] - EndPos, ControlPoints_[i] + EndPos, viewProjection, 0xff0000ff);
	}

	//battleAnimation_->Draw3D(viewProjection);
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
	if (!musicScoreList_.empty()) {
		//最前列を後列に追加
		musicScoreList_.push_back(std::move(musicScoreList_.front()));
		musicScoreList_.pop_front();
		musicScoreList_.back().get()->SetNotes(MusicScore::ScoreType(rand() % 10), makeCatmull_->GetControlPoints(), 3);
	}

}
