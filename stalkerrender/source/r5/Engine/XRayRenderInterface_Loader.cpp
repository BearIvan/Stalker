#include "pch.h"
#include "engine/IGame_Persistent.h"
#include "engine/CustomHUD.h"
#include "engine/xrLevel.h"
void XRayRenderInterface::level_Load(IReader* fs)
{
	BEAR_ASSERT(g_pGamePersistent);
	IReader* chunk;
	{
		chunk = fs->open_chunk(fsL_SHADERS);
		R_ASSERT2(chunk, "Level doesn't builded correctly.");
		u32 count = chunk->r_u32();
		m_Shader.resize(count);
		for (u32 i = 0; i < count; i++)	// skip first shader as "reserved" one
		{
			string512				n_sh, n_tlist;
			LPCSTR			n = LPCSTR(chunk->pointer());
			chunk->skip_stringZ();
			if (0 == n[0])			continue;
			xr_strcpy(n_sh, n);
			LPSTR			delim = strchr(n_sh, '/');
			*delim = 0;
			xr_strcpy(n_tlist, delim + 1);
			Msg("Level shader:%s", n_sh);
			GResourcesManager->CompileBlender(m_Shader[i], n_sh, n_tlist);
		}
		chunk->close();
	}
	{
		g_pGamePersistent->LoadTitle("st_loading_geometry");
		g_pGamePersistent->LoadTitle();
		XRayBearFileStream* geom = XRayBearFileStream::Create(FS.Read("%level%", "level.geom"));
		LoadBuffers(geom);
		XRayBearFileStream::Destroy(geom);

		// Visuals
		g_pGamePersistent->LoadTitle("st_loading_spatial_db");
		g_pGamePersistent->LoadTitle();
		chunk = fs->open_chunk(fsL_VISUALS);
		LoadVisuals(chunk);
		chunk->close();
	}
}
#pragma pack(push,4)
struct D3DVERTEXELEMEN_D3D9
{
	u16    Stream;     // Stream index
	u16    Offset;     // Offset in the stream in bytes
	u8    Type;       // Data type
	u8    Method;     // Processing method
	u8    Usage;      // Semantics
	u8    UsageIndex; // Semantic index
};
#pragma pack(pop)
void XRayRenderInterface::LoadBuffers(XRayBearFileStream* base_fs)
{

	// Vertex buffers
	if (base_fs->find_chunk(fsL_VB))
	{
		// Use DX9-style declarators
		XRayBearFileStream* fs = base_fs->open_chunk(fsL_VB);
		u32 count = fs->r_u32();
		m_VertexState.resize(count);
		m_VertexBuffer.resize(count);
		for (u32 i = 0; i < count; i++)
		{
			u32					buffer_size = (MAXD3DDECLLENGTH + 1) * sizeof(D3DVERTEXELEMEN_D3D9);
			D3DVERTEXELEMEN_D3D9* dcl = (D3DVERTEXELEMEN_D3D9*)_alloca(buffer_size);
			fs->r(dcl, buffer_size);
			fs->advance(-(int)buffer_size);
			fs->advance(GetSize(dcl) );
	
			m_VertexState[i] = ConvertFVF(dcl);
			u32 vCount = fs->r_u32();
			u32 vSize = GResourcesManager->GetStride(m_VertexState[i]);
			Msg("* [Loading VB] %d verts, %d Kb", vCount, (vCount * vSize) / 1024);

			u8* pData = xr_alloc<u8>(vCount * vSize);
			fs->r(pData, vCount * vSize);
			m_VertexBuffer[i] = BearRenderInterface::CreateVertexBuffer();
			m_VertexBuffer[i]->Create(vSize, vCount, false, pData);
			xr_free(pData);
			
		}
		XRayBearFileStream::Destroy(fs);
	}
	else {
		FATAL("DX7-style FVFs unsupported");
	}

	// Index buffers
	if (base_fs->find_chunk(fsL_IB))
	{
		XRayBearFileStream* fs = base_fs->open_chunk(fsL_IB);
		u32 count = fs->r_u32();
		m_IndexBuffers.resize(count);
		for (u32 i = 0; i < count; i++)
		{
			u32 iCount = fs->r_u32();
			Msg("* [Loading IB] %d indices, %d Kb", iCount, (iCount * 2) / 1024);

			u32* pData = xr_alloc<u32>(iCount );
			fs->r(pData, iCount * 2);
			u16* srcData =(u16 *) pData;

			for (u32 a = iCount; a !=0 ; a--)
			{
				pData[a - 1] = srcData[a - 1];
			}
			m_IndexBuffers[i] = BearRenderInterface::CreateIndexBuffer();
			m_IndexBuffers[i]->Create(iCount, true, pData);
			xr_free(pData);
		}
		XRayBearFileStream::Destroy(fs);
	}
}

void XRayRenderInterface::LoadVisuals(IReader* fs)
{
	IReader* chunk = 0;
	u32				index = 0;
	XRayRenderVisual* V = 0;
	ogf_header		H;

	while ((chunk = fs->open_chunk(index)) != 0)
	{
		chunk->r_chunk_safe(OGF_HEADER, &H, sizeof(H));
		V = GModelPool->Instance_Create(H.type);
		V->Load(0, chunk, 0);
		m_Visuals.push_back(V);

		chunk->close();
		index++;

	}
}

void XRayRenderInterface::level_Unload()
{
}
