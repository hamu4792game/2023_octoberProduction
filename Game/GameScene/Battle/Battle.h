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

#include "Game/BattleAnimation/BattleAnimation.h"
#include "Engine/Camera/Camera.h"


class Battle {
public:
	Battle(std::shared_ptr<Camera> camera = nullptr);
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

	void SetCountMeasure(float bpm){ 
		maxCountMeasure_ = int(3600 / bpm * 4);
	}

private:

	std::unique_ptr<Player> player_;

	std::vector<std::unique_ptr<MusicScore>> musicScores_;
	
	std::unique_ptr<MusicScore> musicScore_;

	std::unique_ptr<Skydome> skydome_;

	std::vector<Model*> notesModels_;

	float BPM_ = 120.0f;

	//一小節のカウント
	int maxCountMeasure_ = int(3600 / BPM_ * 4);

	int countMeasure_ = 0;

private:
	std::vector<std::unique_ptr<Line>> lines_;

	std::vector<Vector3> ControlPoints_;

	std::unique_ptr<MakeCatmull> makeCatmull_;

	std::unique_ptr<AudioInput> drumLoop_;

	Vector3 EndPos;

	std::unique_ptr<BattleAnimation> battleAnimation_;

public: // セッター
	void SetHeroModels(std::vector<std::shared_ptr<Model>> model) { battleAnimation_->SetHeroModels(model); }
	void SetBossModels(std::vector<std::shared_ptr<Model>> model) { battleAnimation_->SetBossModels(model); }

};
