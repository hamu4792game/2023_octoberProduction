#include "MusicScore.h"

MusicScore::MusicScore() {

}

MusicScore::~MusicScore() {

	for (Notes* note : notes_) {
		delete note;
	}

}

void MusicScore::Initialize() {

	for (int i = 0; i < 5; i++) {
		
		Notes* newNote = new Notes();
		newNote->LoadModel();
		newNote->Initialize();
		newNote->SetPlayer(player_);
		newNote->SetPosition({ 0.0f,0.0f, i * 15.0f });
		notes_.push_back(newNote);

	}

}

void MusicScore::Update() {

	notes_.remove_if([](Notes* note) {

		if (note->GetIsHit()) {
			return true;
		}

		return false;

		});

	for (Notes* note : notes_) {
		note->Update();
	}

}

void MusicScore::Draw(const Matrix4x4& viewProjection) {

	for (Notes* note : notes_) {
		note->Draw(viewProjection);
	}

}
