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

	void Update(std::vector<Vector3> position);

	void Draw(const Matrix4x4& viewProjection);

	void SetPlayer(Player* player) { player_ = player; }

	void SetNotes(ScoreType type, std::vector<Vector3> position);

	bool IsEmpty() { return notes_.empty(); }

	void ModelLoad(std::vector<Model*> models);

	void SetNoteNormal(const Vector3& position, uint32_t num);

	void SetNoteLStart(const Vector3& position, uint32_t num);

	void SetNoteLEnd(const Vector3& position, uint32_t num);

	void SetNoteDamage(const Vector3& position, uint32_t num);

	void SetBPM(float tempo) { 
		BPM_ = tempo;
		beat_ = int(7200 / BPM_);
	}

private:

	Player* player_;

	std::list<Notes*> notes_;

	std::vector<Model*> notesModels_;

	//曲のテンポの速さ
	float BPM_ = 120.0f;

	int beat_ = int(7200 / BPM_);

	int countBeat_ = 0;

};

