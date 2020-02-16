#pragma once
class XRayShaderElement;
struct D3DVERTEXELEMEN_D3D9;
class XRayRenderInterface :public IRender_interface
{
public:
	XRayRenderInterface();
	virtual GenerationLevel get_generation();

	virtual bool is_sun_static();
	virtual DWORD get_dx_level();

	// Loading / Unloading
	virtual void create();
	virtual void destroy();
	virtual void reset_begin();
	virtual void reset_end();
	 

		
	virtual void level_Load(IReader*);

	virtual void level_Unload();

	//virtual IDirect3DBaseTexture9* texture_load (LPCSTR fname, u32& msize) ;
	void shader_option_skinning(s32 mode) { m_skinning = mode; }
	virtual HRESULT shader_compile(
		LPCSTR name,
		DWORD const* pSrcData,
		UINT SrcDataLen,
		LPCSTR pFunctionName,
		LPCSTR pTarget,
		DWORD Flags,
		void*& result
	);

	// Information
	virtual void Statistics(CGameFont* F) {};

	virtual LPCSTR getShaderPath();
	// virtual ref_shader getShader (int id) ;
	virtual IRender_Sector* getSector(int id);
	virtual IRenderVisual* getVisual(int id);
	virtual IRender_Sector* detectSector(const Fvector& P);
	virtual IRender_Target* getTarget();

	// Main
	IC void set_Frustum(CFrustum* O) { VERIFY(O); View = O; }
	virtual void set_Transform(Fmatrix* M);
	virtual void set_HUD(BOOL V);
	virtual BOOL get_HUD();
	virtual void set_Invisible(BOOL V);
	virtual void flush();
	virtual void set_Object(IRenderable* O);
	virtual void add_Occluder(Fbox2& bb_screenspace); // mask screen region as oclluded (-1..1, -1..1)
	virtual void add_Visual(IRenderVisual* V); // add visual leaf (no culling performed at all)
	virtual void add_Geometry(IRenderVisual* V); // add visual(s) (all culling performed)
	// virtual void add_StaticWallmark (ref_shader& S, const Fvector& P, float s, CDB::TRI* T, Fvector* V)=0;
	virtual void add_StaticWallmark(const wm_shader& S, const Fvector& P, float s, CDB::TRI* T, Fvector* V);
	// Prefer this function when possible
	virtual void add_StaticWallmark(IWallMarkArray* pArray, const Fvector& P, float s, CDB::TRI* T, Fvector* V);
	virtual void clear_static_wallmarks();
	//virtual void add_SkeletonWallmark (intrusive_ptr<CSkeletonWallmark> wm) ;
	//virtual void add_SkeletonWallmark (const Fmatrix* xf, CKinematics* obj, ref_shader& sh, const Fvector& start, const Fvector& dir, float size)=0;
	// Prefer this function when possible
	virtual void add_SkeletonWallmark(const Fmatrix* xf, IKinematics* obj, IWallMarkArray* pArray, const Fvector& start, const Fvector& dir, float size);

	//virtual IBlender* blender_create (CLASS_ID cls) ;
	//virtual void blender_destroy (IBlender* &) ;

	virtual IRender_ObjectSpecific* ros_create(IRenderable* parent);
	virtual void ros_destroy(IRender_ObjectSpecific*&);

	// Lighting/glowing
	virtual IRender_Light* light_create();
	virtual void light_destroy(IRender_Light* p_) { };
	virtual IRender_Glow* glow_create();
	virtual void glow_destroy(IRender_Glow* p_) { };

	// Models
	virtual IRenderVisual* model_CreateParticles(LPCSTR name);
	virtual IRenderVisual* model_Create(LPCSTR name, IReader* data = 0);
	virtual IRenderVisual* model_CreateChild(LPCSTR name, IReader* data);

	virtual IRenderVisual* model_Duplicate(IRenderVisual* V);

	virtual void model_Delete(IRenderVisual*& V, BOOL bDiscard = FALSE);
	virtual void model_Logging(BOOL bEnable);
	virtual void models_Prefetch();
	virtual void models_Clear(BOOL b_complete);

	virtual BOOL occ_visible(vis_data& V);
	virtual BOOL occ_visible(Fbox& B);
	virtual BOOL occ_visible(sPoly& P);

	virtual void Calculate();
	virtual void Render();
	virtual void RenderHUD();

	virtual void Screenshot(ScreenshotMode mode = SM_NORMAL, LPCSTR name = 0);
	virtual void Screenshot(ScreenshotMode mode, CMemoryWriter& memory_writer);
	virtual void ScreenshotAsyncBegin();
	virtual void ScreenshotAsyncEnd(CMemoryWriter& memory_writer);

	virtual void rmNear();
	virtual void rmFar();
	virtual void rmNormal();
	virtual u32 memory_usage();
private:
	int TranslateSector(IRender_Sector* pSector);
	xrXRC														Sectors_xrc;
protected:
	virtual void ScreenshotImpl(ScreenshotMode mode, LPCSTR name, CMemoryWriter* memory_writer);
private:
	BearFactoryPointer<BearRHI::BearRHIUniformBuffer> m_ScreenTransformation;
public:

	u32 ConvertFVF(D3DVERTEXELEMEN_D3D9*);
	bsize GetSize(D3DVERTEXELEMEN_D3D9*);
	void UpdateDescriptorHeap(XRayShaderElement& ShaderElement);
	IC XRayBlender& GetShader(bsize id) { return m_Shader[id]; }
	IC  BearFactoryPointer<BearRHI::BearRHIIndexBuffer>& GetIndexBuffer(bsize id) { return m_IndexBuffers[id]; }
	IC BearFactoryPointer < BearRHI::BearRHIVertexBuffer>& GetVertexBuffer(bsize id) { return m_VertexBuffer[id]; }
	IC u32 GetVertexState(bsize id) { return m_VertexState[id]; }
	IC XRayRenderVisual* GetVisual(bsize id) { return m_Visuals[id]; }
	IC IRender_Portal* GetPortal(bsize id) { return m_Portals[id]; }
	IC FSlideWindowItem* GetSWI(bsize id) { return &m_SWIs[id]; }
	CSector* pLastSector;
	CDB::MODEL* rmPortals;
private:
	BearVector< BearFactoryPointer<BearRHI::BearRHIIndexBuffer>> m_IndexBuffers;
	BearVector< BearFactoryPointer<BearRHI::BearRHIVertexBuffer>> m_VertexBuffer;
	BearVector< XRayRenderVisual*> m_Visuals;
	BearVector< u32> m_VertexState;
	BearVector< XRayBlender> m_Shader;
	BearVector<IRender_Portal*>								m_Portals;
	BearVector<IRender_Sector*>									m_Sectors;
	xr_vector<ISpatial* /**,render_alloc<ISpatial*>/**/>				lstRenderables;
	u32															uLastLTRACK;
public:
	void SetView(const Fmatrix& mat);
	void SetProject(const Fmatrix& mat);
	void SetWorld(const Fmatrix& mat);
	IC Fmatrix& GetWorld() { return m_MWorld; }
private:
	bool m_bInvisible;
	bool m_bHUD;
	Fmatrix m_MView, m_MProject, m_MWorld, m_MVPW, m_MWV,m_LTransform;
#pragma pack(push,4)
	struct ContatMatrix
	{
		Fmatrix View;
		Fmatrix Project;
		Fmatrix World;
		Fmatrix VPW;
	};
#pragma pack(pop)
	Fvector														vLastCameraPos;
	BearVector< XRayRenderVisual*> m_GeomVisuals;
	struct DynamicVisual
	{
		XRayRenderVisual* V;
		Fmatrix World;
	};
	BearVector<DynamicVisual > m_DynamicVisuals;
	BearVector<DynamicVisual > m_HUDVisuals;
	BearVector<FSlideWindowItem>									m_SWIs;
private:
	void								LoadBuffers(XRayBearFileStream* fs);
	void								LoadVisuals(IReader* fs);
	void								LoadSectors(IReader* fs);
	void								LoadSWIs(XRayBearFileStream* base_fs);
};
 extern XRayRenderInterface GRenderInterface;