#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Player.h"
#include <memory>
#include <vector>
#include "Game/Skydome/Skydome.h"
#include "MusicScore.h"

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

	void ModelLoad(std::vector<Model*> models);

private:

	std::unique_ptr<Player> player_;

	std::unique_ptr<MusicScore> musicScore_;

	std::unique_ptr<Skydome> skydome_;

	std::vector<Model*> notesModels_;

};
