#ifndef WAVEFORM_H
#define WAVEFORM_H
#pragma once

#pragma pack(push,4)
struct WaveForm
{
	enum EFunction
	{
		fCONSTANT		= 0,
		fSIN,
		fTRIANGLE,
		fSQUARE,
		fSAWTOOTH,
		fINVSAWTOOTH,
		fFORCE32		= u32(-1)
	};
	IC float signf		(float t) { return t/XrMath::abs(t); }
	IC float Func		(float t)
	{
		switch (F)
		{
			case fCONSTANT:		return 0;
			case fSIN:			return XrMath::sin(t*XrMath::PI_MUL_2);
			case fTRIANGLE:		return asinf(XrMath::sin((t-0.25f)*XrMath::PI_MUL_2))/XrMath::PI_DIV_2;
			case fSQUARE:		return signf(XrMath::cos(t*XrMath::M_PI));
			case fSAWTOOTH:		return atanf(tanf((t+0.5f)*XrMath::M_PI))/XrMath::PI_DIV_2;
			case fINVSAWTOOTH:	return -(atanf(tanf((t+0.5f)*XrMath::M_PI))/XrMath::PI_DIV_2);
		}
		return 0.f;
	}
public:
	EFunction	F;
	float		arg[4];

	IC float Calculate	(float t)
	{
		// y = arg0 + arg1*func( (time+arg2)*arg3 )
		float x = (t+arg[2])*arg[3];
		return arg[0] + arg[1]*Func(x-floorf(x));
	}

	WaveForm()	{
		F		= fCONSTANT;
		arg[0]	= 0;
		arg[1]	= 1;
		arg[2]	= 0;
		arg[3]	= 1;
	}

	IC	BOOL	Similar	(const WaveForm& W) const
	{
		if (!XrMath::fsimilar(arg[0],W.arg[0],XrMath::EPS_L))	return FALSE;
		if (!XrMath::fsimilar(arg[1],W.arg[1],XrMath::EPS_L))	return FALSE;
		if (XrMath::fis_zero(arg[1],XrMath::EPS_L))				return TRUE;
		if (F	!=	W.F)						return FALSE;
		if (!XrMath::fsimilar(arg[2],W.arg[2],XrMath::EPS_L))	return FALSE;
		if (!XrMath::fsimilar(arg[3],W.arg[3],XrMath::EPS_L))	return FALSE;
		return TRUE;
	}
};

#pragma pack(pop)
#endif
