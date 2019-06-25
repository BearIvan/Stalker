#pragma once
#undef min
#undef max
IC bool _valid(const float x)
{
	// check for: Signaling NaN, Quiet NaN, Negative infinity ( –INF), Positive infinity (+INF), Negative denormalized, Positive denormalized
	int cls = _fpclass(double(x));
	if (cls&(_FPCLASS_SNAN + _FPCLASS_QNAN + _FPCLASS_NINF + _FPCLASS_PINF + _FPCLASS_ND + _FPCLASS_PD))
		return false;

	/* *****other cases are*****
	_FPCLASS_NN Negative normalized non-zero
	_FPCLASS_NZ Negative zero ( – 0)
	_FPCLASS_PZ Positive 0 (+0)
	_FPCLASS_PN Positive normalized non-zero
	*/
	return true;
}
class  XrMath
{

public:
	static constexpr  float EPS_S = 0.0000001f;
	static constexpr  float EPS = 0.0000100f;
	static constexpr  float EPS_L = 0.0010000f;
	static constexpr  float M_SQRT1_2 = 0.7071067811865475244008443621048f;
	static constexpr  float M_PI = 3.1415926535897932384626433832795f;
	static constexpr  float PI_MUL_2 = 6.2831853071795864769252867665590f;
	static constexpr  float PI_MUL_3 = 9.4247779607693797153879301498385f;
	static constexpr  float PI_MUL_4 = 12.566370614359172953850573533118f;
	static constexpr  float PI_MUL_6 = 18.849555921538759430775860299677f;
	static constexpr  float PI_MUL_8 = 25.132741228718345907701147066236f;
	static constexpr  float PI_DIV_2 = 1.5707963267948966192313216916398f;
	static constexpr  float PI_DIV_3 = 1.0471975511965977461542144610932f;
	static 	constexpr  float PI_DIV_4 = 0.7853981633974483096156608458199f;
	static	constexpr  float PI_DIV_6 = 0.5235987755982988730771072305466f;
	static constexpr  float PI_DIV_8 = 0.3926990816987241548078304229099f;


	static IC int iFloor(float a) { return static_cast<int>(a); }
	static IC int iCeil(float a) { return static_cast<int>(ceilf(a)); }
	template <class T> IC  static T min(T a, T b) { return a < b ? a : b; }
	template <class T> IC static T max(T a, T b) { return a > b ? a : b; }
	template <class T> IC static T sqr(T a) { return a * a; }
	template<class T> IC static T abs(T a) { if (a < 0) return -a; return a; }
	// float
	template<>  static IC float abs<float>(float x) { return fabsf(x); }
	static IC float sqrt(float x) { return sqrtf(x); }
	static IC float sin(float x) { return sinf(x); }
	static IC float cos(float x) { return cosf(x); }

	template<> static IC double abs<double>(double x) { return ::abs (x); }
	static IC double sqrt(double x) { return ::sqrt(x); }
	static IC double sin(double x) { return ::sin(x); }
	static IC double cos(double x) { return ::cos(x); }

	static	IC BOOL fsimilar(float a, float b, float cmp = EPS) { return abs(a - b) < cmp; }
	static IC BOOL dsimilar(double a, double b, double cmp = EPS) { return abs(a - b) < cmp; }


	template<class T>
	static IC T rsqrt(T v) { return static_cast<T>(1.0) / sqrt(v); }
	
	
	static IC BOOL fis_zero(float val, float cmp = EPS_S) { return abs(val) < cmp; }
	static IC BOOL dis_zero(double val, double cmp = EPS_S) { return abs(val) < cmp; }
public:
	template <class T> IC static T deg2rad(T val) { return (val*T(M_PI) / T(180)); };
	template <class T> IC static T rad2deg(T val) { return (val*T(180) / T(M_PI)); };
	// clamping/snapping
	template <class T>
	static IC void clamp(T& val, const T& _low, const T& _high)
	{
		if (val < _low) val = _low;
		else if (val > _high) val = _high;
	};
	template <class T>
	static IC T clampr(const T& val, const T& _low, const T& _high)
	{
		if (val < _low) return _low;
		else if (val > _high) return _high;
		else return val;
	};
	static	IC float snapto(float value, float snap)
	{
		if (snap <= 0.f) return value;
		return float(iFloor((value + (snap*0.5f)) / snap)) * snap;
	}


	// normalize angle (0..2PI)
	static IC float angle_normalize_always(float a)
	{
		float div = a / PI_MUL_2;
		int rnd = (div > 0) ? iFloor(div) : iCeil(div);
		float frac = div - rnd;
		if (frac < 0) frac += 1.f;
		return frac * PI_MUL_2;
	}

	// normalize angle (0..2PI)
	static IC float angle_normalize(float a)
	{
		if (a >= 0 && a <= PI_MUL_2) return a;
		else return angle_normalize_always(a);
	}

	// -M_PI .. +M_PI
	static IC float angle_normalize_signed(float a)
	{
		if (a >= (-M_PI) && a <= M_PI) return a;
		float angle = angle_normalize_always(a);
		if (angle > M_PI) angle -= PI_MUL_2;
		return angle;
	}

	// -M_PI..M_PI
	static IC float angle_difference_signed(float a, float b)
	{
		float diff = angle_normalize_signed(a) - angle_normalize_signed(b);
		if (diff > 0)
		{
			if (diff > M_PI)
				diff -= PI_MUL_2;
		}
		else
		{
			if (diff < -M_PI)
				diff += PI_MUL_2;
		}
		return diff;
	}

	// 0..M_PI
	static IC float angle_difference(float a, float b)
	{
		return abs(angle_difference_signed(a, b));
	}

	static IC bool are_ordered(float const value0, float const value1, float const value2)
	{
		if ((value1 >= value0) && (value1 <= value2))
			return true;

		if ((value1 <= value0) && (value1 >= value2))
			return true;

		return false;
	}

	static IC bool is_between(float const value, float const left, float const right)
	{
		return are_ordered(left, value, right);
	}

	// c=current, t=target, s=speed, dt=dt
	static IC bool angle_lerp(float& c, float t, float s, float dt)
	{
		float const before = c;
		float diff = t - c;
		if (diff > 0)
		{
			if (diff > M_PI)
				diff -= PI_MUL_2;
		}
		else
		{
			if (diff < -M_PI)
				diff += PI_MUL_2;
		}
		float diff_a = abs(diff);

		if (diff_a < EPS_S)
			return true;

		float mot = s * dt;
		if (mot > diff_a) mot = diff_a;
		c += (diff / diff_a)*mot;

		if (is_between(c, before, t))
			return false;

		if (c < 0)
			c += PI_MUL_2;
		else if (c > PI_MUL_2)
			c -= PI_MUL_2;

		return false;
	}

	// Just lerp :) expects normalized angles in range [0..2PI)
	static IC float angle_lerp(float A, float B, float f)
	{
		float diff = B - A;
		if (diff > M_PI) diff -= PI_MUL_2;
		else if (diff < -M_PI) diff += PI_MUL_2;

		return A + diff * f;
	}

	static IC float angle_inertion(float src, float tgt, float speed, float clmp, float dt)
	{
		float a = angle_normalize_signed(tgt);
		angle_lerp(src, a, speed, dt);
		src = angle_normalize_signed(src);
		float dH = angle_difference_signed(src, a);
		float dCH = clampr(dH, -clmp, clmp);
		src -= dH - dCH;
		return src;
	}

	static IC float angle_inertion_var(float src, float tgt, float min_speed, float max_speed, float clmp, float dt)
	{
		tgt = angle_normalize_signed(tgt);
		src = angle_normalize_signed(src);
		float speed = abs((max_speed - min_speed)*angle_difference(tgt, src) / clmp) + min_speed;
		angle_lerp(src, tgt, speed, dt);
		src = angle_normalize_signed(src);
		float dH = angle_difference_signed(src, tgt);
		float dCH = clampr(dH, -clmp, clmp);
		src -= dH - dCH;
		return src;
	}
};

#pragma pack(push)
#pragma pack(1)
template <class T> struct _quaternion;
#include "Math/XrVector3d.h"
#include "Math/XrVector2.h"
#include "Math/XrVector4.h"
#include "Math/XrMatrix.h"
#include "Math/XrMatrix33.h"
#include "Math/XrQuaternion.h"
#include "Math/XrRect.h"
#include "Math/XrFbox.h"
#include "Math/XrFbox2.h"
#include "Math/XrObb.h"
#include "Math/XrSphere.h"
#include "Math/XrCylinder.h"
#include "Math/XrPlane.h"
#include "Math/XrPlane2.h"

template <class T>
IC _matrix<T>& _matrix<T>::rotation(const _quaternion<T>& Q)
{
	T xx = Q.x*Q.x;
	T yy = Q.y*Q.y;
	T zz = Q.z*Q.z;
	T xy = Q.x*Q.y;
	T xz = Q.x*Q.z;
	T yz = Q.y*Q.z;
	T wx = Q.w*Q.x;
	T wy = Q.w*Q.y;
	T wz = Q.w*Q.z;

	_11 = 1 - 2 * (yy + zz);
	_12 = 2 * (xy - wz);
	_13 = 2 * (xz + wy);
	_14 = 0;
	_21 = 2 * (xy + wz);
	_22 = 1 - 2 * (xx + zz);
	_23 = 2 * (yz - wx);
	_24 = 0;
	_31 = 2 * (xz - wy);
	_32 = 2 * (yz + wx);
	_33 = 1 - 2 * (xx + yy);
	_34 = 0;
	_41 = 0;
	_42 = 0;
	_43 = 0;
	_44 = 1;
	return *this;
}

template <class T>
IC _matrix<T>& _matrix<T>::mk_xform(const _quaternion<T>& Q, const Tvector& V)
{
	T xx = Q.x*Q.x;
	T yy = Q.y*Q.y;
	T zz = Q.z*Q.z;
	T xy = Q.x*Q.y;
	T xz = Q.x*Q.z;
	T yz = Q.y*Q.z;
	T wx = Q.w*Q.x;
	T wy = Q.w*Q.y;
	T wz = Q.w*Q.z;

	_11 = 1 - 2 * (yy + zz);
	_12 = 2 * (xy - wz);
	_13 = 2 * (xz + wy);
	_14 = 0;
	_21 = 2 * (xy + wz);
	_22 = 1 - 2 * (xx + zz);
	_23 = 2 * (yz - wx);
	_24 = 0;
	_31 = 2 * (xz - wy);
	_32 = 2 * (yz + wx);
	_33 = 1 - 2 * (xx + yy);
	_34 = 0;
	_41 = V.x;
	_42 = V.y;
	_43 = V.z;
	_44 = 1;
	return *this;
}

#define TRACE_QZERO_TOLERANCE 0.1f
template <class T>
IC _quaternion<T>& _quaternion<T>::set(const _matrix<T>& M)
{
	float trace, s;

	trace = M._11 + M._22 + M._33;
	if (trace > 0.0f)
	{
		s = XrMath::sqrt(trace + 1.0f);
		w = s * 0.5f;
		s = 0.5f / s;

		x = (M._32 - M._23) * s;
		y = (M._13 - M._31) * s;
		z = (M._21 - M._12) * s;
	}
	else
	{
		int biggest;
		enum { A, E, I };
		if (M._11 > M._22)
		{
			if (M._33 > M._11)
				biggest = I;
			else
				biggest = A;
		}
		else
		{
			if (M._33 > M._11)
				biggest = I;
			else
				biggest = E;
		}

		// in the unusual case the original trace fails to produce a good sqrt, try others...
		switch (biggest)
		{
		case A:
			s = XrMath::sqrt(M._11 - (M._22 + M._33) + 1.0f);
			if (s > TRACE_QZERO_TOLERANCE)
			{
				x = s * 0.5f;
				s = 0.5f / s;
				w = (M._32 - M._23) * s;
				y = (M._12 + M._21) * s;
				z = (M._13 + M._31) * s;
				break;
			}
			// I
			s = XrMath::sqrt(M._33 - (M._11 + M._22) + 1.0f);
			if (s > TRACE_QZERO_TOLERANCE)
			{
				z = s * 0.5f;
				s = 0.5f / s;
				w = (M._21 - M._12) * s;
				x = (M._31 + M._13) * s;
				y = (M._32 + M._23) * s;
				break;
			}
			// E
			s = XrMath::sqrt(M._22 - (M._33 + M._11) + 1.0f);
			if (s > TRACE_QZERO_TOLERANCE)
			{
				y = s * 0.5f;
				s = 0.5f / s;
				w = (M._13 - M._31) * s;
				z = (M._23 + M._32) * s;
				x = (M._21 + M._12) * s;
				break;
			}
			break;
		case E:
			s = XrMath::sqrt(M._22 - (M._33 + M._11) + 1.0f);
			if (s > TRACE_QZERO_TOLERANCE)
			{
				y = s * 0.5f;
				s = 0.5f / s;
				w = (M._13 - M._31) * s;
				z = (M._23 + M._32) * s;
				x = (M._21 + M._12) * s;
				break;
			}
			// I
			s = XrMath::sqrt(M._33 - (M._11 + M._22) + 1.0f);
			if (s > TRACE_QZERO_TOLERANCE)
			{
				z = s * 0.5f;
				s = 0.5f / s;
				w = (M._21 - M._12) * s;
				x = (M._31 + M._13) * s;
				y = (M._32 + M._23) * s;
				break;
			}
			// A
			s = XrMath::sqrt(M._11 - (M._22 + M._33) + 1.0f);
			if (s > TRACE_QZERO_TOLERANCE)
			{
				x = s * 0.5f;
				s = 0.5f / s;
				w = (M._32 - M._23) * s;
				y = (M._12 + M._21) * s;
				z = (M._13 + M._31) * s;
				break;
			}
			break;
		case I:
			s = XrMath::sqrt(M._33 - (M._11 + M._22) + 1.0f);
			if (s > TRACE_QZERO_TOLERANCE)
			{
				z = s * 0.5f;
				s = 0.5f / s;
				w = (M._21 - M._12) * s;
				x = (M._31 + M._13) * s;
				y = (M._32 + M._23) * s;
				break;
			}
			// A
			s = XrMath::sqrt(M._11 - (M._22 + M._33) + 1.0f);
			if (s > TRACE_QZERO_TOLERANCE)
			{
				x = s * 0.5f;
				s = 0.5f / s;
				w = (M._32 - M._23) * s;
				y = (M._12 + M._21) * s;
				z = (M._13 + M._31) * s;
				break;
			}
			// E
			s = XrMath::sqrt(M._22 - (M._33 + M._11) + 1.0f);
			if (s > TRACE_QZERO_TOLERANCE)
			{
				y = s * 0.5f;
				s = 0.5f / s;
				w = (M._13 - M._31) * s;
				z = (M._23 + M._32) * s;
				x = (M._21 + M._12) * s;
				break;
			}
			break;
		}
	}
	return *this;
}
#undef TRACE_QZERO_TOLERANCE
#pragma pack(pop)

