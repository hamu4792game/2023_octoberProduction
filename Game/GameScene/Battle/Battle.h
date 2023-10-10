#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Game/Player/Player.h"
#include <memory>
#include <vector>
#include "Game/Skydome/Skydome.h"
#include "Game/MusicScore/MusicScore.h"

#include "Engine/Texture/Line/Line.h"

#include "Game/MakeCatmull/MakeCatmull.h"
#include "Engine/Input/AudioInput/AudioInput.h"



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

	Player* GetPlayer() { return player_.get(); }

private:

	std::unique_ptr<Player> player_;

	std::unique_ptr<MusicScore> musicScore_;

	std::unique_ptr<Skydome> skydome_;

	std::vector<Model*> notesModels_;

	


private:
	std::unique_ptr<Line> line_;

	std::unique_ptr<MakeCatmull> makeCatmull_;

	std::unique_ptr<AudioInput> drumLoop_;

	Vector3 StartPos;

	Vector3 EndPos;


	Vector3 StartPos2;

	Vector3 EndPos2;
};
