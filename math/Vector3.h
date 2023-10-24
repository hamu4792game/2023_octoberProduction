#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
///
class Vector3 final {
public:
	Vector3();
	Vector3(const float& X, const float& Y, const float& Z);

public:
	float x;
	float y;
	float z;

public:
	Vector3 operator+(const Vector3& num) const;
	Vector3 operator-(const Vector3& num) const;
	Vector3& operator+=(const Vector3& num);
	Vector3& operator-=(const Vector3& num);
	Vector3& operator=(const Vector3& num);

	Vector3 operator*(const float& num) const;
	Vector3& operator*=(const float& num);
};

//	外積を求める
float OuterProduct(const Vector3& vec);
//	正規化
Vector3 Normalize(Vector3 vec3);
//	絶対値を求める
Vector3 AbsoluteValue(const Vector3& num);

Vector3 NormalizeRad(const Vector3& vec);

//	弧度法から度数法へ変換する
float AngleToRadian(float angle);

//	度数法から弧度法へ変換する
float RadianToAngle(float radian);

/// <summary>
/// vec1からvec2のベクトルを求める
/// </summary>
/// <param name="vec1">引くベクトル</param>
/// <param name="vec2">引かれるベクトル</param>
/// <returns>正規化されていないベクトル</returns>
Vector3 FindVector(const Vector3& vec1, const Vector3& vec2);

//長さ
float Length(const Vector3& v);

//内積
float Dot(const Vector3& v1, const Vector3& v2);

// 2点間の距離を求める関数
float Distance(const Vector3& v1, const Vector3& v2);

// スカラー倍
//makeCatmullにて使用しているため追加
Vector3 Multiply(float scalar, const Vector3& Vec);

Vector3 makeCatmullRom(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Vector3& controlPoint3, float t);

int RandNum(int min, int max);
float RandNum(float min, float max);

