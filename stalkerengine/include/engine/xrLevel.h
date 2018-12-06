#pragma once
enum fsL_Chunks {
	fsL_HEADER = 1,		//*
	fsL_SHADERS = 2,		//*
	fsL_VISUALS = 3,		//*
	fsL_PORTALS = 4,		//*		- Portal polygons
	fsL_LIGHT_DYNAMIC = 6,		//*
	fsL_GLOWS = 7,		//*		- All glows inside level
	fsL_SECTORS = 8,		//*		- All sectors on level
	fsL_VB = 9,		//*		- Static geometry
	fsL_IB = 10,	//*
	fsL_SWIS = 11,	//*		- collapse info, usually for trees
	fsL_forcedword = 0xFFFFFFFF
};
enum fsESectorChunks {
	fsP_Portals = 1,	// - portal polygons
	fsP_Root,			// - geometry root
	fsP_forcedword = u32(-1)
};
enum fsSLS_Chunks {
	fsSLS_Description = 1,	// Name of level
	fsSLS_ServerState,
	fsSLS_forcedword = u32(-1)
};

enum EBuildQuality {
	ebqDraft = 0,
	ebqHigh,
	ebqCustom,
	ebq_force_u16 = u16(-1)
};
const u32 CFORM_CURRENT_VERSION = 4;
struct hdrCFORM
{
	u32		version;
	u32		vertcount;
	u32		facecount;
	Fbox	aabb;
};
const u32 XRCL_PRODUCTION_VERSION = 14;	// output 
struct hdrLEVEL
{
	u16		XRLC_version;
	u16		XRLC_quality;
};
