#include "MusicScore.h"

//静的メンバ変数実体
bool MusicScore::isUpdateFlag_ = false;

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

		//
		float nearFrame = 20.0f;

		//どのノーツと当たり判定を取るのか管理する変数
		uint32_t updateNumber = -1;

		for (Notes* note : notes_) {

			if (fabsf(note->GetJudgeFrame()) < nearFrame) {
				updateNumber = note->GetNumber();
				nearFrame = fabsf(note->GetJudgeFrame());
			}

		}

		for (Notes* note : notes_) {

			if (position.size() > note->GetNumber()) {
				note->SetPosition(position[note->GetNumber()]);
			}

			//全てのMusicScoreのノーツの中から一つだけ当たり判定を取る
			if (note->GetNumber() == updateNumber && isUpdateFlag_ == false) {
				note->UpdateFlag();
				isUpdateFlag_ = true;
			}

			if (player_->GetIsMove()) {
				note->MoveJudgeFrame();
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

void MusicScore::Draw2D(const Matrix4x4& viewProjection) {

	for (Notes* note : notes_) {
		note->Draw2D(viewProjection);
	}

}

void MusicScore::SetNotes(ScoreType type, std::vector<Vector3> position, int32_t offset) {

	notes_.remove_if([](Notes* note) {

		delete note;
		return true;

	});

	float judgeLine;

	if (position.size() != 0) {
		judgeLine = float(maxCountMeasure_ / float(position.size() - 1));
	}
	else {
		judgeLine = float(maxCountMeasure_);
	}

	switch (type)
	{
	case MusicScore::Easy_01:

		for (int i = 0; i < position.size() - 1; i++) {
			if (i == 0) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
		}

		break;
	case MusicScore::Easy_02:

		for (int i = 0; i < position.size() - 1; i++) {
			if (i % 8 == 0) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
		}

		break;
	case MusicScore::Easy_03:

		for (int i = 0; i < position.size() - 1; i++) {
			if (i % 4 == 0 && i < 12) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
		}

		break;
	case MusicScore::Easy_04:

		for (int i = 0; i < position.size() - 1; i++) {
			if (i % 4 == 0) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
		}

		break;
	case MusicScore::Easy_05:

		for (int i = 0; i < position.size() - 1; i++) {
			if (i == 0 || i == 8 || i == 12) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
		}

		break;
	case MusicScore::Normal_01:

		for (int i = 0; i < position.size() - 1; i++) {
			if (i == 0 || i == 12) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
			else if (i >= 4 && i < 10 && i % 2 == 0) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
		}

		break;
	case MusicScore::Normal_02:

		for (int i = 0; i < position.size() - 1; i++) {
			if (i % 2 == 0 && i != 2 && i < 14) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
		}

		break;
	case MusicScore::Normal_03:

		for (int i = 0; i < position.size() - 1; i++) {
			if (i % 2 == 0 && i != 6 && i < 14) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
		}

		break;
	case MusicScore::Normal_04:

		for (int i = 0; i < position.size() - 1; i++) {
			if (i % 2 == 0 && i < 14) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
		}

		break;
	case MusicScore::Normal_05:

		for (int i = 0; i < position.size() - 1; i++) {
			if (i % 2 == 0) {

				SetNoteNormal(position[i], i, judgeLine * i + offset * maxCountMeasure_);

			}
		}

		break;
	default:
	case MusicScore::Rest:
		break;
	}

}

void MusicScore::ModelLoad(std::vector<Model*> models, std::vector<Texture2D*> textures) {

	notesModels_ = models;

	noteTextures_ = textures;

}

void MusicScore::SetNoteNormal(const Vector3& position, uint32_t num, float judgeline) {

	NoteNormal* newNote = new NoteNormal();
	newNote->ModelLoad(notesModels_);
	newNote->TextureLoad(noteTextures_[0]);
	newNote->Initialize();
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	newNote->SetNumber(num);
	newNote->SetJudgeFrame(judgeline);
	notes_.push_back(newNote);

}

void MusicScore::SetNoteLStart(const Vector3& position, uint32_t num, float judgeline) {

	NoteLong* newNote = new NoteLong();
	newNote->ModelLoad(notesModels_);
	newNote->TextureLoad(noteTextures_[0]);
	newNote->Initialize();
	newNote->SetLongNoteType(NoteLong::Start);
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	newNote->SetNumber(num);
	newNote->SetJudgeFrame(judgeline);
	notes_.push_back(newNote);

}

void MusicScore::SetNoteLEnd(const Vector3& position, uint32_t num, float judgeline) {

	NoteLong* newNote = new NoteLong();
	newNote->ModelLoad(notesModels_);
	newNote->TextureLoad(noteTextures_[0]);
	newNote->Initialize();
	newNote->SetLongNoteType(NoteLong::End);
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	newNote->SetNumber(num);
	newNote->SetJudgeFrame(judgeline);
	notes_.push_back(newNote);

}

void MusicScore::SetNoteDamage(const Vector3& position, uint32_t num, float judgeline) {

	NoteDamage* newNote = new NoteDamage();
	newNote->ModelLoad(notesModels_);
	newNote->TextureLoad(noteTextures_[0]);
	newNote->Initialize();
	newNote->SetPlayer(player_);
	newNote->SetPosition(position);
	newNote->SetNumber(num);
	newNote->SetJudgeFrame(judgeline);
	notes_.push_back(newNote);

}
