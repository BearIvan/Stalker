#pragma once
class XRayShader
{
	friend class XRayRenderResource;
public:
	XRayShader();
	~XRayShader();
	void SetShader(const bchar*ps, const bchar*vs);
	void SetVertexState(uint32 state);
	void SetTextureToPixel(bsize id, const bchar*texture);
	void SetTextureToVertex(bsize id, const bchar*texture);
	BearVector2<float> GetSizeTextureFromPixel(bsize id );
	BearVector2<float> GetSizeTextureFromVertex(bsize id );
	enum TransformType
	{
		TT_Matrix,
		TT_Screen,
	};
	void SetTransformType(TransformType type);
	enum BlendFactor
	{
		BF_ZERO,
		BF_ONE,
		BF_SRC_COLOR,
		BF_INV_SRC_COLOR,
		BF_SRC_ALPHA,
		BF_INV_SRC_ALPHA,
		BF_DEST_ALPHA,
		BF_INV_DEST_ALPHA,
		BF_DEST_COLOR,
		BF_INV_DEST_COLOR,
	};
	inline XRayShader& SetBlend(BlendFactor blend_src, BlendFactor blend_dst)
	{
		m_blend = true;
		m_blend_src = blend_src;
		m_blend_dst = blend_dst;
		return*this;
	}
private:
	BearGraphics::BearVertexShaderRef m_vertex;
	XRayTexture* m_vertex_textures[16];
	BearGraphics::BearPixelShaderRef m_pixel;
	XRayTexture* m_pixel_textures[16];
	BearString64 m_name_vertex;
	uint32 m_vertex_state;
	TransformType m_transform_type;
	BlendFactor m_blend_src;
	BlendFactor m_blend_dst;
	bool m_blend;
};