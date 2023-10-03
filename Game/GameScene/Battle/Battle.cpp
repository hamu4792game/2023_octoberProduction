#include "Battle.h"

Battle::Battle()
{
}

Battle::~Battle() {

	for (Notes* note : notes_) {
		delete note;
	}

}

void Battle::Initialize()
{

	player_ = std::make_unique<Player>();
	player_->LoadModel();

	for (int i = 0; i < 5; i++) {
		Notes* note = new Notes();
		note->LoadModel();
		note->SetPosition({ 0.0f,0.0f,i * 10.0f });
		notes_.push_back(note);

	}

}

void Battle::Update()
{

	player_->Update();

	for (Notes* note : notes_) {
		note->Update();
	}

}

void Battle::Draw3D(const Matrix4x4& viewProjection)
{

	player_->Draw(viewProjection);

	for (Notes* note : notes_) {
		note->Draw(viewProjection);
	}

}

void Battle::Draw2D(const Matrix4x4& viewProjection)
{

}
