#include "pch.h"
#include "engine/xrTheora_Surface.h"
XRayTexture::XRayTexture(shared_str texture)
{
	pTheora = 0;
	Name = texture;
	BearImage image;
	if (BearString::Find(*texture, "$rt\\"))
	{
		m_type = TT_Default;
		return;
	}
	if (FS.ExistFile(TEXT("%textures%"), texture.c_str(), TEXT(".ogm")))
	{
		m_type = TT_Movie;
		pTheora = xr_new<CTheoraSurface>();
		m_play_time = 0xFFFFFFFF;

		if (!pTheora->Load(*texture))
		{
			xr_delete(pTheora);
			FATAL("Can't open video stream");
		}
		else
		{
			BOOL bstop_at_end = (0 != strstr(texture.c_str(), "intro\\")) || (0 != strstr(texture.c_str(), "outro\\"));
			pTheora->Play(!bstop_at_end, RDEVICE.dwTimeContinual);

			// Now create texture
			u32 _w = pTheora->Width(false);
			u32 _h = pTheora->Height(false);

			Texture = BearRenderInterface::CreateTexture2D(_w, _h, 1, 1, TPF_R8G8B8A8, TU_DYNAMIC);;
			m_size.set(static_cast<float>(_w), static_cast<float>(_h));
		}

	}
	else if (FS.ExistFile(TEXT("%textures%"), texture.c_str(), TEXT(".seq")))
	{
		m_type = TT_Seq;

		string256 buffer;
		IReader* _fs = XRayBearReader::Create(FS.Read("%textures%", texture.c_str(), ".seq"));

		m_seq_cycles = false;
		_fs->r_string(buffer, sizeof(buffer));
		if (0 == BearString::CompareWithoutCase(buffer, "cycled"))
		{
			m_seq_cycles = true;
			_fs->r_string(buffer, sizeof(buffer));
		}
		u32 fps = atoi(buffer);
		m_seq_ms = 1000 / fps;

		while (!_fs->eof())
		{
			_fs->r_string(buffer, sizeof(buffer));
			XrTrims::Trim(buffer);
			if (buffer[0])
			{
				if (FS.ExistFile(TEXT("%textures%"), buffer, TEXT(".dds")))
				{
					BEAR_RASSERT(image.LoadDDSFromStream(**FS.Read(TEXT("%textures%"), buffer, TEXT(".dds"))));
				}
				else
				{
					BEAR_RASSERT(image.LoadDDSFromStream(**FS.Read(TEXT("%textures%"), TEXT("ed"  BEAR_PATH "ed_not_existing_texture"), TEXT(".dds"))));
				}
				Texture = BearRenderInterface::CreateTexture2D(image.GetSize().x, image.GetSize().y, image.GetMips(), image.GetDepth(), image.GetFormat(), TU_STATIC, *image);
				SeqTextures.push_back(Texture);

			}
		}
		BEAR_ASSERT(SeqTextures.size());
		Texture = SeqTextures[0];
		m_size.set(image.GetSize().x, image.GetSize().y);
		XRayBearReader::Destroy(_fs);
	}
	else if (FS.ExistFile(TEXT("%textures%"), texture.c_str(), TEXT(".dds")))
	{
		BEAR_RASSERT(image.LoadDDSFromStream(**FS.Read(TEXT("%textures%"), texture.c_str(), TEXT(".dds"))));
		Texture = BearRenderInterface::CreateTexture2D(image.GetSize().x, image.GetSize().y, image.GetMips(), image.GetDepth(), image.GetFormat(), TU_STATIC, *image);
		m_type = TT_Default;
		m_size.set(image.GetSize().x, image.GetSize().y);
	}
	else
	{
		BEAR_RASSERT(image.LoadDDSFromStream(**FS.Read(TEXT("%textures%"), TEXT("ed"  BEAR_PATH "ed_not_existing_texture"), TEXT(".dds"))));
		Texture = BearRenderInterface::CreateTexture2D(image.GetSize().x, image.GetSize().y, image.GetMips(), image.GetDepth(), image.GetFormat(), TU_STATIC, *image);
		m_size .set( image.GetSize().x, image.GetSize().y);
	}
}

XRayTexture::~XRayTexture()
{
	if (m_type == TT_Movie)
	{
		bear_delete(pTheora);

	}

}

void XRayTexture::Update()
{
	switch (m_type)
	{
	case XRayTexture::TT_Seq:
	{
		bsize	frame = RDEVICE.dwTimeContinual / m_seq_ms;
		u32	frame_data = SeqTextures.size();
		if (m_seq_cycles) {
			u32	frame_id = frame % (frame_data * 2);
			if (frame_id >= frame_data)	frame_id = (frame_data - 1) - (frame_id % frame_data);
			if (frame_id != SeqCurrent)
			{
				SeqCurrent = frame_id;
				Texture = SeqTextures[frame_id];
			}
		}
		else {
			u32	frame_id = frame % frame_data;
			if (frame_id != SeqCurrent)
			{
				SeqCurrent = frame_id;
				Texture = SeqTextures[frame_id];
			}

		}
	}
	break;
	case XRayTexture::TT_Movie:
	{
		if (pTheora->Update(m_play_time != 0xFFFFFFFF ? m_play_time : RDEVICE.dwTimeContinual))
		{

			bsize _pos = 0;
			pTheora->DecompressFrame((u32*)Texture->Lock(), pTheora->Width(false)- pTheora->Width(true), _pos);
			Texture->Unlock();
		}
	}
		break;
	default:
		break;
	}
}
