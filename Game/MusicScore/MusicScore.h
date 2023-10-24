#pragma once
#include "Game/Notes/Notes.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include <list>
#include "Game/Player/Player.h"
#include <memory>
#include "Game/Character/Boss/Boss.h"
#include "Game/Character/Hero/Hero.h"

class MusicScore
{
public:

	MusicScore();
	~MusicScore();

	//最大ノーツ数
	/*static const uint32_t kMaxNotes = 16;*/

	//譜面の種類
	//後ろの数字は十六分音符単位での配置
	enum ScoreType {
		Easy_01, //全音符1つ : 1000 0000 0000 0000
		Easy_02, //二分音符2つ : 1000 0000 1000 1000
		Easy_03, //四分音符3つ : 1000 1000 1000 0000
		Easy_04, //四分音符4つ : 1000 1000 1000 1000
		Easy_05, //二分1つ、四分2つ : 1000 0000 1000 1000
		Normal_01, //四分3つ、八分2つ : 1000 1010 1000 1000
		Normal_02, //四分2つ、八分4つ : 1000 1010 1010 1000
		Normal_03, //四分2つ、八分4つ : 1010 1000 1010 1000
		Normal_04, //四分1つ、八分6つ : 1010 1010 1010 1000
		Normal_05, //八分8つ : 1010 1010 1010 1010
		Rest, //全休符 : 0000 0000 0000 0000
		TUTORIAL_01, //チュートリアル用ノーツの配置
		TUTORIAL_02, 
	};

	void Initialize();

	void Update(std::vector<Vector3> position);

	void Draw(const Matrix4x4& viewProjection);

	void Draw2D(const Matrix4x4& viewProjection);

	void SetPlayer(Player* player) { player_ = player; }

	void SetBoss(Boss* boss) { boss_ = boss; }

	void SetHero(Hero* hero) { hero_ = hero; }

	void SetNotes(ScoreType type, std::vector<Vector3> position, int32_t offset);

	bool IsEmpty() { return notes_.empty(); }

	void ModelLoad(std::vector<Model*> models, std::vector<Texture2D*> textures);

	void SetNoteNormal(const Vector3& position, uint32_t num, float judgeline);

	void SetNoteLStart(const Vector3& position, uint32_t num, float judgeline);

	void SetNoteLEnd(const Vector3& position, uint32_t num, float judgeline);

	void SetNoteDamage(const Vector3& position, uint32_t num, float judgeline);

	void SetNoteTutorial(const Vector3& position, uint32_t num, float judgeline);

	void SetBPM(float tempo) { 
		BPM_ = tempo;
		beat_ = int(7200 / BPM_);
	}

	void SetCountMeasure(float bpm) {
		maxCountMeasure_ = int(3600 / bpm * 4);
	}

	//全てのMusicScoreから一つのノーツだけ判定を取るための変数
	static bool isUpdateFlag_;

	std::list<Notes*> GetNotes() { return notes_; }

	void SetIsStop(bool flag) { isStop_ = flag; }

	static bool isStopAll;

private:

	Player* player_;

	Boss* boss_;

	Hero* hero_;

	std::list<Notes*> notes_;

	std::vector<Model*> notesModels_;

	std::vector<Texture2D*> noteTextures_;

	//曲のテンポの速さ
	float BPM_ = 150.0f;

	int beat_ = int(7200 / BPM_);

	int countBeat_ = 0;

	//一小節のカウント
	int maxCountMeasure_ = int(3600 / BPM_ * 4);

	//一小節当たりの長さ
	const float scoreLength_ = 120.0f;

	//一ノーツ間の長さ(デフォルトは四分音符)
	float noteLength_ = scoreLength_ / 4.0f;

	bool isStop_ = false;

};

