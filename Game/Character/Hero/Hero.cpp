#include "Hero.h"
#include "Game/PartsEnum.h"
#include "externals/imgui/imgui.h"

void Hero::Initialize() {
	partsTransform_.resize(parts_.size());
	partsTransform_[static_cast<uint8_t>(HeroParts::Body)].parent_ = &transform_;
	partsTransform_[static_cast<uint8_t>(HeroParts::Head)].parent_ = &transform_;
	partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].parent_ = &transform_;
	/*for (int8_t i = static_cast<uint8_t>(HeroParts::Body) + 1; i < static_cast<uint8_t>(HeroParts::kMaxCount); i++) {
		partsTransform_[i].parent_ = &transform_;
	}*/

	// 親子関係
	partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::Body)];
	partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::Body)];

	partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)];

	partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::Waist)];
	partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::Waist)];

	partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)];
	partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].parent_ = &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)];

	transform_.translation_ = Vector3(7.5f, 0.0f, -45.0f);
	partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.y = 2.0f;

	partsName = {
		"Body",				// 体
		"Head",				// 頭
		"Waist",			// 腰
		"RightUpperArm",	// 右上腕
		"LeftUpperArm",		// 左上腕
		"RightBottomArm",	// 右下腕
		"LeftBottomArm",	// 左下腕
		"RightUpperLeg",	// 右上脚
		"LeftUpperLeg",		// 左上脚
		"RightBottomLeg",	// 右下脚
		"LeftBottomLeg",	// 左下脚
	};
	posName = {
		"Start",//始点
		"Middle",//中間点
		"End"//終点
	};
	/*if (LoadChackItem(kDirectoryPath, kItemName)) {
		chackOnlyNumber = 1;

		LoadFiles();
	}
	else */{
		chackOnlyNumber = 0;

		startPos.resize(partsName.size());
		for (size_t i = 0; i < partsName.size(); i++) {
			startPos[i].translation_ = partsTransform_[i].translation_;
			startPos[i].rotation_ = partsTransform_[i].rotation_;
			startPos[i].scale_ = partsTransform_[i].scale_;
		}
		middlePos.resize(startPos.size());
		EndPos.resize(startPos.size());
		
	}

	ChackFiles();
	SetStart = Position::Start;
	//LoadFiles();
	LoadFile("run");

	AnimationSpeed = 0.2f;
}

void Hero::Update() {
	DrawImgui();

	//Move(static_cast<uint32_t>(MovePattern::Run));
	if (SetStart == Position::Start) {
		for (size_t i = 0; i < partsName.size(); i++) {
			partsTransform_[i].translation_ = startPos[i].translation_;
			partsTransform_[i].rotation_ =	  startPos[i].rotation_;
			partsTransform_[i].scale_ =		  startPos[i].scale_;
		}
		//SetStart = Position::Animation;
		//isAnimation = true;

	}
	else if (SetStart == Position::Middle) {
		for (size_t i = 0; i < partsName.size(); i++) {
			partsTransform_[i].translation_ = middlePos[i].translation_;
			partsTransform_[i].rotation_ = middlePos[i].rotation_;
			partsTransform_[i].scale_ = middlePos[i].scale_;
		}
	}
	else if (SetStart == Position::End) {
		for (size_t i = 0; i < partsName.size(); i++) {
			partsTransform_[i].translation_ = EndPos[i].translation_;
			partsTransform_[i].rotation_ = EndPos[i].rotation_;
			partsTransform_[i].scale_ = EndPos[i].scale_;
		}
	}
	else if (SetStart ==Position::Animation){
		if (isAnimation){
			AnimationNum += AnimationSpeed * Magnification;
			T = Easing::EaseOutSine(AnimationNum);
			if (AnimationCount ==0){
				partsTransform_[static_cast<uint8_t>(HeroParts::Body)].translation_.y =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::Body)].translation_.y +
					T * middlePos[static_cast<uint8_t>(HeroParts::Body)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.x =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::Head)].translation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::Head)].translation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.y =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::Head)].translation_.y +
					T * middlePos[static_cast<uint8_t>(HeroParts::Head)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].translation_.y =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::Waist)].translation_.y +
					T * middlePos[static_cast<uint8_t>(HeroParts::Waist)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Body)].rotation_.y =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::Body)].rotation_.y +
					T * middlePos[static_cast<uint8_t>(HeroParts::Body)].rotation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y +
					T * middlePos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z +
					T * middlePos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z =
					(1 - T) * startPos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z +
					T * middlePos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z;

			}
			else if (AnimationCount == 1) {
				partsTransform_[static_cast<uint8_t>(HeroParts::Body)].translation_.y =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::Body)].translation_.y +
					T * EndPos[static_cast<uint8_t>(HeroParts::Body)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::Head)].translation_.x +
					T * EndPos[static_cast<uint8_t>(HeroParts::Head)].translation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.y =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::Head)].translation_.y +
					T * EndPos[static_cast<uint8_t>(HeroParts::Head)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].translation_.y =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::Waist)].translation_.y +
					T * EndPos[static_cast<uint8_t>(HeroParts::Waist)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Body)].rotation_.y =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::Body)].rotation_.y +
					T * EndPos[static_cast<uint8_t>(HeroParts::Body)].rotation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x +
					T * EndPos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x +
					T * EndPos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x +
					T * EndPos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x +
					T * EndPos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x +
					T * EndPos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y +
					T * EndPos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z +
					T * EndPos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z +
					T * EndPos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z;

			}
			else if (AnimationCount == 2) {
				partsTransform_[static_cast<uint8_t>(HeroParts::Body)].translation_.y =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::Body)].translation_.y +
					T * middlePos[static_cast<uint8_t>(HeroParts::Body)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.x =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::Head)].translation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::Head)].translation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.y =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::Head)].translation_.y +
					T * middlePos[static_cast<uint8_t>(HeroParts::Head)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].translation_.y =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::Waist)].translation_.y +
					T * middlePos[static_cast<uint8_t>(HeroParts::Waist)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Body)].rotation_.y =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::Body)].rotation_.y +
					T * middlePos[static_cast<uint8_t>(HeroParts::Body)].rotation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x +
					T * middlePos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y +
					T * middlePos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z +
					T * middlePos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z =
					(1 - T) * EndPos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z +
					T * middlePos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z;
			}
			else if (AnimationCount == 3) {
				partsTransform_[static_cast<uint8_t>(HeroParts::Body)].translation_.y =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::Body)].translation_.y +
					T * startPos[static_cast<uint8_t>(HeroParts::Body)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::Head)].translation_.x +
					T * startPos[static_cast<uint8_t>(HeroParts::Head)].translation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.y =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::Head)].translation_.y +
					T * startPos[static_cast<uint8_t>(HeroParts::Head)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].translation_.y =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::Waist)].translation_.y +
					T * startPos[static_cast<uint8_t>(HeroParts::Waist)].translation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::Body)].rotation_.y =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::Body)].rotation_.y +
					T * startPos[static_cast<uint8_t>(HeroParts::Body)].rotation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x +
					T * startPos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x +
					T * startPos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x +
					T * startPos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x +
					T * startPos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x +
					T * startPos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y +
					T * startPos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.y;

				partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z +
					T * startPos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.z;

				partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z =
					(1 - T) * middlePos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z +
					T * startPos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.z;

			}


			if (AnimationNum > 1.00f) {
				AnimationCount++;
				AnimationNum = 0.0f;
			}
			if (AnimationCount>3){
				AnimationCount = 0;
			}

		}

	}
	
	

	//	座標更新
	BaseCharacter::Update();
	/*transform_.UpdateMatrix();
	for (uint8_t i = 0; i < partsTransform_.size(); i++) {
		partsTransform_[i].UpdateMatrix();
	}*/
}

void Hero::Move(const int& pattern) {
	switch (pattern) {
	case static_cast<uint32_t>(MovePattern::Run):
		transform_.translation_.z += 1.0f;
		break;
	case static_cast<uint32_t>(MovePattern::Stop):
		transform_.translation_.y -= 1.0f;
		break;

	}
}

void Hero::DrawImgui(){
	ImGui::Begin("Animation",nullptr,ImGuiWindowFlags_MenuBar);
	/*if (ImGui::Button("保存")) {
		std::string itemNameString = ItemName_;
		if (!itemNameString.empty()) {
			SaveFile(std::string() + ItemName_);
		}
	}*/

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("ファイル保存")){
			ImGui::InputText("ファイル自体の名前", ItemName_, sizeof(ItemName_));
			if (ImGui::MenuItem("保存")) {
				std::string itemNameString = ItemName_;
				if (!itemNameString.empty()) {
					SaveFile(std::string() + ItemName_);
				}
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("ファイル読み込み")) {
			
			ImGui::Text(("選択しているファイル  " + Name_).c_str());
			if (ImGui::MenuItem("ロード")) {
				if (!Name_.empty()) {
					LoadFile(Name_);
					for (size_t i = 0; i < partsName.size(); i++) {
						partsTransform_[i].translation_ = startPos[i].translation_;
						partsTransform_[i].rotation_ = startPos[i].rotation_;
						partsTransform_[i].scale_ = startPos[i].scale_;
					}
				}

			}
			if (ImGui::TreeNode("読み込むファイル")) {
				for (size_t i = 0; i < fileName.size(); i++) {
					if (ImGui::RadioButton(fileName[i].c_str(), &fileNumber, static_cast<int>(i))) {
						Name_ = fileName[fileNumber].c_str();
					}
					ImGui::SameLine();
				}
				ImGui::TreePop();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("ファイル一覧")) {
			for (size_t i = 0; i < fileName.size(); ++i) {
				ImGui::Text(fileName[i].c_str());
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("位置確認用SRT切り替え")) {
			ImGui::RadioButton("始点", &SetStart, Position::Start);		
			ImGui::RadioButton("中間点", &SetStart, Position::Middle);
			ImGui::RadioButton("終点", &SetStart, Position::End);
			ImGui::RadioButton("アニメーション", &SetStart, Position::Animation);
			if (SetStart == Position::Animation) {
				ImGui::Checkbox("アニメーション開始", &isAnimation);
				if (ImGui::Button("アニメーションリセット")) {
					T = 0;
					AnimationNum = 0.0f;
					for (size_t i = 0; i < partsName.size(); i++) {
						partsTransform_[i].translation_ = startPos[i].translation_;
						partsTransform_[i].rotation_ = startPos[i].rotation_;
						partsTransform_[i].scale_ = startPos[i].scale_;
					}
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("位置確認用SRT切り替え")) {
			ImGui::DragFloat("腕の振りの速さ", &AnimationSpeed, 0.001f, 0.001f, 1.0f, "%.3f");
			ImGui::EndMenu();
		}


		ImGui::EndMenuBar();
	}



	if (ImGui::TreeNode("Body")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::Body)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::Body)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::Body)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::Body)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::Body)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::Body)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::Body)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::Body)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::Body)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Head")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::Head)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::Head)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::Head)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::Head)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::Head)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::Head)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::Head)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::Head)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::Head)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Waist")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::Waist)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::Waist)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::Waist)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::Waist)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::Waist)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::Waist)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::Waist)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::Waist)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::Waist)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("RightUpperArm")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::RightUpperArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::RightUpperArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::RightUpperArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::RightUpperArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::RightUpperArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::RightUpperArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("LeftUpperArm")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("RightBottomArm")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::RightBottomArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::RightBottomArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::RightBottomArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::RightBottomArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::RightBottomArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::RightBottomArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("LeftBottomArm")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("RightUpperLeg")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("LeftUpperLeg")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("RightBottomLeg")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("LeftBottomLeg")) {
		if (ImGui::TreeNode("Start")) {
			ImGui::DragFloat3("Translate", &startPos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &startPos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &startPos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Middle")) {
			ImGui::DragFloat3("Translate", &middlePos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &middlePos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &middlePos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("End")) {
			ImGui::DragFloat3("Translate", &EndPos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].translation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &EndPos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].scale_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &EndPos[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x, AngleToRadian(1.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	

	ImGui::End();

#pragma region Parts
	//ImGui::Begin("Hero");
	//if (ImGui::TreeNode("Transform")) {
	//	ImGui::DragFloat3("Translate", &transform_.translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &transform_.scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &transform_.rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("Body")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::Body)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::Body)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::Body)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("Head")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::Head)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::Head)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::Head)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("Waist")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::Waist)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("RightUpperArm")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperArm)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("LeftUpperArm")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperArm)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("RightBottomArm")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomArm)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("LeftBottomArm")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomArm)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("RightUpperLeg")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightUpperLeg)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("LeftUpperLeg")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("RightBottomLeg")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::RightBottomLeg)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("LeftBottomLeg")) {
	//	ImGui::DragFloat3("Translate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].translation_.x, 0.1f);
	//	ImGui::DragFloat3("Scale", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].scale_.x, 0.1f);
	//	ImGui::DragFloat3("Rotate", &partsTransform_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)].rotation_.x, AngleToRadian(1.0f));
	//	ImGui::TreePop();
	//}

	//ImGui::End();


#pragma endregion


	ImGui::Begin("確認");
	ImGui::Text("ファイル読み込み出来たかどうか = %d", chackOnlyNumber);
	ImGui::Text("腕のイージング媒介変数の値 = %.1f", T);
	ImGui::Text("moveArmの合計 = %.2f", AnimationNum);
	ImGui::Text("中に入っている要素数 = %d", fileName.size());
	ImGui::Text("中に入っている要素数 = %d", startPos.size());
	ImGui::Text("中に入っている要素数 = %d", middlePos.size());
	ImGui::Text("中に入っている要素数 = %d", EndPos.size());
	ImGui::Text("アニメーションカウント = %d", AnimationCount);
	ImGui::End();
}

void Hero::SetPosition(const Vector3& position) {
	transform_.translation_ = position;
}

void Hero::SaveFile(const std::string& kItemName) {
	//保存
	json root;
	root = json::object();
	root[kItemName] = json::object();


	for (size_t i = 0; i < partsName.size(); ++i) {
		root[kItemName][partsName[i].c_str()] = json::object();
		for (size_t j = 0; j < posName.size(); ++j) {

			if (j == 0) {
				root[kItemName][partsName[i].c_str()][posName[j].c_str()][0] =
					json::array({ startPos[i].translation_.x,startPos[i].translation_.y, startPos[i].translation_.z });

				root[kItemName][partsName[i].c_str()][posName[j].c_str()][1] =
					json::array({ startPos[i].rotation_.x,startPos[i].rotation_.y, startPos[i].rotation_.z });

				root[kItemName][partsName[i].c_str()][posName[j].c_str()][2] =
					json::array({ startPos[i].scale_.x,startPos[i].scale_.y, startPos[i].scale_.z });
			}

			else if (j == 1) {
				root[kItemName][partsName[i].c_str()][posName[j].c_str()][0] =
					json::array({ middlePos[i].translation_.x,middlePos[i].translation_.y, middlePos[i].translation_.z });

				root[kItemName][partsName[i].c_str()][posName[j].c_str()][1] =
					json::array({ middlePos[i].rotation_.x,middlePos[i].rotation_.y, middlePos[i].rotation_.z });

				root[kItemName][partsName[i].c_str()][posName[j].c_str()][2] =
					json::array({ middlePos[i].scale_.x,middlePos[i].scale_.y, middlePos[i].scale_.z });
			}

			else if (j == 2) {
				root[kItemName][partsName[i].c_str()][posName[j].c_str()][0] =
					json::array({ EndPos[i].translation_.x,EndPos[i].translation_.y, EndPos[i].translation_.z });

				root[kItemName][partsName[i].c_str()][posName[j].c_str()][1] =
					json::array({ EndPos[i].rotation_.x,EndPos[i].rotation_.y, EndPos[i].rotation_.z });

				root[kItemName][partsName[i].c_str()][posName[j].c_str()][2] =
					json::array({ EndPos[i].scale_.x,EndPos[i].scale_.y, EndPos[i].scale_.z });
			}


			std::filesystem::path dir(kDirectoryPath);
			if (!std::filesystem::exists(kDirectoryName)) {
				std::filesystem::create_directory(kDirectoryName);
			}
			// 書き込むjsonファイルのフルパスを合成する
			std::string filePath = kDirectoryPath + kItemName + ".json";
			// 書き込み用ファイルストリーム
			std::ofstream ofs;
			// ファイルを書き込みように開く
			ofs.open(filePath);
			//ファイルオープン失敗
			if (ofs.fail()) {
				std::string message = "Failed open data file for write.";
				MessageBoxA(nullptr, message.c_str(), "Element", 0);
				assert(0);
				break;
			}
			//ファイルにjson文字列を書き込む(インデント幅4)
			ofs << std::setw(6) << root << std::endl;
			//ファイルを閉じる
			ofs.close();
		}
	}
	if (fileName.size() != 0) {
		bool noneFail = true;
		for (size_t i = 0; i < fileName.size(); i++) {
			if (fileName[i].c_str() == kItemName) {
				noneFail = false;
			}
		}
		if (noneFail) {
			fileName.push_back(kItemName);
		}

	}
	else {
		//ファイルの名前を取得
		fileName.push_back(kItemName);
	}

	std::string message = std::format("{}.json saved.", kItemName);
	MessageBoxA(nullptr, message.c_str(), "Element", 0);
}

void Hero::ChackFiles(){
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Element", 0);
		assert(0);
		return;
	}

	std::filesystem::directory_iterator dir_it(kDirectoryPath);

	for (const std::filesystem::directory_entry& entry : dir_it) {
		//ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		//ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		//.jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		if (LoadChackItem(kDirectoryPath, filePath.stem().string())) {
			chackOnlyNumber = 1;
		}

		if (fileName.size() != 0) {
			bool noneFail = true;
			for (size_t i = 0; i < fileName.size(); i++){
				if (fileName[i].c_str() == filePath.stem().string()) {
					noneFail = false;
				}
			}
			if (noneFail){
				fileName.push_back(filePath.stem().string());
			}

		}
		else {
			//ファイルの名前を取得
			fileName.push_back(filePath.stem().string());
		}
	}
}

void Hero::LoadFiles() {
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Animetion", 0);
		assert(0);
		return;
	}

	std::filesystem::directory_iterator dir_it(kDirectoryPath);

	for (const std::filesystem::directory_entry& entry : dir_it) {
		//ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		//ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		//.jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		//ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}

void Hero::LoadFile(const std::string& groupName) {
	//読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//読み込み用のファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Adjustment_Item", 0);
		assert(0);
		return;
	}
	nlohmann::json root;

	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();
	//グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);
	//未登録チェック
	assert(itGroup != root.end());

	//各アイテムについて
	for (size_t i = 0; i < partsName.size(); ++i) {
		int count = 0;
		HeroTransform baseTrans{};
		for (const auto& root_ : root[groupName][partsName[i].c_str()][posName[0].c_str()]) {
			Vector3 v{};
			from_json(root_, v);
			if (count == 0) {
				baseTrans.translation_ = v;
			}else if (count == 1) {
				baseTrans.rotation_ = v;
			}else if (count == 2) {
				baseTrans.scale_ = v;
			}
			count++;
		}
		startPos[i] = baseTrans;

		count = 0;
		HeroTransform baseTrans2{};
		for (const auto& root_ : root[groupName][partsName[i].c_str()][posName[1].c_str()]) {
			Vector3 v{};
			from_json(root_, v);
			if (count == 0) {
				baseTrans2.translation_ = v;
			}
			else if (count == 1) {
				baseTrans2.rotation_ = v;
			}
			else if (count == 2) {
				baseTrans2.scale_ = v;
			}
			count++;
		}
		middlePos[i] = baseTrans2;

		count = 0;
		HeroTransform baseTrans3{};
		for (const auto& root_ : root[groupName][partsName[i].c_str()][posName[2].c_str()]) {
			Vector3 v{};
			from_json(root_, v);
			if (count == 0) {
				baseTrans3.translation_ = v;
			}
			else if (count == 1) {
				baseTrans3.rotation_ = v;
			}
			else if (count == 2) {
				baseTrans3.scale_ = v;
			}
			count++;
		}
		EndPos[i] = baseTrans3;

	}
#ifdef DEBUG
	/*std::string message = std::format("{}.json Loading successful.", groupName);
	MessageBoxA(nullptr, message.c_str(), "Animetion", 0);*/
#endif // DEBUG

	
}

void Hero::from_json(const json& j, Vector3& v) {
	v.x = j.at(0).get<float>();
	v.y = j.at(1).get<float>();
	v.z = j.at(2).get<float>();
}

bool Hero::LoadChackItem(const std::string& directoryPath, const std::string& itemName) {
	// 書き込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + itemName + ".json";
	//読み込み用のファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Adjustment_Item", 0);
		ifs.close();
		return false;
	}
	nlohmann::json root;

	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();
	//グループを検索
	nlohmann::json::iterator itGroup = root.find(itemName);
	//未登録チェック
	if (itGroup != root.end()) {
		return true;
	}
	else {
		return false;
	}
}