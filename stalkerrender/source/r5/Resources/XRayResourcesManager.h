#pragma once
class XRayResourcesManager
{
	BEAR_CLASS_WITHOUT_COPY(XRayResourcesManager);
public:
	XRayResourcesManager();
	~XRayResourcesManager();
	void LoadShaders(const bchar* name);
	void CompileBlender(XRayBlender& Blender, const char* name, const char* textures);
	bsize GetStride(uint32 VertexState);
	BearFactoryPointer<BearRHI::BearRHIShader> GetPixelShader (shared_str name);
	BearFactoryPointer<BearRHI::BearRHIShader> GetVertexShader(shared_str name);

	XRayTexture* GetTexture(shared_str name);
	void Free(XRayTexture* texture);


	BearFactoryPointer<BearRHI::BearRHISampler> SamplerDefault;
private:
	XRayBlenderCompiler* CreateBlender(CLASS_ID ID);
	BearMap< shared_str, XRayTexture*> m_Textures;
	BearMap<shared_str, XRayBlenderCompiler*> m_Blenders;
	BearMap< shared_str, BearFactoryPointer<BearRHI::BearRHIShader>> m_PShaders; 
	BearMap< shared_str, BearFactoryPointer<BearRHI::BearRHIShader>> m_VShaders;
};
extern XRayResourcesManager* GResourcesManager;