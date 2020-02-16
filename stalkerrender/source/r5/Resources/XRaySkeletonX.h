#pragma once
class XRayKinematics;
#include "engine/bone.h"
class XRaySkeletonX
{
protected:
	enum { RM_SKINNING_SOFT, RM_SINGLE, RM_SKINNING_1B, RM_SKINNING_2B, RM_SKINNING_3B, RM_SKINNING_4B };

	XRayKinematics* Parent;		// setted up by parent
	ref_smem<vertBoned1W>	Vertices1W;		// shared
	ref_smem<vertBoned2W>	Vertices2W;		// shared
	ref_smem<vertBoned3W>	Vertices3W;		// shared
	ref_smem<vertBoned4W>	Vertices4W;		// shared
	ref_smem<u16>			BonesUsed;		// actual bones which have influence on vertices

	u16						RenderMode;
	u16						ChildIDX;

	// render-mode specifics
	union {
		struct {			// soft-skinning only
			u32				cache_DiscardID;
			bsize				cache_vCount;
			bsize				cache_vOffset;
		};
		u32					RMS_boneid;			// single-bone-rendering
		u32					RMS_bonecount;		// skinning, maximal bone ID
	};

	void					_Copy(XRaySkeletonX* V);
	void					_Load(const char* N, IReader* data, bsize& dwVertCount);

	void					_Render_soft(u32 FVF,BearFactoryPointer<BearRHI::BearRHIVertexBuffer>&VertexBuffer, BearFactoryPointer<BearRHI::BearRHIIndexBuffer>& IdexBuffer, bsize CountVertex, bsize OffsetIndex, bsize CountIndex);
	void					_Render(u32 FVF, BearFactoryPointer<BearRHI::BearRHIVertexBuffer>& VertexBuffer, BearFactoryPointer<BearRHI::BearRHIIndexBuffer>& IdexBuffer, bsize CountVertex, bsize OffsetIndex, bsize CountIndex, BearFactoryPointer<BearRHI::BearRHIUniformBuffer>& UniformBuffer);
	virtual void			_Load_hw(XRayFVisual& V, void* data) = 0;
	virtual void			_CollectBoneFaces(XRayFVisual* V, bsize iBase, bsize iCount) = 0;


	BOOL					_PickBoneSoft1W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, u16* indices, CBoneData::FacesVec& faces);
	BOOL					_PickBoneSoft2W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, u16* indices, CBoneData::FacesVec& faces);
	BOOL					_PickBoneSoft3W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, u16* indices, CBoneData::FacesVec& faces);
	BOOL					_PickBoneSoft4W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, u16* indices, CBoneData::FacesVec& faces);

	virtual BOOL			_PickBoneHW1W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, XRayFVisual* V, u16* indices, CBoneData::FacesVec& faces) = 0;
	virtual BOOL			_PickBoneHW2W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, XRayFVisual* V, u16* indices, CBoneData::FacesVec& faces) = 0;
	virtual BOOL			_PickBoneHW3W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, XRayFVisual* V, u16* indices, CBoneData::FacesVec& faces) = 0;
	virtual BOOL			_PickBoneHW4W(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, XRayFVisual* V, u16* indices, CBoneData::FacesVec& faces) = 0;

	virtual BOOL			_PickBone(IKinematics::pick_result& r, float range, const Fvector& S, const Fvector& D, XRayFVisual* V, u16 bone_id, bsize iBase, bsize iCount) = 0;
public:
	BOOL					has_visible_bones();
	XRaySkeletonX() { Parent = 0; ChildIDX = u16(-1); }

	virtual void			SetParent(XRayKinematics* K) { Parent = K; }
	virtual void			AfterLoad(XRayKinematics* parent, u16 child_idx) = 0;
	virtual void			EnumBoneVertices(SEnumVerticesCallback& C, u16 bone_id) = 0;
	virtual BOOL			PickBone(IKinematics::pick_result& r, float dist, const Fvector& start, const Fvector& dir, u16 bone_id) = 0;

	void			_DuplicateIndices(const char* N, IReader* data);
	//	Index buffer replica since we can't read from index buffer in DX10
	ref_smem<u16>			m_Indices;
};