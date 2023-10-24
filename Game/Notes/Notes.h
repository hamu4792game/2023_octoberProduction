#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include <memory>
#include "math/Matrix4x4.h"
#include "Game/Player/Player.h"
#include "math/Vector3.h"
#include "Engine/Texture/Texture2D.h"

class Boss;

class Hero;

class Notes
{
public:

	Notes();
	~Notes() = default;

	enum NoteType {
		Normal, //通常ノーツ
		Long, //長押し
		Damage //敵の妨害ノーツ

	};

	static int Combo;

	static bool isUpSE;

	static void StaticInitialize();

	virtual void Initialize();

	virtual void Update();

	//判定の更新
	virtual void UpdateFlag();

	virtual void Draw(const Matrix4x4& viewProjection);

	virtual void Draw2D(const Matrix4x4& viewProjection);

	virtual void ModelLoad(std::vector<Model*> models);

	void TextureLoad(Texture2D* texture) { lineTexture_ = texture; }

	//描画上のスタートポジションセット
	void SetStartPosition(const Vector3& position) { 
		startPosition_ = position;
	}

	Vector3 GetPosition() { return worldTransform_.translation_; }
	
	//描画上の中間ポジション
	void SetMiddlePosition(const Vector3& position) {
		middlePosition_ = position;
	}

	//描画上のゴールポジション
	void SetGoalPosition(const Vector3& position) {
		goalPosition_ = position;
	}

	void SetPlayer(Player* player) { player_ = player; }

	void SetBoss(Boss* boss) { boss_ = boss; }

	void SetHero(Hero* hero) { hero_ = hero; }

	void SetNoteType(NoteType type) { type_ = type; }

	bool GetIsHit() { return isHit_; }

	bool GetIsMiss() { return isMiss_; }

	void SetSize(float size) { worldTransform_.scale_ = { size, size, size }; }

	void SetNumber(uint32_t num) { number_ = num; }

	uint32_t GetNumber() { return number_; }

	Vector3 GetInsidePosition() { return worldTransformInside_.translation_; }

	void SetInsidePosition(const Vector3& position) {
		worldTransformInside_.translation_ = position;
		worldTransformInside_.UpdateMatrix();
	}

	void SetBPM(float bpm) {
		BPM_ = bpm;
		//4/4拍子の速さ
		noteSpeed_ = 30.0f / float(3600 / BPM_);
	}

	void SetJudgeFrame(float frame) { judgeFrame_ = frame; }

	float GetJudgeFrame() { return judgeFrame_; }

	void MoveJudgeFrame() { judgeFrame_ -= noteSpeed_; }

	//チュートリアルノーツにする
	void SetTutorialNotes() { isTutorialNotes_ = true; }

protected:

	//ノーツのSE(全ノーツで共有)
	static std::unique_ptr<AudioInput> notesSE_[3][8];

	static AudioInput* currentNotesSE_[2][8];

	NoteType type_ = Normal;

	std::vector<Model*> modelList_;

	//表示するモデル
	Model* model_ = nullptr;

	//ノーツの判定ライン画像
	Texture2D* lineTexture_;

	WorldTransform worldTransform_;
	WorldTransform worldTransformLine_;

	//内部判定用のワールドトランスフォーム
	WorldTransform worldTransformInside_;

	//描画上のスタートポジション
	Vector3 startPosition_{};

	//描画上の中間ポジション
	Vector3 middlePosition_{};

	//描画上のゴールポジション
	Vector3 goalPosition_{};

	//判定フレーム初期位置
	float judgeFrame_ = 0.0f;

	//BPM
	float BPM_ = 150.0f;

	//内部当たり判定のノーツの流れる速度
	float noteSpeed_ = 1.0f;

	Player* player_;

	Boss* boss_;

	Hero* hero_;

	//元のサイズ
	const float kNormalSize = 1.0f;

	//ノーツの配置番号
	uint32_t number_ = 0;

	//速度
	Vector3 velocity_;

	//ノーツを叩いたかどうか
	bool isHit_ = false;

	//プレイヤーがノーツに触れたか
	bool isTouch_ = false;

	//ノーツをスルー、または反応がずれすぎた場合ミス
	bool isMiss_ = false;

	//サイズを戻す演出
	void RestoreSize();

	//攻撃命令を出したかどうかのフラグ
	bool isAttack_ = false;

	//チュートリアルノーツかどうか
	bool isTutorialNotes_ = false;

};

class NoteNormal : public Notes
{
public:
	NoteNormal();
	~NoteNormal() = default;

	void Initialize() override;

private:

};

class NoteLong : public Notes
{
public:
	NoteLong();
	~NoteLong() = default;

	static void StaticInitialize();

	enum LongNoteType {
		Start,
		Middle,
		End
	};

	void Update() override;

	void UpdateFlag() override;

	void Initialize() override;

	void SetLongNoteType(LongNoteType type) { longNoteType_ = type; }

private:

	//始点、中点、終点のいずれか
	LongNoteType longNoteType_ = Start;

	//始点をタップしたかどうか
	static bool isHitStart_;

	//終点用のフラグ
	bool onHoldNote_ = false;

};

class NoteDamage : public Notes
{
public:
	NoteDamage();
	~NoteDamage() = default;

	void Update() override;

	void UpdateFlag() override;

	void Initialize() override;

private:

};


