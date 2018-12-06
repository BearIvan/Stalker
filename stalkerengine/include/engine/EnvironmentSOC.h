#pragma once
#include "sound/Sound.h"
// refs
#include "EnvironmentRef.h"
#include "XrRender/FactoryPtr.h"
#include "XrRender/EnvironmentRender.h"
class ENGINE_API	IRender_Visual;
class ENGINE_API	CInifile;
class ENGINE_API 	CEnvironmentSOC;

// refs - effects
class ENGINE_API	CLensFlare;	
class ENGINE_API	CEffect_Rain;
class ENGINE_API	CEffect_Thunderbolt;

class ENGINE_API	CPerlinNoise1D;
#define ENV_SOC (*dynamic_cast<CEnvironmentSOC*>(g_pGamePersistent->Environment()))
#define DAY_LENGTH		86400.f
/*
#include "blenders\blender.h"
class CBlender_skybox		: public IBlender  
{
public:
	virtual		LPCSTR		getComment()	{ return "INTERNAL: combiner";	}
	virtual		BOOL		canBeDetailed()	{ return FALSE;	}
	virtual		BOOL		canBeLMAPped()	{ return FALSE;	}

	virtual		void		Compile			(CBlender_Compile& C)
	{
		C.r_Pass			("sky2",		"sky2",			FALSE,	TRUE, FALSE);
		C.r_Sampler_clf		("s_sky0",		"$null"			);
		C.r_Sampler_clf		("s_sky1",		"$null"			);
		C.r_Sampler_rtf		("s_tonemap",	"$user$tonemap"	);	//. hack
		C.r_End				();
	}
};
*/
// t-defs*
class ENGINE_API	CEnvModifierSOC
{
public:
	Fvector3			position;
	float				radius;
	float				power;

	float				far_plane;
	Fvector3			fog_color;
	float				fog_density;
	Fvector3			ambient;
	Fvector3			sky_color;		
	Fvector3			hemi_color;

	void				load		(IReader*		fs);
	float				sum			(CEnvModifierSOC&	_another, Fvector3& view);
};

class ENGINE_API	CEnvAmbientSOC{
public:
	struct SEffect{
		u32 			life_time;
		ref_sound		sound;		
		shared_str		particles;
		Fvector			offset;
		float			wind_gust_factor;
	};
	DEFINE_VECTOR(SEffect,EffectVec,EffectVecIt);
protected:
	shared_str			section;
	EffectVec			effects;
	xr_vector<ref_sound>sounds;
	Fvector2			sound_dist;
	Ivector2			sound_period;
	Ivector2			effect_period;
public:
	void				load				(const shared_str& section);
	IC SEffect*			get_rnd_effect		(){return effects.empty()?0:&effects[Random.randI(effects.size())];}
	IC ref_sound*		get_rnd_sound		(){return sounds.empty()?0:&sounds[Random.randI(sounds.size())];}
	IC const shared_str&name				(){return section;}
	IC u32				get_rnd_sound_time	(){return Random.randI(sound_period.x,sound_period.y);}
	IC float			get_rnd_sound_dist	(){return Random.randF(sound_dist.x,sound_dist.y);}
	IC u32				get_rnd_effect_time (){return Random.randI(effect_period.x,effect_period.y);}
};

class ENGINE_API	CEnvDescriptorSOC:public EnvDescriptorRef
{
public:
	virtual  const char* getEvnSkyName();
	virtual  const char* getSkyName();
	virtual  const char* getCloudsName();
	float				exec_time;
	float				exec_time_loaded;
	
	shared_str			sky_texture_name	;
	shared_str			sky_texture_env_name;
	shared_str			clouds_texture_name	;

/*	ref_texture			sky_texture		;
	ref_texture			sky_texture_env	;
	ref_texture			clouds_texture	;*/
	FactoryPtr<IEnvDescriptorRender> m_pDescriptor;
	Fvector4			clouds_color	;
	Fvector3			sky_color		;
	float				sky_rotation	;

	float				far_plane;

	Fvector3			fog_color;
	float				fog_density;
	float				fog_distance;

	float				rain_density;
	Fvector3			rain_color;

	float				bolt_period;
	float				bolt_duration;

    float				wind_velocity;
    float				wind_direction;  
    
	Fvector3			ambient		;
	Fvector4			hemi_color	;	// w = R2 correction
	Fvector3			sun_color	;
	Fvector3			sun_dir		;

    int					lens_flare_id;
	int					tb_id;
    
	CEnvAmbientSOC*		env_ambient;

#ifdef DEBUG
	shared_str			sect_name;
#endif	
						CEnvDescriptorSOC	();

	void				load			(LPCSTR exec_tm, LPCSTR sect, CEnvironmentSOC* parent);
	void				copy			(const CEnvDescriptorSOC& src)
	{
		float tm0		= exec_time;
		float tm1		= exec_time_loaded; 
		*this			= src;
		exec_time		= tm0;
		exec_time_loaded= tm1;
	}

	void				on_device_create	();
	void				on_device_destroy	();
};

class ENGINE_API		CEnvDescriptorMixerSOC: public CEnvDescriptorSOC{
public:
/*	STextureList		sky_r_textures;		
	STextureList		sky_r_textures_env;	
	STextureList		clouds_r_textures;	*/
	FactoryPtr<IEnvDescriptorMixerRender> m_pDescriptorMixer;
	float				weight;				

	float				fog_near;		
	float				fog_far;		
public:
	void				lerp			(CEnvironmentSOC* parent, CEnvDescriptorSOC& A, CEnvDescriptorSOC& B, float f, CEnvModifierSOC& M, float m_power);
	void				clear			();
	void				destroy			();
};

class ENGINE_API	CEnvironmentSOC:public EnvironmentRef
{
	struct str_pred : public std::binary_function<shared_str, shared_str, bool>	{	
		IC bool operator()(const shared_str& x, const shared_str& y) const
		{	return xr_strcmp(x,y)<0;	}
	};
public:
	DEFINE_VECTOR			(CEnvAmbientSOC*,EnvAmbVec,EnvAmbVecIt);
	DEFINE_VECTOR			(CEnvDescriptorSOC*,EnvVec,EnvIt);
	DEFINE_MAP_PRED			(shared_str,EnvVec,EnvsMap,EnvsMapIt,str_pred);
public:
	// clouds
	FvectorVec				CloudsVerts;
	U16Vec					CloudsIndices;
private:
	float					NormalizeTime	(float tm);
	float					TimeDiff		(float prev, float cur);
	float					TimeWeight		(float val, float min_t, float max_t);
	void					SelectEnvs		(EnvVec* envs, CEnvDescriptorSOC*& e0, CEnvDescriptorSOC*& e1, float tm);
	void					SelectEnv		(EnvVec* envs, CEnvDescriptorSOC*& e, float tm);
	void					StopWFX			();
//	FactoryPtr<IEnvironmentRender> *m_pRender;
public:
	static bool sort_env_pred	(const CEnvDescriptorSOC* x, const CEnvDescriptorSOC* y)
	{	return x->exec_time < y->exec_time;	}
	static bool sort_env_etl_pred	(const CEnvDescriptorSOC* x, const CEnvDescriptorSOC* y)
	{	return x->exec_time_loaded < y->exec_time_loaded;	}
protected:
//	CBlender_skybox			m_b_skybox;
	CPerlinNoise1D*			PerlinNoise1D;

	float					fGameTime;
public:
	FactoryPtr<IEnvironmentRender> m_pRender;
	float					wind_strength_factor;	
	float					wind_gust_factor;
	bool bNeed_re_create_env;
	// Environments
	CEnvDescriptorMixerSOC		CurrentEnv;
	CEnvDescriptorSOC*			Current[2];

	bool					bWFX;
	float					wfx_time;
	CEnvDescriptorSOC*			WFX_end_desc[2];
    
    EnvVec*					CurrentWeather;
    shared_str				CurrentWeatherName;
	shared_str				CurrentCycleName;

	EnvsMap					WeatherCycles;
	EnvsMap					WeatherFXs;
	xr_vector<CEnvModifierSOC>	Modifiers;
	EnvAmbVec				Ambients;

	/*ref_shader				sh_2sky;
	ref_geom				sh_2geom;

	ref_shader				clouds_sh;
	ref_geom				clouds_geom;*/

	CEffect_Rain*			eff_Rain;
	CLensFlare*				eff_LensFlare;
	CEffect_Thunderbolt*	eff_Thunderbolt;

	float					fTimeFactor;
	/*ref_texture				tonemap;
	ref_texture				tsky0,tsky1;*/

    void					SelectEnvs			(float gt);

	void					UpdateAmbient		();

	CEnvAmbientSOC*			AppendEnvAmb		(const shared_str& sect);

	void					Invalidate			();
public:
	CEnvironmentSOC();
	virtual	~CEnvironmentSOC();

virtual	void					load				();
virtual    void					unload				();

virtual void					mods_load			();
	void					mods_unload			();

	virtual 	void					OnFrame				();

	void					RenderSky			();
	void					RenderClouds		();
	void					RenderFlares		();
	void					RenderLast			();

	bool					SetWeatherFX		(shared_str name);
	bool					IsWFXPlaying		(){return bWFX;}
    void					SetWeather			(shared_str name, bool forced=false);
    shared_str				GetWeather			()					{ return CurrentWeatherName;}
	void					SetGameTime			(float game_time, float time_factor);

	void					OnDeviceCreate		();
	void					OnDeviceDestroy		();

	// editor-related
#ifdef _EDITOR
public:
	float					ed_from_time		;
	float					ed_to_time			;
public:
    void					ED_Reload			();
    float					GetGameTime			(){return fGameTime;}
#endif
};

ENGINE_API extern Flags32	psEnvFlags;
ENGINE_API extern float		psVisDistance;

