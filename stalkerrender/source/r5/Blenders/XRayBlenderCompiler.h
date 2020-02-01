#pragma once
#pragma pack(push,4)
struct XRayBlenderDescription
{
public:
	CLASS_ID		ID;
	BearString128	Name;
	BearString32	Computer;
	uint32			Time;
	uint16			Version;

	XRayBlenderDescription()
	{
		ID = CLASS_ID(0);
		Name[0] = 0;
		Computer[0] = 0;
		Time = 0;
		Version = 0;
	}
};
class  XRayBlenderCompiler:public CPropertyBase
{
	//BEAR_CLASS_NO_COPY(XRayBlender);
public:
	virtual void Initialize();
	virtual LPCSTR getName();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	XRayBlenderCompiler();
	virtual  ~XRayBlenderCompiler();
	inline const XRayBlenderDescription&GetDescription()const
	{
		return Description;
	}
	virtual void Compile(XRayShaderElement& shader);
	void Compile(XRayBlender& shader,const bchar*textures=0);
protected:
	void SetInputLayout(BearPipelineDescription& Description, u32 VertexState);
	BearFactoryPointer<BearRHI::BearRHIDescriptorHeap>CreateDescriptorHeap(BearFactoryPointer < BearRHI::BearRHIRootSignature>& RootSignature);
	void SetTexture(XRayShaderElement& shader,bsize id,shared_str name);
	//;
protected:
	shared_str GetTexture(shared_str name);
	XRayBlenderDescription			Description;
	xrP_Integer						oPriority;
	xrP_BOOL						oStrictSorting;
	BearString64					oTName;
	BearString64					oTXform;
	bsize IDShader;
private:
	BearVector<shared_str> m_textures;
};
#pragma pack(pop)