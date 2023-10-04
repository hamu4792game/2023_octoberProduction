#pragma once
#include "Notes.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include <list>
#include "Player.h"

class MusicScore
{
public:

	MusicScore();
	~MusicScore();

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_;

	std::list<Notes*> notes_;

};

