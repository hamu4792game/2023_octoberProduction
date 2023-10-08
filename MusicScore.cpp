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
	/*SetNotes(Easy_01);*/
}

void MusicScore::Update() {

	/*if (player_->GetPosition().x >= 39.5f) {
		SetNotes(Easy_01);
	}*/

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

void MusicScore::SetNotes(ScoreType type, std::vector<Vector3> position) {

	notes_.remove_if([](Notes* note) {

		delete note;
		return true;

	});

	switch (type)
	{
	case MusicScore::Easy_01:

		for (int i = 0; i < kMaxNotes; i++) {

			if (i >= 1 && i <= 3 && i != 2) {

				SetNoteNormal(position[i]);

			}
			else if (i == 4) {

				SetNoteLStart(position[i]);

			}
			else if (i == 8) {

				SetNoteLEnd(position[i]);

			}
			else if(i == 10) {

				SetNoteLStart(position[i]);
			
			}
			else if (i == 12) {

				SetNoteLEnd(position[i]);

			}
			else if (i == 14) {

				SetNoteDamage(position[i]);

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

void MusicScore::SetNoteNormal(const Vector3& position) {

	NoteNormal* newNote = new NoteNormal();
	newNote->ModelLoad(notesModels_[0]);
	newNote->Initialize();
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	notes_.push_back(newNote);

}

void MusicScore::SetNoteLStart(const Vector3& position) {

	NoteLong* newNote = new NoteLong();
	newNote->ModelLoad(notesModels_[1]);
	newNote->Initialize();
	newNote->SetLongNoteType(NoteLong::Start);
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	notes_.push_back(newNote);

}

void MusicScore::SetNoteLEnd(const Vector3& position) {

	NoteLong* newNote = new NoteLong();
	newNote->ModelLoad(notesModels_[1]);
	newNote->Initialize();
	newNote->SetLongNoteType(NoteLong::End);
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	notes_.push_back(newNote);

}

void MusicScore::SetNoteDamage(const Vector3& position) {

	NoteDamage* newNote = new NoteDamage();
	newNote->ModelLoad(notesModels_[2]);
	newNote->Initialize();
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	notes_.push_back(newNote);

}
