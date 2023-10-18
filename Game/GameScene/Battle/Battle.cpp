#include "Battle.h"

Battle::Battle(std::shared_ptr<Camera> camera)
{

	player_ = std::make_unique<Player>();
	player_->ModelLoad();

	skydome_ = std::make_unique<Skydome>();
	skydome_->ModelLoad();

	musicScore_ = std::make_unique<MusicScore>();
	musicScore_->SetPlayer(player_.get());
	musicScore_->SetBPM(BPM_);
	
	

	makeCatmull_ = std::make_unique<MakeCatmull>();

	makeCatmull_->Initialize();

	
	battleAnimation_ = std::make_unique<BattleAnimation>(camera);
	

	drumLoop_ = std::make_unique<AudioInput>();

	//	ロード
	drumLoop_->SoundLoadWave("./Resources/loopBGM/drumloop2.wav");
	//	音の再生
	
	//	音量の設定
	

}

Battle::~Battle() {



}

void Battle::Initialize() {
	EndPos = { 0.0f,2.0f,0.0f };

	player_->Initialize(makeCatmull_->GetFirstControlPoint());
	musicScore_->Initialize();

	battleAnimation_->Initialize();


}

void Battle::Update() {

	if (player_->GetIsMove()) {
		countMeasure_--;
	}

	if (countMeasure_ <= 0/* && player_->GetLinePass() == 0*/) {
		musicScore_->SetNotes(MusicScore::Easy_01, makeCatmull_->GetControlPoints());
		player_->SetLinePass(0);
		player_->SetDivisionNumber(float(maxCountMeasure_ / float(makeCatmull_->GetControlPoints().size() - 1) / 10.0f));
		countMeasure_ = maxCountMeasure_ - 1;

		if (player_->GetIsMove()) {
			drumLoop_->SoundStop();
			drumLoop_->SoundPlayWave();
			drumLoop_->SetVolume(0.2f);
		}
		
	}

	player_->Update(makeCatmull_->GetControlPoints(),makeCatmull_->GetLastLinePass());

	musicScore_->Update(makeCatmull_->GetControlPoints());

	ControlPoints_ = makeCatmull_->GetControlPoints();

	if (lines_.size() < makeCatmull_->GetControlPoints().size()) {
		lines_.push_back(std::make_unique<Line>());
	}
	else if (lines_.size() > makeCatmull_->GetControlPoints().size()){
		lines_.pop_back();
	}

	//makeCatmull_->Update();
	//battleAnimation_->Update();

}

void Battle::Draw3D(const Matrix4x4& viewProjection) {

	skydome_->Draw(viewProjection);

	player_->Draw(viewProjection);

	musicScore_->Draw(viewProjection);

	makeCatmull_->Draw(viewProjection);

	for (size_t i = 0; i < lines_.size(); i++){
		lines_[i]->DrawLine(ControlPoints_[i] - EndPos, ControlPoints_[i] + EndPos, viewProjection, 0xff0000ff);
	}

	battleAnimation_->Draw3D(viewProjection);
}

void Battle::Draw2D(const Matrix4x4& viewProjection) {

}

void Battle::ModelLoad(std::vector<Model*> models) {

	notesModels_ = models;
	musicScore_->ModelLoad(notesModels_);
}
