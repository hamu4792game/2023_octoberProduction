#include "math/Matrix4x4.h"
#include "Game/Character/Hero/Hero.h"
#include "Game/Character/Boss/Boss.h"
#include "Engine/Camera/Camera.h"
#include "Game/Stage/Stage.h"
#include <list>

class BattleAnimation
{
public:
	BattleAnimation(std::shared_ptr<Camera> camera = nullptr);
	~BattleAnimation() = default;

	void Initialize();

	void Update();

	void Draw3D(const Matrix4x4& viewProjectionMat);

	Hero* GetHero() { return hero_.get(); }

	Boss* GetBoss() { return boss_.get(); }

private:
	std::unique_ptr<Hero> hero_;
	std::unique_ptr<Boss> boss_;
	std::shared_ptr<Camera> camera_;
	std::list<std::unique_ptr<Stage>> stage_;
	std::vector<std::shared_ptr<Model>> stageModel_;

private:
	//	ステージ用カウント変数
	uint32_t stageCount = 0u;

private: // メンバ関数
	

public: // セッター
	void SetHeroModels(std::vector<std::shared_ptr<Model>> model) { hero_->SetModel(model); }
	void SetBossModels(std::vector<std::shared_ptr<Model>> model) { boss_->SetModel(model); }
	void SetStageModels(std::vector<std::shared_ptr<Model>> model) { stageModel_ = model; }
	void SetBossBulletModel(Model* model) { boss_->SetBulletModel(model); }
	
public:
	const WorldTransform& GetHeroTransform() { return hero_->GetTransform(); }

};
