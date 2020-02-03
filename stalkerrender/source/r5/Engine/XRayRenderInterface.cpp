#include "pch.h"
#include "engine/IGame_Persistent.h"
#include "engine/CustomHUD.h"


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
	return nullptr;
}

IRenderVisual* XRayRenderInterface::getVisual(int id)
{
	return nullptr;
}

IRender_Sector* XRayRenderInterface::detectSector(const Fvector& P)
{
	return nullptr;
}

IRender_Target* XRayRenderInterface::getTarget()
{
	return m_RenderTarget;
}

void XRayRenderInterface::set_Transform(Fmatrix* M)
{
}

void XRayRenderInterface::set_HUD(BOOL V)
{
}

BOOL XRayRenderInterface::get_HUD()
{
	return 0;
}

void XRayRenderInterface::set_Invisible(BOOL V)
{
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
}

void XRayRenderInterface::add_Geometry(IRenderVisual* V)
{
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

void XRayRenderInterface::Calculate()
{
}

void XRayRenderInterface::Render()
{
	
	if(m_Visuals.size())
	for (bsize i = 0; i < m_Visuals.size(); i++)
	{
		m_Visuals[i]->Render(0);
	}
	if (g_pGamePersistent)	g_pGamePersistent->OnRenderPPUI_main();
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
}

void XRayRenderInterface::rmFar()
{
}

void XRayRenderInterface::rmNormal()
{
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
	m_RenderTarget = 0;
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
	m_ScreenTransformation = BearRenderInterface::CreateUniformBuffer();
	m_ScreenTransformation->Create(sizeof(float) * 4, true);
	{
		BearVector4<float> ScreenTransformation;
		ScreenTransformation.set(static_cast<float>(Device.dwWidth), static_cast<float>(Device.dwHeight), 1.f / static_cast<float>(Device.dwWidth), 1.f / static_cast<float>(Device.dwHeight));
		bear_copy(m_ScreenTransformation->Lock(), ScreenTransformation.Array, sizeof(float) * 4);
		m_ScreenTransformation->Unlock();
	}
	m_RenderTarget = bear_new<XRayRenderTarget>();
	GModelPool = bear_new<XRayModelPool>();
}

void XRayRenderInterface::destroy()
{
	bear_delete(GModelPool); GModelPool = 0;
	bear_delete(m_RenderTarget); m_RenderTarget = 0;
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