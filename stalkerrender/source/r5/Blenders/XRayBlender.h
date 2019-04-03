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
class  XRayBlender:public CPropertyBase
{
	//BEAR_CLASS_NO_COPY(XRayBlender);
public:
	virtual LPCSTR getName();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void Destroy();
	inline const XRayBlenderDescription&GetDescription()const
	{
		return Description;
	}
	virtual void Compile(XRayShader&shader);
protected:
	XRayBlenderDescription			Description;
	xrP_Integer						oPriority;
	xrP_BOOL						oStrictSorting;
	BearString64					oTName;
	BearString64					oTXform;
};
#pragma pack(pop)