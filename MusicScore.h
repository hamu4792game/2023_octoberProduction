#pragma once
#include "Notes.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include <list>
#include "Player.h"
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

	void SetNotes(ScoreType type);

	bool IsEmpty() { return notes_.empty(); }

	void ModelLoad(std::vector<Model*> models);

private:

	Player* player_;

	std::list<Notes*> notes_;

	std::vector<Model*> notesModels_;

};

