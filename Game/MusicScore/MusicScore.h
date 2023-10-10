#pragma once
#include "Game/Notes/Notes.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include <list>
#include "Game/Player/Player.h"
#include <memory>

class MusicScore
{
public:

	MusicScore();
	~MusicScore();

	//最大ノーツ数
	static const uint32_t kMaxNotes = 16;

	//譜面の種類
	enum ScoreType {
		Easy_01,
	};

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void SetPlayer(Player* player) { player_ = player; }

	void SetNotes(ScoreType type, std::vector<Vector3> position);

	bool IsEmpty() { return notes_.empty(); }

	void ModelLoad(std::vector<Model*> models);

	void SetNoteNormal(const Vector3& position);

	void SetNoteLStart(const Vector3& position);

	void SetNoteLEnd(const Vector3& position);

	void SetNoteDamage(const Vector3& position);

	void SetBPM(uint32_t tempo) { BPM_ = tempo; }

private:

	Player* player_;

	std::list<Notes*> notes_;

	std::vector<Model*> notesModels_;

	//曲のテンポの速さ
	uint32_t BPM_;

};

