#pragma once
struct SEnumVerticesCallback
{
	virtual void operator () (const Fvector& p) = 0;
};


class		XRaySkeletonXExt : public XRaySkeletonX	// shared code for SkeletonX derivates
{
protected:
	BearFactoryPointer<BearRHI::BearRHIUniformBuffer> m_UniformBuffer;
	virtual void			_Load_hw(XRayFVisual& V, void* data);
	virtual void			_CollectBoneFaces(XRayFVisual* V, bsize iBase, bsize iCount);
	void			        _EnumBoneVertices(SEnumVerticesCallback& C, XRayFVisual* V, u16 bone_id, bsize iBase, bsize iCount) const;

	virtual BOOL			_PickBoneHW1W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, XRayFVisual* V, u16* indices, CBoneData::FacesVec& faces);
	virtual BOOL			_PickBoneHW2W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, XRayFVisual* V, u16* indices, CBoneData::FacesVec& faces);
	virtual BOOL			_PickBoneHW3W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, XRayFVisual* V, u16* indices, CBoneData::FacesVec& faces);
	virtual BOOL			_PickBoneHW4W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, XRayFVisual* V, u16* indices, CBoneData::FacesVec& faces);

	virtual BOOL			_PickBone(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, XRayFVisual* V, u16 bone_id, bsize iBase, bsize iCount);

public:
};

class XRaySkeletonX_ST : public XRayFVisual, public XRaySkeletonXExt
{
private:
	typedef XRayFVisual			inherited1;
	typedef XRaySkeletonXExt	inherited2;
public:
	XRaySkeletonX_ST() {}
	virtual					~XRaySkeletonX_ST() {}
	virtual void			Render(float LOD);
	virtual void			Load(const char* N, IReader* data, u32 dwFlags);
	virtual void			Copy(XRayRenderVisual* pFrom);
	virtual void			AfterLoad(XRayKinematics* parent, u16 child_idx);
	virtual void			EnumBoneVertices(SEnumVerticesCallback& C, u16 bone_id);
	virtual BOOL			PickBone(IKinematics::pick_result& r, float dist, const Fvector& start, const Fvector& dir, u16 bone_id);
private:
};

class XRaySkeletonX_PM : public XRayFProgressive, public XRaySkeletonXExt
{
private:
	typedef XRayFProgressive	inherited1;
	typedef XRaySkeletonXExt	inherited2;
public:
	XRaySkeletonX_PM() {}
	virtual					~XRaySkeletonX_PM() {}
	virtual void			Render(float LOD);
	virtual void			Load(const char* N, IReader* data, u32 dwFlags);
	virtual void			Copy(XRayRenderVisual* pFrom);
	virtual void			AfterLoad(XRayKinematics* parent, u16 child_idx);
	virtual void			EnumBoneVertices(SEnumVerticesCallback& C, u16 bone_id);
	virtual BOOL			PickBone(IKinematics::pick_result& r, float dist, const Fvector& start, const Fvector& dir, u16 bone_id);
private:
};