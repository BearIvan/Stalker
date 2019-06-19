#include "pch.h"

XRayTexture::XRayTexture(const bchar * texture) :m_count(1), m_seq_current(0)
{
	BearString::Copy(m_name, texture);
	BearGraphics::BearImage image;
	if (FS.ExistFile(TEXT("%textures%"), texture, TEXT(".xxx")))
	{
		m_type = TT_Movie;
	}
	else if (FS.ExistFile(TEXT("%textures%"), texture, TEXT(".seq")))
	{
		m_type = TT_Seq;

		string256 buffer;
		IReader* _fs = XRayBearReader::Create(FS.Read("%textures%", texture, ".seq"));

		m_seq_cycles = false;
		_fs->r_string(buffer, sizeof(buffer));
		if (0 == stricmp(buffer, "cycled"))
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
				m_texture.Clear();
				m_texture.Create(image);
				m_seq_array_img.push_back(m_texture);
				
			}
		}
		BEAR_ASSERT(m_seq_array_img.size());
		m_texture = m_seq_array_img[0];
		m_size = m_texture.GetSize();
		XRayBearReader::Destroy(_fs);
	}
	else if (FS.ExistFile(TEXT("%textures%"), texture, TEXT(".dds")))
	{
		BEAR_RASSERT(image.LoadDDSFromStream(**FS.Read(TEXT("%textures%"), texture, TEXT(".dds"))));
		m_texture.Create(image);
		m_type = TT_Default;
		m_size = m_texture.GetSize();
	}
	else
	{
		BEAR_RASSERT(image.LoadDDSFromStream(**FS.Read(TEXT("%textures%"), TEXT("ed"  BEAR_PATH "ed_not_existing_texture"), TEXT(".dds"))));
		m_texture.Create(image);
		m_size = m_texture.GetSize();
	}

}

XRayTexture::~XRayTexture()
{
}

void XRayTexture::Update()
{
	switch (m_type)
	{
	case XRayTexture::TT_Seq:
	{
		bsize	frame = RDEVICE.dwTimeContinual / m_seq_ms; 
		u32	frame_data = m_seq_array_img.size();
		if (m_seq_cycles) {
			u32	frame_id = frame % (frame_data * 2);
			if (frame_id >= frame_data)	frame_id = (frame_data - 1) - (frame_id%frame_data);
			if (frame_id != m_seq_current)
			{
				m_seq_current = frame_id;
				m_texture = m_seq_array_img[frame_id];
			}
		}
		else {
			u32	frame_id = frame % frame_data;
			if (frame_id != m_seq_current)
			{
				m_seq_current = frame_id;
				m_texture = m_seq_array_img[frame_id];
			}
	
		}
	}
		break;
	case XRayTexture::TT_Movie:
		break;
	default:
		break;
	}
}

XRayTexture::TextureType XRayTexture::GetType()
{
	return m_type;
}

const bchar * XRayTexture::GetName() const
{
	return m_name;
}

BearFVector2 XRayTexture::GetSize() const
{
	return m_size;
}
