#include "pch.h"
#include "BLenders/Blender_CLSID.h"
#include "Blenders/Specially/XRayBlenderHudMovie.h"
XRayResourcesManager::XRayResourcesManager()
{
	{
		BearSamplerDescription SamplerDescription;
		SamplerDescription.Filter = SF_ANISOTROPIC;
		SamplerDescription.MaxAnisotropy = 16;
		SamplerDefault = BearRenderInterface::CreateSampler(SamplerDescription);
	}

}

XRayResourcesManager::~XRayResourcesManager()
{
	for (auto b = m_Blenders.begin(), e = m_Blenders.end(); b != e; b++)
	{
		bear_delete(b->second);
	}
	BEAR_RASSERT(m_Textures.size() == 0);
}

void XRayResourcesManager::LoadShaders(const bchar* name)
{
	string32	ID = "shENGINE";
	string32	id;
	IReader* F = XRayBearReader::Create(FS.Read(TEXT("%content%"), name));
	F->r(&id, 8);
	if (0 == strncmp(id, ID, 8))
	{
		FATAL("Unsupported blender library. Compressed?");
	}


	IReader* fs = 0;
	// Load constants
	
	fs = F->open_chunk(2);
	if (fs) {
		IReader* chunk = NULL;
		int			chunk_id = 0;

		while ((chunk = fs->open_chunk(chunk_id)) != NULL) {
			XRayBlenderDescription	desc;
			chunk->r(&desc, sizeof(desc));
			XRayBlenderCompiler* B = CreateBlender(desc.ID);
			if (0 == B)
			{
				Msg("! Renderer doesn't support blender '%s'", desc.Name);
			}
			else
			{
				if (B->GetDescription().Version != desc.Version)
				{
					Msg("! Version conflict in shader '%s'", desc.Name);
				}

				chunk->seek(0);
				B->Load(*chunk, desc.Version);
				m_Blenders.insert(desc.Name, B);
			}
			BearLog::DebugPrintf(TEXT("Register blender [%s]"), desc.Name);
			chunk->close();
			chunk_id += 1;
		}
		fs->close();
	}
	XRayBearReader::Destroy(F);

	{
		m_Blenders.insert("hud\\movie",bear_new< XRayBlenderHudMovie>());
		m_Blenders.insert("hud\\crosshair", bear_new< XRayBlenderCompiler>());
	}

	for (auto b = m_Blenders.begin(), e = m_Blenders.end(); b != e; b++)
	{
		b->second->Initialize();
	}
}

void XRayResourcesManager::CompileBlender(XRayBlender& Blender, const char* name, const char* textures)
{
	BEAR_ASSERT(m_Blenders.find(name) != m_Blenders.end());
	m_Blenders[name]->Compile(Blender, textures);
}

bsize XRayResourcesManager::GetStride(uint32 VertexState)
{
	switch (VertexState)
	{
	case FVF::F_1W:
		return 36;
		break;
	case FVF::F_2W:
		return 44;
		break;
	case FVF::F_3W:
		return 44;
		break;
	case FVF::F_4W:
		return 40;
		break;
	case FVF::F_R1LMap:
		return 32;
		break;
	case FVF::F_R1Vert:
		return 32;
		break;
	case FVF::F_XVert:
		return 12;
		break;
	case FVF::F_MUModel:
		return 32;
	default:
		break;
	}
	if ((D3DFVF_TEX2 & int32(VertexState)) == D3DFVF_TEX2)
	{
		VertexState |= D3DFVF_TEX1;

	}
	else if ((D3DFVF_TEX3 & int32(VertexState)) == D3DFVF_TEX3)
	{
		VertexState |= D3DFVF_TEX1;
		VertexState |= D3DFVF_TEX2;
	}
	else if ((D3DFVF_TEX4 & int32(VertexState)) == D3DFVF_TEX4)
	{
		VertexState |= D3DFVF_TEX1;
		VertexState |= D3DFVF_TEX2;
		VertexState |= D3DFVF_TEX3;

	}
	static int32 StrideArray[] =
	{
#define REGISTER(name,byte,stride,a1,a2) byte,stride,
#include "D3D9VertexState.h"
#undef REGISTER
	};
	bsize Stride = 0;
	for (bsize i = 0; i < D3DFVF_COUNT; i++)
	{
		if ((StrideArray[i * 2] & int32(VertexState)) == StrideArray[i * 2])
			Stride += StrideArray[i * 2 + 1];
	}
	return Stride;
}

BearFactoryPointer<BearRHI::BearRHIShader> XRayResourcesManager::GetPixelShader(shared_str name)
{
	auto item = m_PShaders.find(name);
	if (item == m_PShaders.end())
	{
		Msg(TEXT("Load shader [%s]"), name.c_str());
	
		BearFactoryPointer<BearRHI::BearRHIShader> shader = BearRenderInterface::CreatePixelShader();
		BearString Text;
		{
			auto File = FS.Read(TEXT("%cur_shaders%"), name.c_str(), ".ps");
			File->ToString(Text, BearEncoding::UTF8);
		}
		BearMap<BearString, BearString> Defines;
		{

		
			BearString out;
			if (!shader->LoadAsText(*Text, Defines, out, 0))
			{
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BearLog::Printf(*out);
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BEAR_RASSERT(0);
			}
			if (out.size())
			{
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BearLog::Printf(*out);
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
			}
		}
		m_PShaders.insert(name, shader);
		return shader;
	}
	return item->second;
}

BearFactoryPointer<BearRHI::BearRHIShader> XRayResourcesManager::GetVertexShader(shared_str name)
{

	auto item = m_VShaders.find(name);
	if (item == m_VShaders.end())
	{
		Msg(TEXT("Load shader [%s]"), name.c_str());
		BearFactoryPointer<BearRHI::BearRHIShader> shader = BearRenderInterface::CreateVertexShader();
		BearString Text;
		{
			auto File = FS.Read(TEXT("%cur_shaders%"), name.c_str(),".vs");
			File->ToString(Text, BearEncoding::UTF8);
		}
		BearMap<BearString, BearString> Defines;
		{


			BearString out;
			if (!shader->LoadAsText(*Text, Defines, out, 0))
			{
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BearLog::Printf(*out);
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BEAR_RASSERT(0);
			}
			if (out.size())
			{
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BearLog::Printf(*out);
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
			}
		}
		m_VShaders.insert(name, shader);
		return shader;
	}
	return item->second;
}

XRayTexture* XRayResourcesManager::GetTexture(shared_str name)
{
	auto texture = m_Textures.find(name);
	if (texture == m_Textures.end())
	{
		Msg(TEXT("Load texture [%s]"), name.c_str());
		m_Textures.insert(name,bear_new<XRayTexture>(name));
		texture = m_Textures.find(name);
	}
	texture->second->Counter++;
	return texture->second;
}

void XRayResourcesManager::Free(XRayTexture* texture)
{
	if (!texture)return;
	texture->Counter--;
	if (texture->Counter == 0)
	{
		Msg(TEXT("Unload texture [%s]"), texture->GetName().c_str());
		m_Textures.erase(texture->GetName());
		bear_delete(texture);
	}
}

XRayBlenderCompiler* XRayResourcesManager::CreateBlender(CLASS_ID ID)
{
	switch (ID)
	{
	case B_DEFAULT:
		return bear_new< XRayBlenderDefault>();
	case B_DEFAULT_AREF:
		return bear_new< XRayBlenderDefaultAref>();
	case B_VERT:
		return bear_new< XRayBlenderVertex>();
	case B_VERT_AREF:
		return bear_new< XRayBlenderVertexAref>();
	case B_LaEmB:
		return bear_new< XRayBlenderLaEmB>();
	case B_LmEbB:
		return bear_new< XRayBlenderLmEbB>();
	case B_BmmD:
		return bear_new< XRayBlenderBmmD>();
	case B_PARTICLE:
		return bear_new< XRayBlenderParticle>();
	case B_SCREEN_SET:
		return bear_new< XRayBlenderScreenSet>();
	case B_SCREEN_GRAY:
		return bear_new< XRayBlenderScreenGray>();
	case B_BLUR:
		return bear_new< XRayBlenderBlur>();
	case B_SHADOW_WORLD:
		return bear_new< XRayBlenderShWorld>();
	case B_DETAIL:
		return bear_new< XRayBlenderDetailStill>();
	case B_TREE:
		return bear_new< XRayBlenderTree>();
	case B_MODEL:
		return bear_new< XRayBlenderModel>();
	case B_MODEL_EbB:
		return bear_new< XRayBlenderModelEbB>();
	case B_EDITOR_WIRE:
		return bear_new<XRayBlenderEditorSelection>();
	case B_EDITOR_SEL:
		return bear_new<XRayBlenderEditorWire>();
	default:
	{
		BearString16 name;
		XrClsid::Clsid2String(ID, name);
		BEAR_FATALERROR(false, TEXT("Not found blender [" BEAR_PRINT_STR "]"), name);
		return 0;
	}
	}
}

XRayResourcesManager* GResourcesManager = nullptr;