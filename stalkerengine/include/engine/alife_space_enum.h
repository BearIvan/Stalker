////////////////////////////////////////////////////////////////////////////
//	Module 		: alife_space.h
//	Created 	: 08.01.2002
//  Modified 	: 08.01.2003
//	Author		: Dmitriy Iassenev
//	Description : ALife space
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_ALIFE_SPACE_ENUM
#define XRAY_ALIFE_SPACE_ENUM

namespace ALife {
	enum EHitType {
		eHitTypeBurn = u32(0),
		eHitTypeShock,
		eHitTypeChemicalBurn,
		eHitTypeRadiation,
		eHitTypeTelepatic,
		eHitTypeWound,
		eHitTypeFireWound,
		eHitTypeStrike,
		eHitTypeExplosion,
		eHitTypeWound_2,		//knife's alternative fire
//		eHitTypePhysicStrike,
		eHitTypeLightBurn,
		eHitTypeMax,
	};

};

#endif //XRAY_ALIFE_SPACE