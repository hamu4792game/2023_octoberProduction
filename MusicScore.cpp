#include "MusicScore.h"

MusicScore::MusicScore() {

	NoteLong::StaticInitialize();

}

MusicScore::~MusicScore() {

	for (Notes* note : notes_) {
		delete note;
	}

}

void MusicScore::Initialize() {
	SetNotes(Easy_01);
}

void MusicScore::Update() {

	if (player_->GetPosition().x >= 39.5f) {
		SetNotes(Easy_01);
	}

	notes_.remove_if([](Notes* note) {

		if (note->GetIsHit() || note->GetIsMiss()) {
			delete note;
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

	notes_.remove_if([](Notes* note) {

		delete note;
		return true;

	});

	switch (type)
	{
	case MusicScore::Easy_01:

		for (int i = 0; i < kMaxNotes; i++) {

			if (i >= 1 && i <= 3 && i != 2) {

				NoteNormal* newNote = new NoteNormal();
				newNote->ModelLoad(notesModels_[0]);
				newNote->Initialize();
				newNote->SetPlayer(player_);
				newNote->SetPosition({ i * 5.0f - 40.0f,0.0f, 0.0f });
				notes_.push_back(newNote);

			}
			else if (i == 4) {

				NoteLong* newNote = new NoteLong();
				newNote->ModelLoad(notesModels_[1]);
				newNote->Initialize();
				newNote->SetLongNoteType(NoteLong::Start);
				newNote->SetPlayer(player_);
				newNote->SetPosition({ i * 5.0f - 40.0f,0.0f, 0.0f });
				notes_.push_back(newNote);

			}
			else if (i == 8) {

				NoteLong* newNote = new NoteLong();
				newNote->ModelLoad(notesModels_[1]);
				newNote->Initialize();
				newNote->SetLongNoteType(NoteLong::End);
				newNote->SetPlayer(player_);
				newNote->SetPosition({ i * 5.0f - 40.0f,0.0f, 0.0f });
				notes_.push_back(newNote);

			}
			else if(i == 10) {

				NoteLong* newNote = new NoteLong();
				newNote->ModelLoad(notesModels_[1]);
				newNote->Initialize();
				newNote->SetLongNoteType(NoteLong::Start);
				newNote->SetPlayer(player_);
				newNote->SetPosition({ i * 5.0f - 40.0f,0.0f, 0.0f });
				notes_.push_back(newNote);
			
			}
			else if (i == 12) {

				NoteLong* newNote = new NoteLong();
				newNote->ModelLoad(notesModels_[1]);
				newNote->Initialize();
				newNote->SetLongNoteType(NoteLong::End);
				newNote->SetPlayer(player_);
				newNote->SetPosition({ i * 5.0f - 40.0f,0.0f, 0.0f });
				notes_.push_back(newNote);

			}
			else if (i == 14) {

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
