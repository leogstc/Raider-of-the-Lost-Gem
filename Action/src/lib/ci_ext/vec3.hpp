//--------------------------------------------------------------------------------------------
//  作成者 : 植山沙欧
//
//  更新履歴  ：2014.09.30  Ver1.00  植山沙欧  基本的な機能の実装
//              2015.05.28  Ver1.01  植山沙欧  長さ、内積、2ベクトルの角度を求める関数を作成した
//--------------------------------------------------------------------------------------------
#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <Windows.h>
#include <vector>
#include <d3dx9.h>
#include "math.h"
namespace ci_ext
{

template<typename T = float>
class Vec3
{
  T x_, y_, z_;
public:
  operator D3DXVECTOR3() { return D3DXVECTOR3(x_, y_, z_); }
  void operator=(const D3DXVECTOR3& d)
  {
    x(d.x);
    y(d.y);
    z(d.z);
  }
  Vec3(const D3DXVECTOR3& d)
    :
    x_(d.x),
    y_(d.y),
    z_(d.z)
  {}
  operator POINT()
  {
    POINT p = { static_cast<long>(x_), static_cast<long>(y_) };
    return p;
  }

  static Vec3<T> one()
  {
    return Vec3<T>(1,1,1);
  }
  static Vec3<T> zero()
  {
    return Vec3<T>(0, 0, 0);
  }
  void x(T x) { x_ = x; }
  void y(T y) { y_ = y; }
  void z(T z) { z_ = z; }
  Vec3(T x = 0.0f, T y = 0.0f, T z = 0.0f) :
    x_(x),
    y_(y),
    z_(z) {}
  Vec3(const Vec3<T>& v) :
    x_(v.x()),
    y_(v.y()),
    z_(v.z()) {}
	Vec3(const std::vector<float>& v)
	{
		set(v);
	}
	~Vec3() {}
  T x() const { return x_; }
  T y() const { return y_; }
  T z() const { return z_; }
  int ix() const { return static_cast<int>(x_); }
  int iy() const { return static_cast<int>(y_); }
  int iz() const { return static_cast<int>(z_); }

  Vec3<T>& offset(T xx, T yy, T zz = 0.0f)
  {
    x(x() + xx);
    y(y() + yy);
    z(z() + zz);
    return *this;
  }

  Vec3<T>& offset(const Vec3<T>& p)
  {
    x(x() + p.x());
    y(y() + p.y());
    z(z() + p.z());
    return *this;
  }

  Vec3<T>& set(const Vec3<T>& p)
  {
    x(p.x());
    y(p.y());
    z(p.z());
    return *this;
  }
  Vec3<T>& set(T xx = 0.0f, T yy = 0.0f, T zz = 0.0f)
  {
    x(xx);
    y(yy);
    z(zz);
    return *this;
  }

	Vec3<T>& set(const std::vector<float>& v)
	{
		x_ = y_ = z_ = T(0);
		if (v.size() >= T(1))
			x_ = v[0];
		if (v.size() >= T(2))
			y_ = v[1];
		if (v.size() >= T(3))
			z_ = v[2];
		return *this;
	}

  Vec3<T>& operator=(const Vec3<T>& v)
  {
    set(v);
    return *this;
  }

  Vec3<T>& operator=(const POINT& p)
  {
    set(static_cast<T>(p.x), static_cast<T>(p.y));
    return *this;
  }

  Vec3<T>& operator+=(const Vec3<T>& p)
  {
    x(x() + p.x());
    y(y() + p.y());
    z(z() + p.z());
    return *this;
  }

  Vec3<T> operator+(const Vec3<T>& p) const
  {
    return Vec3<T>(x() + p.x(), y() + p.y(), z() + p.z());
  }

  Vec3<T>& operator-=(const Vec3<T>& p)
  {
    x(x() - p.x());
    y(y() - p.y());
    z(z() - p.z());
    return *this;
  }

  Vec3<T> operator-(const Vec3<T>& p) const
  {
    return Vec3<T>(x() - p.x(), y() - p.y(), z() - p.z());
  }
  bool operator==(const Vec3<T>& p)
  {
    return (x() == p.x() && y() == p.y() && z() == p.z());
  }
  bool operator==(T c)
  {
    return (x() == c && y() == c && z() == c);
  }
  Vec3<T>& translate(const Vec3<T>& translate)
  {
    x( x() + translate.x());
    y( y() + translate.y());
    return *this;
  }
  Vec3<T>& rotate(T radian)
  {
    T xx = x();
    T yy = y();
    x( xx * std::cos(-radian) - yy * std::sin(-radian));
    y( xx * std::sin(-radian) + yy * std::cos(-radian));
    return *this;
  }
  Vec3<T>& rotateDegree(T degree)
  {
    rotate(DegreeToRadian(degree));
    return *this;
  }
  Vec3<T>& scale(const Vec3<T>& scale)
  {
    x(x() * scale.x());
    y(y() * scale.y());
    z(z() * scale.z());
    return *this;
  }

  float length() const
  {
    return pow((x() * x()) + (y() * y()) + (z() * z()), 0.5f);
  }

  float dotProduct(const Vec3<T>& v) const
  {
    return x() * v.x() + y() * v.y() + z() * v.z();
  }

  //2ベクトル間の角をラジアン値で求める
  float radianOf2Vector(const Vec3<T>& v) const
  {
    //内積とベクトル長さを使ってcosθを求める
    float cos_sita = dotProduct(v) / (length() * v.length());

    return acos(cos_sita);
  }

  //2ベクトル間の角をディグリ値で求める
  float degreeOf2Vector(const Vec3<T>& v)
  {
    return radianOf2Vector(v) * static_cast<T>(180.0 / M_PI);
  }

//関数
  //内積を求める
  static float dotProduct(const Vec3<T>& v1, const Vec3<T>& v2)
  {
    return v1.dotProduct(v2);
  }

  //2ベクトル間の角をラジアン値で求める
  static float radianOf2Vector(const Vec3<T>& v1, const Vec3<T>& v2)
  {
    return v1.radianOf2Vector(v2);
  }

  //2ベクトル間の角をディグリ値で求める
  static float degreeOf2Vector(const Vec3<T>& v1, const Vec3<T>& v2)
  {
    return radianOf2Vector(v1, v2) * static_cast<T>(180.0 / M_PI);
  }

  static Vec3<T>* null() { return static_cast<Vec3<T>*>(nullptr); }
};

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;

}