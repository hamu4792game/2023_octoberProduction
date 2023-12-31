#pragma once
#include"externals/imgui/imgui.h"
#include<algorithm>
#include<fstream>
#include<iostream>
#include<memory>
#include"externals/nlohmann/json.hpp"
#include"Engine/Texture/Line/Line.h"
#include"math/Matrix4x4.h"
#include"math/Vector3.h"
#include<math.h>
#define _USE_MATH_DEFINES

class MakeCatmull {
public:

	MakeCatmull();
	~MakeCatmull() = default;

	struct Sphere {
		Vector3 center;
		float radius;
		uint32_t color;
	};
	//初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw(const Matrix4x4& viewProjectionMatrix);

	int GetControlPointsNum() { return static_cast<int>(ControlPoints.size()); }

	std::vector<Vector3> GetControlPoints() { return ControlPoints; }

	Vector3 GetFirstControlPoint() { return ControlPoints[0]; }

	int GetLastLinePass() { return LastLinePass; }

private:

	std::vector<std::unique_ptr<Line>> lines_;	

	Vector3 controlPoint[4] = {
		{-8.0f,5.8f,10.0f},
		{17.6f,10.0f,-3.0f},
		{9.4f,-7.0f,23.0f},
		{-5.3f,-2.6f,-1.5f}
	};
	
	std::vector<Vector3> ControlPoints;

	enum Division{
		CUT8 = 8,
		CUT12 = 12,
		CUT16 = 16,
		CUT24 = 24,
		CUT48 = 48,
	};

	//キャトムル-ロム移動用
	//分割数
	uint32_t Linecolor = 0xffffffff;

	const int divisionNumber = 8;
	float point = 0.0f;
	float t = 0.0f;

	int chackOnlyNumber = 0;

	int linePass = 0;

	const int firstLinePass = 0;
	int LastLinePass = 2;

	bool isMove = false;

	using json = nlohmann::json;

	const std::string kDirectoryPath = "Resources/Elements/";

	const std::string kDirectoryName = "Resources/Elements";

	const std::string kItemName = "Points";

	int drawCount = 0;

	int addElementsNum = 1;

	int subtractionElementsNum = 1;

	int mode = 0;

	enum {
		First,
		Last,
	};

	//曲線一本の長さを調べるための変数群
	std::vector<float> catMullLength;

	float length;

	//1区間ごとの分割数を格納
	std::vector<int> dividingNumber;

	std::vector<int> dividingMode;

	//int dividingMode;

	void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix);

	void DrawCatmullRom(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Vector3& controlPoint3,
		const Matrix4x4& viewProjectionMatrix, uint32_t color);

	void DrawControlPoints();

	void LoadFiles();

	void LoadFile(const std::string& groupName);

	void from_json(const json& j, Vector3& v);

	void DrawImgui();

	bool LoadChackItem(const std::string& directoryPath, const std::string& itemName);

};

