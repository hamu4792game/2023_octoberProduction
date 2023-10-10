#include "Battle.h"

Battle::Battle()
{

	player_ = std::make_unique<Player>();
	player_->ModelLoad();

	skydome_ = std::make_unique<Skydome>();
	skydome_->ModelLoad();

	musicScore_ = std::make_unique<MusicScore>();
	musicScore_->SetPlayer(player_.get());
	
	line_ = std::make_unique<Line>();

	makeCatmull_ = std::make_unique<MakeCatmull>();

	makeCatmull_->Initialize();

	drumLoop_ = std::make_unique<AudioInput>();

	//	ロード
	drumLoop_->SoundLoadWave("./Resources/loopBGM/drumloop1.wav");
	//	音の再生
	drumLoop_->SoundPlayWave(true);
	//	音量の設定
	drumLoop_->SetVolume(0.2f);

}

Battle::~Battle() {



}

void Battle::Initialize()
{
	StartPos = { 0.0f,0.0f,0.0f };

	EndPos = { 100.0f,0.0f,0.0f };

	StartPos2 = { 0.0f,0.0f,0.0f };

	EndPos2 = { 0.0f,100.0f,0.0f };

	player_->Initialize(makeCatmull_->GetFirstControlPoint());
	musicScore_->Initialize();

	

}

void Battle::Update()
{

	if (musicScore_->IsEmpty()) {
		musicScore_->SetNotes(MusicScore::Easy_01, makeCatmull_->GetControlPoints());
	}

	player_->Update(makeCatmull_->GetControlPoints(),makeCatmull_->GetLastLinePass());

	musicScore_->Update(makeCatmull_->GetControlPoints());

	//makeCatmull_->Update();

}

void Battle::Draw3D(const Matrix4x4& viewProjection)
{

	skydome_->Draw(viewProjection);

	player_->Draw(viewProjection);

	musicScore_->Draw(viewProjection);

	makeCatmull_->Draw(viewProjection);

	/*line_->DrawLine(StartPos, EndPos, viewProjection, 0xff0000ff);

	line_->DrawLine(StartPos2, EndPos2, viewProjection, 0x00ff00ff);*/

}

void Battle::Draw2D(const Matrix4x4& viewProjection)
{

}

void Battle::ModelLoad(std::vector<Model*> models) {

	notesModels_ = models;
	musicScore_->ModelLoad(notesModels_);
}
