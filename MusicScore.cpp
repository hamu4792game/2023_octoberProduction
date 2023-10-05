#include "MusicScore.h"

MusicScore::MusicScore() {

}

MusicScore::~MusicScore() {

	for (Notes* note : notes_) {
		delete note;
	}

}

void MusicScore::Initialize() {

}

void MusicScore::Update() {

	if (IsEmpty()) {
		SetNotes(Easy_01);
	}

	notes_.remove_if([](Notes* note) {

		if (note->GetIsHit() || note->GetIsMiss()) {
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

void MusicScore::SetNotes(ScoreType type) {

	switch (type)
	{
	case MusicScore::Easy_01:

		for (int i = 0; i < kMaxNotes; i++) {

			if (i <= 3 || i >= 14) {

				NoteNormal* newNote = new NoteNormal();
				newNote->ModelLoad(notesModels_[0]);
				newNote->Initialize();
				newNote->SetPlayer(player_);
				newNote->SetPosition({ i * 5.0f - 40.0f,0.0f, 0.0f });
				notes_.push_back(newNote);

			}
			else if (i >= 4 && i <= 5) {

				NoteLong* newNote = new NoteLong();
				newNote->ModelLoad(notesModels_[1]);
				newNote->Initialize();
				newNote->SetPlayer(player_);
				newNote->SetPosition({ i * 5.0f - 40.0f,0.0f, 0.0f });
				notes_.push_back(newNote);

			}
			else if(i <= 10 && i >= 8) {

				NoteDamage* newNote = new NoteDamage();
				newNote->ModelLoad(notesModels_[2]);
				newNote->Initialize();
				newNote->SetPlayer(player_);
				newNote->SetPosition({ i * 5.0f - 40.0f,0.0f, 0.0f });
				notes_.push_back(newNote);

			}

			

		}

		break;
	default:
		break;
	}

}

void MusicScore::ModelLoad(std::vector<Model*> models) {

	notesModels_ = models;

}
