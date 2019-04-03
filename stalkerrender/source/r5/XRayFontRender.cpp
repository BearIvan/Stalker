#include "pch.h"
#include "engine/GameFont.h"
ENGINE_API extern  Fvector2 g_current_font_scale;

XRayFontRender::XRayFontRender() : m_index_count(0), m_vertex_count(0)
{
}

XRayFontRender::~XRayFontRender()
{
}

void XRayFontRender::Initialize(LPCSTR cShader, LPCSTR cTexture)
{
	GXRayRenderResource->CreateShader(cShader, m_shader);
	m_shader.SetTextureToPixel(0, cTexture);
}

void XRayFontRender::OnRender(CGameFont & owner)
{
	GXRayRenderResource->SetShader(m_shader);

	if (!(owner.uFlags&CGameFont::fsValid)) {
		auto size = m_shader.GetSizeTextureFromPixel(0);
		owner.vTS.set((int)size.x, (int)size.y);
		owner.fTCHeight = owner.fHeight / float(owner.vTS.y);
		owner.uFlags |= CGameFont::fsValid;
	}

	for (u32 i = 0; i < owner.strings.size(); ) {
		// calculate first-fit
		int		count = 1;

		int length = owner.smart_strlen(owner.strings[i].string);

		while ((i + count) < owner.strings.size()) {
			int L = owner.smart_strlen(owner.strings[i + count].string);

			if ((L + length) < MAX_MB_CHARS) {
				count++;
				length += L;
			}
			else		break;
		}

		// lock AGP memory
		u32	vOffset = 0;
		if(m_vertex_count< length * 4)
		m_vertex_buffer.Create(length * 4 * sizeof(FVF::TL));

		FVF::TL* v = (FVF::TL*)	m_vertex_buffer.Lock();

		FVF::TL* start = v;

		// fill vertices
		u32 last = i + count;
		for (; i < last; i++) {
			CGameFont::String		&PS = owner.strings[i];
			wide_char wsStr[MAX_MB_CHARS];

			int	len = owner.IsMultibyte() ?
				mbhMulti2Wide(wsStr, NULL, MAX_MB_CHARS, PS.string) :
				xr_strlen(PS.string);

			if (len) {
				float	X = float(iFloor(PS.x));
				float	Y = float(iFloor(PS.y));
				float	S = PS.height*g_current_font_scale.y;
				float	Y2 = Y + S;
				float fSize = 0;

				if (PS.align)
					fSize = owner.IsMultibyte() ? owner.SizeOf_(wsStr) : owner.SizeOf_(PS.string);

				switch (PS.align)
				{
				case CGameFont::alCenter:
					X -= (iFloor(fSize * 0.5f)) * g_current_font_scale.x;
					break;
				case CGameFont::alRight:
					X -= iFloor(fSize);
					break;
				}

				u32	clr, clr2;
				clr2 = clr = PS.c;
				if (owner.uFlags&CGameFont::fsGradient) {
					u32	_R = color_get_R(clr) / 2;
					u32	_G = color_get_G(clr) / 2;
					u32	_B = color_get_B(clr) / 2;
					u32	_A = color_get_A(clr);
					clr2 = color_rgba(_R, _G, _B, _A);
				}

				X -= 0.5f;
				Y -= 0.5f;
				Y2 -= 0.5f;

				float	tu, tv;
				for (int j = 0; j < len; j++)
				{
					Fvector l;

					l = owner.IsMultibyte() ? owner.GetCharTC(wsStr[1 + j]) : owner.GetCharTC((u16)(u8)PS.string[j]);

					float scw = l.z * g_current_font_scale.x;

					float fTCWidth = l.z / owner.vTS.x;

					if (!fis_zero(l.z))
					{
						//						tu			= ( l.x / owner.vTS.x ) + ( 0.5f / owner.vTS.x );
						//						tv			= ( l.y / owner.vTS.y ) + ( 0.5f / owner.vTS.y );
						tu = (l.x / owner.vTS.x);
						tv = (l.y / owner.vTS.y);
#ifndef	USE_DX10
						//	Make half pixel offset for 1 to 1 mapping
						tu += (0.5f / owner.vTS.x);
						tv += (0.5f / owner.vTS.y);
#endif	//	USE_DX10

						v->set(X, Y2, clr2, tu, tv + owner.fTCHeight);						v++;
						v->set(X, Y, clr, tu, tv);									v++;
						v->set(X + scw, Y2, clr2, tu + fTCWidth, tv + owner.fTCHeight);		v++;
						v->set(X + scw, Y, clr, tu + fTCWidth, tv);					v++;
					}
					X += scw * owner.vInterval.x;
					if (owner.IsMultibyte()) {
						X -= 2;
						if (IsNeedSpaceCharacter(wsStr[1 + j]))
							X += owner.fXStep;
					}
				}
			}
		}

		// Unlock and draw
		u32 vCount = (u32)(v - start);
		m_vertex_buffer.Unlock();

		if (vCount) {
			bsize count = vCount / 4 * 6;
			BearGraphics::BearRenderInterface::SetVertexBuffer(m_vertex_buffer);
			if (m_index_count < count)
			{
			
				m_index_buffer.Create(count * sizeof(uint32));
				uint32*list = (uint32*)m_index_buffer.Lock();
				bear_fill(list, count);
				m_index_count = count;
				for (bsize i = 0; i < count/6; i++)
				{
					list[i * 6+0] = i * 4 + 0;
					list[i * 6+1] = i * 4 + 1;
					list[i * 6+2] = i * 4 + 2;
					list[i * 6+3] = i *4 + 1;
					list[i * 6+4] = i * 4 + 3;
					list[i * 6+5] = i *4 + 2;
					
				}
				m_index_buffer.Unlock();
			}
			BearGraphics::BearRenderInterface::SetIndexBuffer(m_index_buffer);
			BearGraphics::BearRenderInterface::Draw(count);
		}
	}
}
