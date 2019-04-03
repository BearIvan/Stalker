#pragma once
class XRayTexture
{
	friend class XRayRenderResource;
	BEAR_CLASS_NO_COPY(XRayTexture);
public:
	XRayTexture(const bchar*texture);
	~XRayTexture();
	void Update();
	enum TextureType
	{
		TT_Default,
		TT_Seq,
		TT_Movie,
	};
	TextureType GetType();
	const bchar*GetName()const;
	BearFVector2 GetSize()const;
private:
	bsize m_count;
	BearStringPath m_name;
	BearFVector2 m_size;
	BearGraphics::BearTexture2DRef m_texture;
	TextureType m_type;

	BearVector<BearGraphics::BearTexture2DRef> m_seq_array_img;
	bsize m_seq_ms;
	bsize m_seq_current;
	bool m_seq_cycles;
};