#pragma once
class CTheoraSurface;
class XRayTexture:public XRayResourceNamed
{
	BEAR_CLASS_WITHOUT_COPY(XRayTexture);
public:
	XRayTexture(shared_str texture);
	~XRayTexture();

	void Update();
	enum TextureType
	{
		TT_Default,
		TT_Seq,
		TT_Movie,
	};
	IC TextureType GetType() const { return m_type; }
	IC BearFVector2 GetSize()const { return m_size; }

	BearFactoryPointer<BearRHI::BearRHITexture2D> Texture;

private:
	BearVector< BearFactoryPointer<BearRHI::BearRHITexture2D>> SeqTextures;
	bsize SeqCurrent;
	BearFVector2 m_size;

	TextureType m_type;

	
	bsize m_seq_ms;

	bool m_seq_cycles;
private:
	CTheoraSurface* pTheora;
	u32 m_play_time;
};