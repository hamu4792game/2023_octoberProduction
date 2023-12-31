#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Game/Player/Player.h"
#include <memory>
#include <vector>
#include "Game/Skydome/Skydome.h"
#include "Game/MusicScore/MusicScore.h"
#include "Game/NotesEffect/NotesEffect.h"

#include "Engine/Texture/Line/Line.h"

#include "Game/MakeCatmull/MakeCatmull.h"
#include "Engine/Input/AudioInput/AudioInput.h"

#include "Game/BattleAnimation/BattleAnimation.h"
#include "Engine/Camera/Camera.h"
#include "Game/PartsEnum.h"


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

	void ModelLoad(std::vector<Model*> models, std::vector<Texture2D*> textures);

	Player* GetPlayer() { return player_.get(); }

	void SetCountMeasure(float bpm){ 
		maxCountMeasure_ = int(3600 / bpm * 4);
	}

	//ゲームクリアフラグ
	bool isGameClear_ = false;
	//ゲームオーバーフラグ
	bool isGameOver_ = false;

private:

	void UpdateScores();

	//プレイヤー、譜面などの更新処理を纏めたもの
	void UpdateObjects();

	std::unique_ptr<Player> player_;

	std::unique_ptr<AudioInput> loopBGMs_[9];

	std::list<std::unique_ptr<MusicScore>> tutorialMusicScoreList_;

	std::list<std::unique_ptr<MusicScore>> musicScoreList_;

	std::unique_ptr<MusicScore> musicScores_[1];
	
	/*MusicScore* currentMusicScore_;*/

	std::unique_ptr<Skydome> skydome_;

	std::vector<Model*> notesModels_;

	Model* notesEffectModel_;

	std::vector<Texture2D*> UITextures_;

	std::list<std::unique_ptr<NotesEffect>> notesEffects_;

	std::vector<Texture2D*> noteTextures_;

	WorldTransform worldTransformLine_;

	float BPM_ = 150.0f;

	//一小節のカウント
	int maxCountMeasure_ = int(3600 / BPM_ * 4);

	int countMeasure_ = maxCountMeasure_;

	uint32_t loopCount_ = 8;

	//更新処理速度によるズレを修正するための変数
	float frameDifference_ = 0.0f;

	//譜面の配置を更新するかどうか
	bool isUpdateScore_ = false;

	//フレーム確認
	int frameCounter_ = 0;

	//チュートリアル完了フラグ
	bool endTutorial_ = false;

	//ストップフラグ
	bool isStop_ = false;

	//チュートリアルでノーツを叩いた数
	int tutorialNotesCount_ = 0;

	int testScoreCount_ = 0;

	//現在の進行度
	int currentStage_ = 0;

	//ノルマ数
	int goalNotesCount_ =25;

	//現在のノーツ叩いた数
	int currentNotesCount_ = 0;

	void SetNextGoalNotes();


private:
	std::vector<std::unique_ptr<Line>> lines_;

	std::vector<Vector3> ControlPoints_;

	std::unique_ptr<MakeCatmull> makeCatmull_;

	Vector3 EndPos;

	std::unique_ptr<BattleAnimation> battleAnimation_;

private:
	
	//	箱用
	WorldTransform boxTrans_;

	Texture2D* boxtexture_;

	uint32_t boxColor_;

	//	タイトル
	Texture2D* titleTexture_;
	WorldTransform titleTrans_;

	//ボタン
	int changeCount_ = 0;
	Texture2D* BButtomTexture_;
	Texture2D* pushBButtomTexture_;
	WorldTransform BButtomTrans_;

	WorldTransform stageTrans_[6];
	WorldTransform gaugeTrans_;
	WorldTransform gaugeFrameTrans_;

	float easeFrame_ = 0.0f;
	bool titleFlag_ = false;
	bool startFlag_ = false;
	Vector3 titleStartPos_;
	Vector3 titleEndPos_;

	AudioInput* selectSE_;

private:
	//	カメラ移動用のパラメーターセット関数
	void SetCameraMove();
	Camera* camera_ = nullptr;
	bool cameraMoveFlag = false;
	Vector3 cameraMoveStart; Vector3 cameraMoveEnd;
	Vector3 cameraRotateStart; Vector3 cameraRotateEnd;
	float cameraT_ = 0.0f;
	float cameraTspeed_ = 0.0f;
	MovePattern movepattern_ = MovePattern::Run;

	//	カメラを四角に回す
	void QuadRangle();
	int quadFrame_ = 0;
	int quadFlag_ = 0;
	bool quadMoveFlag = false;

	// box出すフラグ
	bool isBoxFlag = false;

public: // セッター
	void SetHeroModels(std::vector<std::shared_ptr<Model>> model) { battleAnimation_->SetHeroModels(model); }
	void SetBossModels(std::vector<std::shared_ptr<Model>> model) { battleAnimation_->SetBossModels(model); }
	void SetStageModels(std::vector<std::shared_ptr<Model>> model) { battleAnimation_->SetStageModels(model); }
	void SetNotesEffectModel(Model* model) { notesEffectModel_ = model; }
	void SetBossBulletModel(Model* model) { battleAnimation_->SetBossBulletModel(model); }
	void SetBoxTexture(Texture2D* texture) { boxtexture_ = texture; }
	void SetTitleTexture(Texture2D* texture) { titleTexture_ = texture; }
	void SetBButtomTexture(Texture2D* texture) { BButtomTexture_ = texture; }
	void SetPushBButtomTexture(Texture2D* texture) { pushBButtomTexture_ = texture; }
	void SetUI(std::vector<Texture2D*> textures);
	void SetSE(AudioInput* se) { selectSE_ = se; }
};
