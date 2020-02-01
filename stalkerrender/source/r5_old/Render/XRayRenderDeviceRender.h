#pragma once

class XRayRenderDeviceRender: public IRenderDeviceRender
{
public:
	XRayRenderDeviceRender();
	~XRayRenderDeviceRender();
	virtual void	Copy(IRenderDeviceRender &_in) ;

	//	Gamma correction functions
	virtual void	setGamma(float fGamma) ;
	virtual void	setBrightness(float fGamma) ;
	virtual void	setContrast(float fGamma) ;
	virtual void	updateGamma() ;

	//	Destroy
	virtual void	OnDeviceDestroy(BOOL bKeepTextures) ;
	virtual void	ValidateHW() ;
	virtual void	DestroyHW() ;
	virtual void	Reset(BearUI::BearViewport&viewport) ;
	//	Init
	virtual void	SetupStates() ;
	virtual void	OnDeviceCreate(LPCSTR shName) ;
	virtual void	Create(BearUI::BearViewport&viewport, bool) ;
	virtual void	SetupGPU(BOOL bForceGPU_SW, BOOL bForceGPU_NonPure, BOOL bForceGPU_REF) ;
	//	Overdraw
	virtual void	overdrawBegin() ;
	virtual void	overdrawEnd() ;

	//	Resources control
	virtual void	DeferredLoad(BOOL E) ;
	virtual void	ResourcesDeferredUpload() ;
	virtual void	ResourcesGetMemoryUsage(u32& m_base, u32& c_base, u32& m_lmaps, u32& c_lmaps) ;
	virtual void	ResourcesDestroyNecessaryTextures() ;
	virtual void	ResourcesStoreNecessaryTextures() ;
	virtual void	ResourcesDumpMemoryUsage() ;

	//	HWSupport
	virtual bool	HWSupportsShaderYUV2RGB() ;

	//	Device state
	virtual DeviceState GetDeviceState() ;
	virtual BOOL	GetForceGPU_REF() ;
	virtual u32		GetCacheStatPolys() ;
	virtual void	Begin() ;
	virtual void	Clear() ;
	virtual void	End() ;
	virtual void	ClearTarget() ;
	virtual void	SetCacheXform(Fmatrix &mView, Fmatrix &mProject) ;
	virtual void	OnAssetsChanged() ;
private:
	BearMatrix m_Projection;
	BearUI::BearViewport * m_viewport;
	

};