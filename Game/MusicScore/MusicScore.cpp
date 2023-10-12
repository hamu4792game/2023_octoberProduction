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

void MusicScore::Update(std::vector<Vector3> position) {

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

	if (++countBeat_ >= beat_) {

		for (Notes* note : notes_) {
			note->SetSize(1.5f);
		}

		countBeat_ = 0;

	}

	//判定更新
	{

		//全てのノーツの最小距離
		float minDistance = 30.0f;

		//どのノーツと当たり判定を取るのか管理する変数
		uint32_t updateNumber = -1;

		for (Notes* note : notes_) {

			if (Distance(note->GetPosition(), player_->GetPosition()) < minDistance) {
				updateNumber = note->GetNumber();
				minDistance = Distance(note->GetPosition(), player_->GetPosition());
			}

		}

		for (Notes* note : notes_) {

			if (&position[note->GetNumber()]) {
				note->SetPosition(position[note->GetNumber()]);
			}

			if (note->GetNumber() == updateNumber) {
				note->UpdateFlag();
			}

			note->Update();
		}

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

			//要素がある場合に処理
			if (&position[i]) {

				if (i >= 1 && i <= 3 && i != 2) {

					SetNoteNormal(position[i], i);

				}
				else if (i == 4) {

					SetNoteLStart(position[i], i);

				}
				else if (i == 8) {

					SetNoteLEnd(position[i], i);

				}
				else if (i == 10) {

					SetNoteLStart(position[i], i);

				}
				else if (i == 12) {

					SetNoteLEnd(position[i], i);

				}
				else if (i == 14) {

					SetNoteDamage(position[i], i);

				}

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

void MusicScore::SetNoteNormal(const Vector3& position, uint32_t num) {

	NoteNormal* newNote = new NoteNormal();
	newNote->ModelLoad(notesModels_[0]);
	newNote->Initialize();
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	newNote->SetNumber(num);
	notes_.push_back(newNote);

}

void MusicScore::SetNoteLStart(const Vector3& position, uint32_t num) {

	NoteLong* newNote = new NoteLong();
	newNote->ModelLoad(notesModels_[1]);
	newNote->Initialize();
	newNote->SetLongNoteType(NoteLong::Start);
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	newNote->SetNumber(num);
	notes_.push_back(newNote);

}

void MusicScore::SetNoteLEnd(const Vector3& position, uint32_t num) {

	NoteLong* newNote = new NoteLong();
	newNote->ModelLoad(notesModels_[1]);
	newNote->Initialize();
	newNote->SetLongNoteType(NoteLong::End);
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	newNote->SetNumber(num);
	notes_.push_back(newNote);

}

void MusicScore::SetNoteDamage(const Vector3& position, uint32_t num) {

	NoteDamage* newNote = new NoteDamage();
	newNote->ModelLoad(notesModels_[2]);
	newNote->Initialize();
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	newNote->SetNumber(num);
	notes_.push_back(newNote);

}
