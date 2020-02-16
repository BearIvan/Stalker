#include "pch.h"
#include "engine/IGame_Persistent.h"
#include "engine/CustomHUD.h"
#include "engine/irenderable.h"
#include "engine/xr_object.h"
#include "engine/EnvironmentSOC.h"
#include "api/XrGameVersionController.h"
HRESULT XRayRenderInterface::shader_compile(LPCSTR name, DWORD const* pSrcData, UINT SrcDataLen, LPCSTR pFunctionName, LPCSTR pTarget, DWORD Flags, void*& result)
{
	return E_NOTIMPL;
}

LPCSTR XRayRenderInterface::getShaderPath()
{
	return "r5";
}

IRender_Sector* XRayRenderInterface::getSector(int id)
{
	return m_Sectors[id];
}

IRenderVisual* XRayRenderInterface::getVisual(int id)
{
	return GetVisual(id);
}


IRender_Target* XRayRenderInterface::getTarget()
{
	return GRenderTarget;
}

void XRayRenderInterface::set_Transform(Fmatrix* M)
{
	m_LTransform = *M;
}

void XRayRenderInterface::set_HUD(BOOL V)
{
	m_bHUD = V;
}

BOOL XRayRenderInterface::get_HUD()
{
	return m_bHUD;
}

void XRayRenderInterface::set_Invisible(BOOL V)
{
	m_bInvisible = V;
}

void XRayRenderInterface::flush()
{
}

void XRayRenderInterface::set_Object(IRenderable* O)
{
}

void XRayRenderInterface::add_Occluder(Fbox2& bb_screenspace)
{
}

void XRayRenderInterface::add_Visual(IRenderVisual* V)
{
	if (m_bInvisible)return;
	XRayRenderVisual* pVisual = (XRayRenderVisual*)V;
	if (0 == pVisual)				return;

	// Visual is 100% visible - simply add it

	switch (pVisual->Type) {
	case MT_PARTICLE_GROUP:
	{
		// Add all children, doesn't perform any tests
		/*PS::CParticleGroup* pG = (PS::CParticleGroup*)pVisual;
		for (PS::CParticleGroup::SItemVecIt i_it = pG->items.begin(); i_it != pG->items.end(); i_it++) {
			PS::CParticleGroup::SItem& I1 = *i_it;
			if (I1._effect)		add_leafs_Dynamic(I1._effect);
			for (xr_vector<dxRender_Visual*>::iterator pit = I1._children_related.begin(); pit != I1._children_related.end(); pit++)	add_leafs_Dynamic(*pit);
			for (xr_vector<dxRender_Visual*>::iterator pit = I1._children_free.begin(); pit != I1._children_free.end(); pit++)	add_leafs_Dynamic(*pit);
		}*/
		return;
	}
	return;
	case MT_HIERRARHY:
	{
		// Add all children, doesn't perform any tests
		XRayFHierrarhyVisual* pV = (XRayFHierrarhyVisual*)pVisual;
		auto I = pV->children.begin();
		auto E = pV->children.end();
		for (; I != E; I++)	add_Visual(*I);
	}
	return;
	case MT_SKELETON_ANIM:
	case MT_SKELETON_RIGID:
	{
		// Add all children, doesn't perform any tests
		XRayKinematics* pV = (XRayKinematics*)pVisual;

		pV->CalculateBones(TRUE);
		//pV->CalculateWallmarks();		//. bug?
		auto I = pV->children.begin();
		auto E = pV->children.end();
		for (; I != E; I++)	add_Visual(*I);
	}
	return;
	default:
	{
		// General type of visual
		// Calculate distance to it's center
		if (m_bHUD)
		{
			DynamicVisual V;
			V.World = m_LTransform;
			V.V = pVisual;
			m_HUDVisuals.push_back(V);
		}
		else
		{
			DynamicVisual V;
			V.World = m_LTransform;
			V.V = pVisual;
			m_DynamicVisuals.push_back(V);
		}
	}
	return;
	}
}

void XRayRenderInterface::add_Geometry(IRenderVisual* V)
{
	EFC_Visible	VIS;
	vis_data& vis = V->getVisData();
	u32 mask= View->getMask();
	VIS = View->testSAABB(vis.sphere.P, vis.sphere.R, vis.box.data(), mask);
	if (fcvNone == VIS)
		return;
	m_GeomVisuals.push_back((XRayRenderVisual*)V);
}

void XRayRenderInterface::add_StaticWallmark(const wm_shader& S, const Fvector& P, float s, CDB::TRI* T, Fvector* V)
{
}

void XRayRenderInterface::add_StaticWallmark(IWallMarkArray* pArray, const Fvector& P, float s, CDB::TRI* T, Fvector* V)
{
}

void XRayRenderInterface::clear_static_wallmarks()
{
}

void XRayRenderInterface::add_SkeletonWallmark(const Fmatrix* xf, IKinematics* obj, IWallMarkArray* pArray, const Fvector& start, const Fvector& dir, float size)
{
}
static float a;
class  ObjectSpecific:public IRender_ObjectSpecific
{
public:
	enum mode
	{
		TRACE_LIGHTS = (1 << 0),
		TRACE_SUN = (1 << 1),
		TRACE_HEMI = (1 << 2),
		TRACE_ALL = (TRACE_LIGHTS | TRACE_SUN | TRACE_HEMI),
	};
public:
	virtual void force_mode(u32 mode) {}
	virtual float get_luminocity() { return 0; }
	virtual float get_luminocity_hemi() { return 0; }
	virtual float* get_luminocity_hemi_cube() { return &a; }

	virtual ~ObjectSpecific() {};
};
IRender_ObjectSpecific* XRayRenderInterface::ros_create(IRenderable* parent)
{
	return xr_new< ObjectSpecific>();
}

void XRayRenderInterface::ros_destroy(IRender_ObjectSpecific*&)
{
}
class RLight : public IRender_Light
{
public:
public:
	virtual void set_type(LT type) {}
	virtual void set_active(bool) {}
	virtual bool get_active() { return false; }
	virtual void set_shadow(bool) {}
	virtual void set_volumetric(bool) {}
	virtual void set_volumetric_quality(float) {}
	virtual void set_volumetric_intensity(float) {}
	virtual void set_volumetric_distance(float) {}
	virtual void set_indirect(bool) {};
	virtual void set_position(const Fvector& P) {}
	virtual void set_rotation(const Fvector& D, const Fvector& R) {}
	virtual void set_cone(float angle) {}
	virtual void set_range(float R) {}
	virtual void set_virtual_size(float R) {}
	virtual void set_texture(LPCSTR name) {}
	virtual void set_color(const Fcolor& C) {}
	virtual void set_color(float r, float g, float b) {}
	virtual void set_hud_mode(bool b) {}
	virtual bool get_hud_mode() {
		return false;
	}
	virtual ~RLight(){}
};
IRender_Light* XRayRenderInterface::light_create()
{
	return xr_new< RLight>();
}
class  RGlow : public IRender_Glow
{
public:
	virtual void set_active(bool) {}
	virtual bool get_active() { return false; }
	virtual void set_position(const Fvector& P) {}
	virtual void set_direction(const Fvector& P) {}
	virtual void set_radius(float R) {}
	virtual void set_texture(LPCSTR name) {}
	virtual void set_color(const Fcolor& C) {}
	virtual void set_color(float r, float g, float b) {}
	virtual ~RGlow() {}
};
IRender_Glow* XRayRenderInterface::glow_create()
{
	return xr_new< RGlow>();
}

IRenderVisual* XRayRenderInterface::model_CreateParticles(LPCSTR name)
{
	return xr_new<XRayRenderVisual>();
}

IRenderVisual* XRayRenderInterface::model_Create(LPCSTR name, IReader* data)
{
	return GModelPool->Create(name, data);
}

IRenderVisual* XRayRenderInterface::model_CreateChild(LPCSTR name, IReader* data)
{
	return  GModelPool->CreateChild(name, data);
}

IRenderVisual* XRayRenderInterface::model_Duplicate(IRenderVisual* V)
{
	return GModelPool->Instance_Duplicate((XRayRenderVisual*)V);
}

void XRayRenderInterface::model_Delete(IRenderVisual*& V, BOOL bDiscard)
{
	XRayRenderVisual* pVisual = (XRayRenderVisual*)V;
	GModelPool->Delete(pVisual, bDiscard);
	V = 0;
}

void XRayRenderInterface::model_Logging(BOOL bEnable)
{
	GModelPool->Logging(bEnable);
}

void XRayRenderInterface::models_Prefetch()
{
	GModelPool->Prefetch();
}

void XRayRenderInterface::models_Clear(BOOL b_complete)
{
	GModelPool->ClearPool(b_complete);
}

BOOL XRayRenderInterface::occ_visible(vis_data& V)
{
	return 0;
}

BOOL XRayRenderInterface::occ_visible(Fbox& B)
{
	return 0;
}

BOOL XRayRenderInterface::occ_visible(sPoly& P)
{
	return 0;
}
ICF bool			pred_sp_sort(ISpatial* _1, ISpatial* _2)
{
	float	d1 = _1->spatial.sphere.P.distance_to_sqr(Device.vCameraPosition);
	float	d2 = _2->spatial.sphere.P.distance_to_sqr(Device.vCameraPosition);
	return	d1 < d2;
}
constexpr float		ps_r__ssaDISCARD = 3.5f;					//RO
constexpr	float	ps_r1_ssaLOD_A = 64.f;
constexpr	float	ps_r1_ssaLOD_B = 48.f;
constexpr float 		ps_r__LOD = 0.75f;
 float r_ssaDISCARD;
 float r_ssaLOD_A, r_ssaLOD_B;
void XRayRenderInterface::Calculate()
{
	float	fov_factor = XrMath::sqr(90.f / Device.fFOV);
	float  g_fSCREEN = float(GRenderTarget->get_width() * GRenderTarget->get_height()) * fov_factor * (XrMath::EPS_S + ps_r__LOD);
	r_ssaDISCARD = XrMath::sqr(ps_r__ssaDISCARD) / g_fSCREEN;
	r_ssaLOD_A = XrMath::sqr(ps_r1_ssaLOD_A / 3) / g_fSCREEN;
	r_ssaLOD_B = XrMath::sqr(ps_r1_ssaLOD_B / 3) / g_fSCREEN;

	ViewBase.CreateFromMatrix(Device.mFullTransform, FRUSTUM_P_LRTB | FRUSTUM_P_FAR);
	View = 0;

	// Detect camera-sector
	if (!vLastCameraPos.similar(Device.vCameraPosition, XrMath::EPS_S))
	{
		CSector* pSector = (CSector*)detectSector(Device.vCameraPosition);
		if (pSector && (pSector != pLastSector))
			g_pGamePersistent->OnSectorChanged(TranslateSector(pSector));

		if (0 == pSector) pSector = pLastSector;
		pLastSector = pSector;
		vLastCameraPos.set(Device.vCameraPosition);
	}

	// Check if camera is too near to some portal - if so force DualRender
	if (rmPortals)
	{
		Fvector box_radius;		box_radius.set(XrMath::EPS_L * 2, XrMath::EPS_L * 2, XrMath::EPS_L * 2);
		Sectors_xrc.box_options(CDB::OPT_FULL_TEST);
		Sectors_xrc.box_query(rmPortals, Device.vCameraPosition, box_radius);
		for (int K = 0; K < Sectors_xrc.r_count(); K++)
		{
			CPortal* pPortal = (CPortal*)m_Portals[rmPortals->get_tris()[Sectors_xrc.r_begin()[K].id].dummy];
			pPortal->bDualRender = TRUE;
		}
	}
	//

	if (pLastSector)
	{
		// Traverse sector/portal structure
		GPortalTraverser.traverse
		(
			pLastSector,
			ViewBase,
			Device.vCameraPosition,
			Device.mFullTransform,
			CPortalTraverser::VQ_HOM + CPortalTraverser::VQ_SSA + CPortalTraverser::VQ_FADE
		);

		// Determine visibility for static geometry hierrarhy
		if (psDeviceFlags.test(rsDrawStatic))
		{
			for (u32 s_it = 0; s_it < GPortalTraverser.r_sectors.size(); s_it++)
			{
				CSector* sector = (CSector*)GPortalTraverser.r_sectors[s_it];
				XRayRenderVisual* root = sector->root();
				for (u32 v_it = 0; v_it < sector->r_frustums.size(); v_it++)
				{
					set_Frustum(&(sector->r_frustums[v_it]));
					add_Geometry(root);
				}
			}
		}
		if (psDeviceFlags.test(rsDrawDynamic)) 
		{
			g_SpatialSpace->q_frustum
			(
				lstRenderables,
				ISpatial_DB::O_ORDERED,
				STYPE_RENDERABLE + STYPE_LIGHTSOURCE,
				ViewBase
			);

			// Exact sorting order (front-to-back)
			std::sort(lstRenderables.begin(), lstRenderables.end(), pred_sp_sort);

			// Determine visibility for dynamic part of scene
			set_Object(0);
			g_hud->Render_First();	// R1 shadows
			g_hud->Render_Last();
			u32 uID_LTRACK = 0xffffffff;
			/*if (phase == PHASE_NORMAL)*/
		/*	{
				uLastLTRACK++;
				if (lstRenderables.size())		uID_LTRACK = uLastLTRACK % lstRenderables.size();

				// update light-vis for current entity / actor
				CObject* O = g_pGameLevel->CurrentViewEntity();
				if (O) {
					CROS_impl* R = (CROS_impl*)O->ROS();
					if (R)		R->update(O);
				}
			}*/
			for (u32 o_it = 0; o_it < lstRenderables.size(); o_it++)
			{
				ISpatial* spatial = lstRenderables[o_it];		spatial->spatial_updatesector();
				CSector* sector = (CSector*)spatial->spatial.sector;
				if (0 == sector)
					continue;	// disassociated from S/P structure

				// Filter only not light spatial
				if (GPortalTraverser.i_marker != sector->r_marker && (spatial->spatial.type & STYPE_RENDERABLE))	continue;	// inactive (untouched) sector

				if (spatial->spatial.type & STYPE_RENDERABLE)
				{
					for (u32 v_it = 0; v_it < sector->r_frustums.size(); v_it++)
					{
						set_Frustum(&(sector->r_frustums[v_it]));

						if (!View->testSphere_dirty(spatial->spatial.sphere.P, spatial->spatial.sphere.R) /*&& (spatial->spatial.type & STYPE_RENDERABLE)*/)	continue;
						// renderable
						IRenderable* renderable = spatial->dcast_Renderable();
						if (0 == renderable)
						{
							// It may be an glow
							/*CGlow* glow = dynamic_cast<CGlow*>(spatial);
							VERIFY(glow);
							L_Glows->add(glow);*/
						}
						else 
						{
							// Occlusiond
							vis_data& v_orig = renderable->renderable.visual->getVisData();
							vis_data		v_copy = v_orig;
							v_copy.box.xform(renderable->renderable.xform);
							//BOOL			bVisible = HOM.visible(v_copy);
							v_orig.accept_frame = v_copy.accept_frame;
							v_orig.marker = v_copy.marker;
							v_orig.hom_frame = v_copy.hom_frame;
							v_orig.hom_tested = v_copy.hom_tested;
						//	if (!bVisible)					break;	// exit loop on frustums

							// rendering
							if (o_it == uID_LTRACK && renderable->renderable_ROS()) 
							{
								/*// track lighting environment
								CROS_impl* T1 = (CROS_impl*)renderable->renderable_ROS();
								T1->update(renderable);*/
							}
							set_Object(renderable);
							renderable->renderable_Render();
							set_Object(0);	//? is it needed at all
						}
						break;	// exit loop on frustums
					}
				}
				else
				{
					/*if (ViewBase.testSphere_dirty(spatial->spatial.sphere.P, spatial->spatial.sphere.R))
					{
						VERIFY(spatial->spatial.type & STYPE_LIGHTSOURCE);
						// lightsource
						light* L = (light*)spatial->dcast_Light();
						VERIFY(L);
						if (L->spatial.sector) {
							vis_data& vis = L->get_homdata();
							if (HOM.visible(vis))	L_DB->add_light(L);
						}
					}*/
				}
			}
		}
	}
	else
	{
		
	}
}

void XRayRenderInterface::Render()
{
	RenderHUD();

	SetWorld(Fidentity);
	for (bsize i = 0; i < m_GeomVisuals.size(); i++)
	{
		m_GeomVisuals[i]->Render(1.f);
	}
	
	for (bsize i = 0; i < m_DynamicVisuals.size(); i++)
	{
		SetWorld(m_DynamicVisuals[i].World);
		m_DynamicVisuals[i].V->Render(1.f);
	}
	if (g_pGamePersistent)	g_pGamePersistent->OnRenderPPUI_main();
	m_GeomVisuals.clear_not_free();
	m_DynamicVisuals.clear_not_free();
	m_HUDVisuals.clear_not_free();
}

void XRayRenderInterface::RenderHUD()
{
	extern ENGINE_API float		psHUD_FOV;

	//PIX_EVENT(r_dsgraph_render_hud);

	// Change projection
	Fmatrix Pold = Device.mProject;
	Fmatrix FTold = Device.mFullTransform;
	if (gameVersionController->getGame() == gameVersionController->SOC)
		Device.mProject.build_projection(
			XrMath::deg2rad(psHUD_FOV * Device.fFOV /* *Device.fASPECT*/),
			Device.fASPECT, VIEWPORT_NEAR,
			ENV_SOC.CurrentEnv.far_plane);
	else
		Device.mProject.build_projection(
			XrMath::deg2rad(psHUD_FOV * Device.fFOV /* *Device.fASPECT*/),
			Device.fASPECT, VIEWPORT_NEAR,
			ENV.CurrentEnv->far_plane);

	Device.mFullTransform.mul(Device.mProject, Device.mView);
	SetProject(Device.mProject);

	// Rendering
	rmNear();
	for (bsize i = 0; i < m_HUDVisuals.size(); i++)
	{
		SetWorld(m_HUDVisuals[i].World);
		m_HUDVisuals[i].V->Render(1.f);
	}

#if	RENDER==R_R1
	if (g_hud && g_hud->RenderActiveItemUIQuery())
		g_hud->RenderActiveItemUI();
#endif
	/*
	if(g_hud && g_hud->RenderActiveItemUIQuery())
	{
#if	RENDER!=R_R1
		// Targets, use accumulator for temporary storage
		const ref_rt	rt_null;
		//	Reset all rt.
		//RCache.set_RT(0,	0);
		RCache.set_RT(0,	1);
		RCache.set_RT(0,	2);
		//if (RImplementation.o.albedo_wo)	RCache.set_RT(RImplementation.Target->rt_Accumulator->pRT,	0);
		//else								RCache.set_RT(RImplementation.Target->rt_Color->pRT,	0);
		if (RImplementation.o.albedo_wo)	RImplementation.Target->u_setrt		(RImplementation.Target->rt_Accumulator,	rt_null,	rt_null,	HW.pBaseZB);
		else								RImplementation.Target->u_setrt		(RImplementation.Target->rt_Color,			rt_null,	rt_null,	HW.pBaseZB);
		//	View port is reset in DX9 when you change rt
		rmNear						();
#endif
		g_hud->RenderActiveItemUI	();

#if	RENDER!=R_R1
		//RCache.set_RT(0,	0);
		// Targets, use accumulator for temporary storage
		if (RImplementation.o.albedo_wo)	RImplementation.Target->u_setrt		(RImplementation.Target->rt_Position,	RImplementation.Target->rt_Normal,	RImplementation.Target->rt_Accumulator,	HW.pBaseZB);
		else								RImplementation.Target->u_setrt		(RImplementation.Target->rt_Position,	RImplementation.Target->rt_Normal,	RImplementation.Target->rt_Color,		HW.pBaseZB);
#endif
	}
	*/

	rmNormal();

	// Restore projection
	Device.mProject = Pold;
	Device.mFullTransform = FTold;
	SetProject(Device.mProject);

}

void XRayRenderInterface::Screenshot(ScreenshotMode mode, LPCSTR name)
{
}

void XRayRenderInterface::Screenshot(ScreenshotMode mode, CMemoryWriter& memory_writer)
{
}

void XRayRenderInterface::ScreenshotAsyncBegin()
{
}

void XRayRenderInterface::ScreenshotAsyncEnd(CMemoryWriter& memory_writer)
{
}

void XRayRenderInterface::rmNear()
{
	HW->Context->SetViewport(0, 0, static_cast<float>(GRenderTarget->get_width()), static_cast<float>(GRenderTarget->get_height()), 0, 0.02f);
}

void XRayRenderInterface::rmFar()
{
	HW->Context->SetViewport(0, 0, static_cast<float>(GRenderTarget->get_width()), static_cast<float>(GRenderTarget->get_height()), 0.99999f, 1.f);
}

void XRayRenderInterface::rmNormal()
{
	HW->Context->SetViewport(0, 0, static_cast<float>(GRenderTarget->get_width()), static_cast<float>(GRenderTarget->get_height()), 0, 1.f);
}

u32 XRayRenderInterface::memory_usage()
{
	return u32();
}

void XRayRenderInterface::ScreenshotImpl(ScreenshotMode mode, LPCSTR name, CMemoryWriter* memory_writer)
{
}


void XRayRenderInterface::UpdateDescriptorHeap(XRayShaderElement& ShaderElement)
{
	if (ShaderElement.RootSignature.empty())return;
	if (ShaderElement.DescriptorHeap.empty())
	{
		ShaderElement.CreateDescriptorHeap();
	}
	switch (ShaderElement.TypeTransformation)
	{
	case STT_Screen:
		ShaderElement.DescriptorHeap->SetUniformBuffer(0, m_ScreenTransformation);
		break;
	case STT_Matrix:
	{
		BEAR_ASSERT(sizeof(ContatMatrix) == sizeof(float) * 16 * 4)
		m_MWV.mul_43(m_MView, m_MWorld);
		m_MVPW.mul(m_MProject, m_MWV);

		if (ShaderElement.TransformationBuffer.empty())
		{
			ShaderElement.TransformationBuffer = BearRenderInterface::CreateUniformBuffer();
			ShaderElement.TransformationBuffer->Create(sizeof(ContatMatrix), true);
		}

		ContatMatrix* matrix = (ContatMatrix*)ShaderElement.TransformationBuffer->Lock();
		matrix->View = m_MView;
		matrix->Project = m_MProject;
		matrix->World = m_MWorld;
		matrix->VPW = m_MVPW;
		ShaderElement.TransformationBuffer->Unlock();
		ShaderElement.DescriptorHeap->SetUniformBuffer(0, ShaderElement.TransformationBuffer);
	}
		break;
	default:
		break;
	}
	for (bsize i = 0; i < 16; i++)
	{
		if (ShaderElement.Textures[i])
		{
			ShaderElement.Textures[i]->Update();
			ShaderElement.DescriptorHeap->SetShaderResource(i, ShaderElement.Textures[i]->Texture);
		}
	}
}

void XRayRenderInterface::SetView(const Fmatrix& mat)
{
	m_MView = mat;
}

void XRayRenderInterface::SetProject(const Fmatrix& mat)
{
	m_MProject = mat;
}

void XRayRenderInterface::SetWorld(const Fmatrix& mat)
{
	m_MWorld = mat;
}

XRayRenderInterface::XRayRenderInterface()
{
	GRenderTarget = 0;
	GModelPool = 0;
	m_MWorld = Fidentity;
}

XRayRenderInterface::GenerationLevel XRayRenderInterface::get_generation()
{
	return GENERATION_R2;
}

bool XRayRenderInterface::is_sun_static()
{
	return true;
}

DWORD XRayRenderInterface::get_dx_level()
{
	return 12;
}

void XRayRenderInterface::create()
{
	m_bHUD = FALSE;
	m_bInvisible = FALSE;
	m_ScreenTransformation = BearRenderInterface::CreateUniformBuffer();
	m_ScreenTransformation->Create(sizeof(float) * 4, true);
	{
		BearVector4<float> ScreenTransformation;
		ScreenTransformation.set(static_cast<float>(Device.dwWidth), static_cast<float>(Device.dwHeight), 1.f / static_cast<float>(Device.dwWidth), 1.f / static_cast<float>(Device.dwHeight));
		bear_copy(m_ScreenTransformation->Lock(), ScreenTransformation.Array, sizeof(float) * 4);
		m_ScreenTransformation->Unlock();
	}
	GRenderTarget = bear_new<XRayRenderTarget>();
	GModelPool = bear_new<XRayModelPool>();
}

void XRayRenderInterface::destroy()
{

	bear_delete(GModelPool); GModelPool = 0;
	bear_delete(GRenderTarget); GRenderTarget = 0;
	m_ScreenTransformation.clear();
}

void XRayRenderInterface::reset_begin()
{
}

void XRayRenderInterface::reset_end()
{
	{
		BearVector4<float> ScreenTransformation;
		ScreenTransformation.set(static_cast<float>(Device.dwWidth), static_cast<float>(Device.dwHeight), 1.f / static_cast<float>(Device.dwWidth), 1.f / static_cast<float>(Device.dwHeight));
		bear_copy(m_ScreenTransformation->Lock(), ScreenTransformation.Array, sizeof(float) * 4);
		m_ScreenTransformation->Unlock();
	}
}
XRayRenderInterface GRenderInterface;