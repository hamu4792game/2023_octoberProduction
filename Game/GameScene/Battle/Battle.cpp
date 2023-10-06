#include "Battle.h"

Battle::Battle()
{

	player_ = std::make_unique<Player>();
	player_->ModelLoad();

	skydome_ = std::make_unique<Skydome>();
	skydome_->ModelLoad();

	musicScore_ = std::make_unique<MusicScore>();
	musicScore_->SetPlayer(player_.get());
	

}

Battle::~Battle() {



}

void Battle::Initialize()
{

	player_->Initialize();
	musicScore_->Initialize();

}

void Battle::Update()
{

	player_->Update();

	musicScore_->Update();

}

void Battle::Draw3D(const Matrix4x4& viewProjection)
{

	skydome_->Draw(viewProjection);

	player_->Draw(viewProjection);

	musicScore_->Draw(viewProjection);

}

void Battle::Draw2D(const Matrix4x4& viewProjection)
{

}

void Battle::ModelLoad(std::vector<Model*> models) {

	notesModels_ = models;
	musicScore_->ModelLoad(notesModels_);
}
