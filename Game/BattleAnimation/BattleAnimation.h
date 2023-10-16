#include "math/Matrix4x4.h"
#include "Game/Character/Hero/Hero.h"
#include "Engine/Camera/Camera.h"

class BattleAnimation
{
public:
	BattleAnimation(std::shared_ptr<Camera> camera = nullptr);
	~BattleAnimation() = default;

	void Initialize();

	void Update();

	void Draw3D(const Matrix4x4& viewProjectionMat);

private:
	std::unique_ptr<Hero> hero_;
	std::shared_ptr<Camera> camera_;

public: // セッター
	void SetHeroModels(std::vector<std::shared_ptr<Model>> model) { hero_->SetModel(model); }

};
