#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Notes.h"
#include "Player.h"
#include <memory>
#include <list>

class Battle {
public:
	Battle();
	~Battle();

	//	初期化
	void Initialize();
	//	更新処理
	void Update();
	//	描画処理
	void Draw3D(const Matrix4x4& viewProjection);
	void Draw2D(const Matrix4x4& viewProjection);

private:

	std::unique_ptr<Player> player_;

	std::list<Notes*> notes_;


};
