// Rain.h: interface for the CRain class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ThunderboltH
#define ThunderboltH
#pragma once

//refs
class ENGINE_API IRender_DetailModel;
class ENGINE_API CLAItem;

#include "XrRender/FactoryPtr.h"
#include "XrRender/LensFlareRender.h"
#include "XrRender/ThunderboltDescRender.h"
#include "XrRender/ThunderboltRender.h"

#ifdef INGAME_EDITOR
# define INGAME_EDITOR_VIRTUAL virtual
#else // #ifdef INGAME_EDITOR
# define INGAME_EDITOR_VIRTUAL
#endif // #ifdef INGAME_EDITOR

class CEnvironment;

struct SThunderboltDesc
{
    // geom
    //IRender_DetailModel* l_model;
    FactoryPtr<IThunderboltDescRender> m_pRender;
    // sound
    ref_sound snd;
    // gradient
    struct SFlare
    {
        float fOpacity;
        Fvector2 fRadius;
        shared_str texture;
        shared_str shader;
        //ref_shader hShader;
        FactoryPtr<IFlareRender> m_pFlare;
        SFlare() { fOpacity = 0; fRadius.set(0.f, 0.f); }
    };
    SFlare* m_GradientTop;
    SFlare* m_GradientCenter;
    shared_str name;
    CLAItem* color_anim;
public:
    SThunderboltDesc();
    INGAME_EDITOR_VIRTUAL ~SThunderboltDesc();
	SThunderboltDesc(CInifile* pIni, LPCSTR sect);
    void load(CInifile& pIni, shared_str const& sect);
    INGAME_EDITOR_VIRTUAL void create_top_gradient(CInifile& pIni, shared_str const& sect);
    INGAME_EDITOR_VIRTUAL void create_center_gradient(CInifile& pIni, shared_str const& sect);
};

#undef INGAME_EDITOR_VIRTUAL

struct SThunderboltCollection
{
    DEFINE_VECTOR(SThunderboltDesc*, DescVec, DescIt);
    DescVec palette;
    shared_str section;
public:
	SThunderboltCollection(CInifile* pIni, LPCSTR sect);
    SThunderboltCollection();
    ~SThunderboltCollection();
    void load(CInifile* pIni, CInifile* thunderbolts, LPCSTR sect);
    SThunderboltDesc* GetRandomDesc() { VERIFY(palette.size() > 0); return palette[static_cast<bsize>(Random.randI(static_cast<s32>(palette.size())))]; }
};

#define THUNDERBOLT_CACHE_SIZE 8
//
class ENGINE_API CEffect_Thunderbolt
{
	friend class dxThunderboltRender;
protected:
	DEFINE_VECTOR(SThunderboltCollection*, CollectionVec, CollectionVecIt);
	CollectionVec collection;
	SThunderboltDesc* current;
private:
	Fmatrix current_xform;
	Fvector3 current_direction;

	FactoryPtr<IThunderboltRender> m_pRender;
	//ref_geom hGeom_model;
	// states
	enum EState
	{
		stIdle,
		stWorking
	};
	EState state;

	//ref_geom hGeom_gradient;

	Fvector lightning_center;
	float lightning_size;
	float lightning_phase;

	float life_time;
	float current_time;
	float next_lightning_time;
	BOOL bEnabled;

	// params
	 Fvector2 p_var_alt;
	 float p_var_long;
	 float p_min_dist;
	 float p_tilt;
	 float p_second_prop;
	 float p_sky_color;
	 float p_sun_color;
	 float p_fog_color;
private:
	BOOL RayPick(const Fvector& s, const Fvector& d, float& range);
	void Bolt(shared_str id, float period, float life_time);
	void						Bolt(bsize id, float period, float life_time);

public:
	CEffect_Thunderbolt();
	~CEffect_Thunderbolt();
	void						OnFrame(bsize id, float period, float duration);

	void OnFrame(shared_str id, float period, float duration);
	void Render();

	shared_str AppendDef(CEnvironment& environment, CInifile* pIni, CInifile* thunderbolts, LPCSTR sect);
	bsize							AppendDef(CInifile* pIni, LPCSTR sect);

};

#endif //ThunderboltH