#pragma once
#include "Game/Character/BaseCharacter/BaseCharacter.h"

class Hero : public BaseCharacter
{
public:
	Hero();
	~Hero() = default;

	void Initialize() override;

	void Update() override;

private:

};
