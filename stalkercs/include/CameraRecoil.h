////////////////////////////////////////////////////////////////////////////
//	Module 		: CameraRecoil.h
//	Created 	: 26.05.2008
//	Author		: Evgeniy Sokolov
//	Description : Camera Recoil struct
////////////////////////////////////////////////////////////////////////////

#ifndef CAMERA_RECOIL_H_INCLUDED
#define CAMERA_RECOIL_H_INCLUDED

//מעהאקא ןנט סענוכüבו 
struct CameraRecoil
{
	float		RelaxSpeed;
	float		RelaxSpeed_AI;
	float		Dispersion;
	float		DispersionInc;
	float		DispersionFrac;
	float		MaxAngleVert;
	float		MaxAngleHorz;
	float		StepAngleHorz;
	bool		ReturnMode;
	bool		StopReturn;

	CameraRecoil():
		MaxAngleVert	( XrMath::EPS   ),
		RelaxSpeed		( XrMath::EPS_L ),
		RelaxSpeed_AI	( XrMath::EPS_L ),
		Dispersion		( XrMath::EPS   ),
		DispersionInc	( 0.0f  ),
		DispersionFrac	( 1.0f  ),
		MaxAngleHorz	( XrMath::EPS   ),
		StepAngleHorz	( 0.0f  ),
		ReturnMode		( false ),
		StopReturn		( false )
	{};

	CameraRecoil( const CameraRecoil& clone )		{	Clone( clone );	}

	IC void Clone( const CameraRecoil& clone )
	{
		// *this = clone;
		RelaxSpeed		= clone.RelaxSpeed;
		RelaxSpeed_AI	= clone.RelaxSpeed_AI;
		Dispersion		= clone.Dispersion;
		DispersionInc	= clone.DispersionInc;
		DispersionFrac	= clone.DispersionFrac;
		MaxAngleVert	= clone.MaxAngleVert;
		MaxAngleHorz	= clone.MaxAngleHorz;
		StepAngleHorz	= clone.StepAngleHorz;

		ReturnMode		= clone.ReturnMode;
		StopReturn		= clone.StopReturn;
		
		VERIFY( !XrMath::fis_zero(RelaxSpeed)    );
		VERIFY( !XrMath::fis_zero(RelaxSpeed_AI) );
		VERIFY( !XrMath::fis_zero(MaxAngleVert)  );
		VERIFY( !XrMath::fis_zero(MaxAngleHorz)  );
	}
}; //struct CameraRecoil

#endif // CAMERA_RECOIL_H_INCLUDED
