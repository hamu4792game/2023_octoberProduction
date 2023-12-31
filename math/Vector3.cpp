#include "Vector3.h"
#include <cmath>
#include <cassert>
#include <numbers>
#include <random>

Vector3::Vector3() {
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

Vector3::Vector3(const float& X, const float& Y, const float& Z) {
	this->x = X;
	this->y = Y;
	this->z = Z;
}

Vector3 Vector3::operator+(const Vector3& num) const {
	Vector3 result;
	result.x = this->x + num.x;
	result.y = this->y + num.y;
	result.z = this->z + num.z;
	return result;
}

Vector3 Vector3::operator-(const Vector3& num) const {
	Vector3 result;
	result.x = this->x - num.x;
	result.y = this->y - num.y;
	result.z = this->z - num.z;
	return result;
}

Vector3& Vector3::operator+=(const Vector3& num) {
	*this = *this + num;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& num) {
	*this = *this - num;

	return *this;
}

Vector3& Vector3::operator=(const Vector3& num)
{
	this->x = num.x;
	this->y = num.y;
	this->z = num.z;
	return *this;
}

Vector3 Vector3::operator*(const float& num) const
{
	Vector3 result;
	result.x = this->x * num;
	result.y = this->y * num;
	result.z = this->z * num;
	return result;
}

Vector3& Vector3::operator*=(const float& num)
{
	*this = *this * num;

	return *this;
}

float OuterProduct(const Vector3& vec)
{
	return sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

Vector3 Normalize(Vector3 vec3)
{
	Vector3 result;
	float w = OuterProduct(vec3);
	if (w == 0)	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}
	w = 1.0f / w;
	result.x = vec3.x * w;
	result.y = vec3.y * w;
	result.z = vec3.z * w;
	return result;
}

Vector3 AbsoluteValue(const Vector3& num)
{
	Vector3 result{};
	result.x = fabsf(num.x);
	result.y = fabsf(num.y);
	result.z = fabsf(num.z);

	return result;
}

Vector3 NormalizeRad(const Vector3& vec)
{
	Vector3 result;
	result.x = std::atan2f(vec.y, vec.z);
	result.y = std::atan2f(vec.z, vec.x);
	result.z = std::atan2f(vec.y, vec.x);
	return result;
}

float AngleToRadian(float angle)
{
	return angle * (std::numbers::pi_v<float> / 180.0f);
}

float RadianToAngle(float radian)
{
	return radian * (180.0f / std::numbers::pi_v<float>);
}

Vector3 FindVector(const Vector3& vec1, const Vector3& vec2)
{
	return vec2 - vec1;
}

// 長さ(ノルム)
float Length(const Vector3& v) {

	float length;

	length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return length;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {

	float x;

	x = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return x;
}

// 2点間の距離を求める
float Distance(const Vector3& v1, const Vector3& v2) {

	Vector3 v = v2 - v1;
	float result = Dot(v, v);

	return result;
}

Vector3 Multiply(float scalar, const Vector3& Vec) {
	Vector3 result{};
	result.x = Vec.x * scalar;
	result.y = Vec.y * scalar;
	result.z = Vec.z * scalar;
	return result;
}

Vector3 makeCatmullRom(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Vector3& controlPoint3, float t) {
	Vector3 result{};
	result = Multiply(0.5f,
		Multiply(powf(t, 3.0f), (Multiply(-1.0f, controlPoint0) + Multiply(3.0f, controlPoint1)
			+ Multiply(-3.0f, controlPoint2) + controlPoint3)) +
		Multiply(powf(t, 2.0f), (Multiply(2.0f, controlPoint0) + Multiply(-5.0f, controlPoint1)
			+ Multiply(4.0f, controlPoint2) - controlPoint3)) +
		Multiply(t, (controlPoint2 - controlPoint0)) +
		Multiply(2.0f, controlPoint1));

	return result;
}

/// <summary>
/// 指定した範囲で実数値を返す
/// </summary>
/// <param name="min">最小値</param>
/// <param name="max">最大値</param>
/// <returns>指定した範囲の乱数</returns>
int RandNum(int min, int max) {
	//	ハードウェア乱数をシードにして初期化
	std::random_device seedGen;
	//	メルセンヌツイスター法で疑似乱数生成器の作成
	std::mt19937_64 randNum(seedGen());
	//	(min,max)の範囲で等間隔に乱数を生成 一様実数分布
	std::uniform_real_distribution<> dist(min, max);

	return static_cast<int>(dist(randNum));
}
float RandNum(float min, float max) {
	//	ハードウェア乱数をシードにして初期化
	std::random_device seedGen;
	//	メルセンヌツイスター法で疑似乱数生成器の作成
	std::mt19937_64 randNum(seedGen());
	//	(min,max)の範囲で等間隔に乱数を生成 一様実数分布
	std::uniform_real_distribution<> dist(min, max);

	return static_cast<float>(dist(randNum));
}