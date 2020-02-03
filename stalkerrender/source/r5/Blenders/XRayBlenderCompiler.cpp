#include "pch.h"

void XRayBlenderCompiler::Initialize()
{
}

LPCSTR XRayBlenderCompiler::getName()
{
	return Description.Name;
}

LPCSTR XRayBlenderCompiler::getComment()
{
	return "Он пуст";
}

void XRayBlenderCompiler::Save(IWriter & fs)
{
	BEAR_RASSERT(false);
}

void XRayBlenderCompiler::Load(IReader & fs, u16 version)
{
	u16	V = Description.Version;
	fs.r(&Description, sizeof(Description));
	Description.Version = V;

	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_INTEGER, oPriority);
	xrPREAD_PROP(fs, xrPID_BOOL, oStrictSorting);
	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_TEXTURE, oTName);
	xrPREAD_PROP(fs, xrPID_MATRIX, oTXform);
}

XRayBlenderCompiler::XRayBlenderCompiler()
{
	
}

XRayBlenderCompiler::~XRayBlenderCompiler()
{
}
inline  bsize GetSize(BearVertexFormat format)
{
	switch (format)
	{
	case VF_R32_FLOAT:
		return sizeof(float) * 1;
	case VF_R32G32_FLOAT:
		return sizeof(float) * 2;
	case VF_R32G32B32_FLOAT:
		return sizeof(float) * 3;
	case VF_R32G32B32A32_FLOAT:
		return sizeof(float) * 4;
	case VF_R32_INT:
		return sizeof(int32);
	case VF_R8:
		return sizeof(int8) * 1;
	case VF_R8G8:
		return sizeof(int8) * 2;
	case VF_R8G8B8A8:
		return sizeof(int8) * 4;
	default:
		return 0;
	}
}
void XRayBlenderCompiler::Compile(XRayShaderElement& shader)
{
}
inline void FixTextureName(LPSTR fn)
{
	LPSTR _ext = strext(fn);
	if (_ext &&
		(0 == stricmp(_ext, ".tga") ||
			0 == stricmp(_ext, ".dds") ||
			0 == stricmp(_ext, ".bmp") ||
			0 == stricmp(_ext, ".ogm")))
		*_ext = 0;
}
void XRayBlenderCompiler::Compile(XRayBlender& shader, const bchar* textures )
{
	m_textures.clear();
	if (textures && textures[0])
	{
		char* P = (char*)textures;
		svector<char, 128> N;

		while (*P)
		{
			if (*P == ',')
			{
				// flush
				N.push_back(0);
				strlwr(N.begin());

				FixTextureName(N.begin());
				m_textures.push_back(N.begin());
				N.clear();
			}
			else
			{
				N.push_back(*P);
			}
			P++;
		}
		if (N.size())
		{
			// flush
			N.push_back(0);
			strlwr(N.begin());

			FixTextureName(N.begin());
			m_textures.push_back(N.begin());
		}
	}

	for (bsize i = 0; i < 6; i++)
	{
		IDShader = i;
		shader.E[i].RootSignature = RootSignature;
		for (bsize a = 0; a < 16; a++)
			shader.E[i].Pipeline[a] = m_pipeline[a];
		Compile(shader.E[i]);
	}
	
}



void XRayBlenderCompiler::SetInputLayout(BearPipelineDescription& Description, u32 VertexState)
{
	switch (VertexState)
	{
	case FVF::F_1W:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", VF_R32G32B32A32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", VF_R8G8B8A8, 16);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", VF_R8G8B8A8, 20);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", VF_R8G8B8A8, 24);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", VF_R32G32_FLOAT, 28);
		Description.InputLayout.Elements[5] = BearInputLayoutElement();

	}
	return;
	break;
	case FVF::F_2W:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", VF_R32G32B32A32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", VF_R8G8B8A8, 16);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", VF_R8G8B8A8, 20);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", VF_R8G8B8A8, 24);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", VF_R32G32B32A32_FLOAT, 28);
		Description.InputLayout.Elements[5] = BearInputLayoutElement();
		return;
		break;
	}
	case FVF::F_3W:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", VF_R32G32B32A32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", VF_R8G8B8A8, 16);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", VF_R8G8B8A8, 20);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", VF_R8G8B8A8, 24);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", VF_R32G32B32A32_FLOAT, 28);
		Description.InputLayout.Elements[5] = BearInputLayoutElement();
	}
	return;
	break;
	case FVF::F_4W:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", VF_R32G32B32A32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", VF_R8G8B8A8, 16);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", VF_R8G8B8A8, 20);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", VF_R8G8B8A8, 24);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", VF_R32G32_FLOAT, 28);
		Description.InputLayout.Elements[5] = BearInputLayoutElement("indices", VF_R8G8B8A8, 36);
		Description.InputLayout.Elements[6] = BearInputLayoutElement();
	}
	return;
	break;
	case FVF::F_R1LMap:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", VF_R32G32B32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", VF_R8G8B8A8, 12);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", VF_R8G8B8A8, 16);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", VF_R8G8B8A8, 20);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", VF_R16G16_SINT, 24);
		Description.InputLayout.Elements[5] = BearInputLayoutElement("uv2t", VF_R16G16_SINT, 28);
		Description.InputLayout.Elements[6] = BearInputLayoutElement();

	}
	return;
	break;
	case FVF::F_R1Vert:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", VF_R32G32B32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", VF_R8G8B8A8, 12);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", VF_R8G8B8A8, 16);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", VF_R8G8B8A8, 20);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("color", VF_R8G8B8A8, 24);
		Description.InputLayout.Elements[5] = BearInputLayoutElement("uv1t", VF_R16G16_SINT, 28);
		Description.InputLayout.Elements[6] = BearInputLayoutElement();
	}
	return;
	break;
	case FVF::F_XVert:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", VF_R32G32B32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement();

	}
	return;
	break;
	case FVF::F_MUModel:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", VF_R32G32B32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", VF_R8G8B8A8, 12);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", VF_R8G8B8A8, 16);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", VF_R8G8B8A8, 20);
		Description.InputLayout.Elements[5] = BearInputLayoutElement("uv1t", VF_R16G16B16A16_SINT, 24);
		Description.InputLayout.Elements[6] = BearInputLayoutElement();

	}
	return;
	default:
		break;
	};
	int32 Count = 0;
	struct
	{
		int32 bits;
		bsize size;
		BearVertexFormat type;
		const bchar8* name;
	}

	StrideArray[] =
	{
#define REGISTER(name,byte,stride,sh_name,sh_type) {byte,stride,sh_type,sh_name},
#include "../D3D9VertexState.h"
#undef REGISTER
	};

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

	for (bsize i = 0; i < D3DFVF_COUNT; i++)
	{
		if ((StrideArray[i].bits & int32(VertexState)) == StrideArray[i].bits)
		{
			Description.InputLayout.Elements[Count].Type = StrideArray[i].type;
			Description.InputLayout.Elements[Count].Name = StrideArray[i].name;
			if (Count == 0)
				Description.InputLayout.Elements[Count].Offset = 0;
			else
			{
				Description.InputLayout.Elements[Count].Offset = Description.InputLayout.Elements[Count - 1].Offset + GetSize(Description.InputLayout.Elements[Count - 1].Type);
			}
			Count += 1;
		}

	}
	if (Count != 16)
	{
		for (bsize i = Count; i < 16; i++)
			Description.InputLayout.Elements[i] = BearInputLayoutElement();
	}
}



void XRayBlenderCompiler::SetTexture(XRayShaderElement& shader, bsize id, shared_str name)
{
	shader.Textures[id] = GResourcesManager->GetTexture(GetTexture(name));
}

void XRayBlenderCompiler::CreatePipeline(BearPipelineDescription& Description, const bchar* name_vs, const bchar* name_ps, ShaderVertexDeclaration FVFType)
{
	u32 fvf = 0;
	const bchar* prefix = TEXT("");
	switch (FVFType)
	{
	case SVD_L:
		prefix = TEXT("_L");
		fvf = FVF::F_L;
		break;
	case SVD_V:
		prefix = TEXT("_V");
		fvf = FVF::F_V;
		break;
	case SVD_LIT:
		prefix = TEXT("_LIT");
		fvf = FVF::F_LIT;
		break;
	case SVD_TL0uv:
		prefix = TEXT("_TL0uv");
		fvf = FVF::F_TL0uv;
		break;
	case SVD_TL:
		prefix = TEXT("_TL");
		fvf = FVF::F_TL;
		break;
	case SVD_TL2uv:
		prefix = TEXT("_TL2uv");
		fvf = FVF::F_TL2uv;
		break;
	case SVD_TL4uv:
		prefix = TEXT("_TL4uv");
		fvf = FVF::F_TL4uv;
		break;
	case SVD_1W:
		prefix = TEXT("_1W");
		fvf = FVF::F_1W;
		break;
	case SVD_2W:
		prefix = TEXT("_2W");
		fvf = FVF::F_2W;
		break;
	case SVD_3W:
		prefix = TEXT("_3W");
		fvf = FVF::F_3W;
		break;
	case SVD_4W:
		prefix = TEXT("_4W");
		fvf = FVF::F_4W;
		break;
	case SVD_R1LMap:
		prefix = TEXT("_R1LMap");
		fvf = FVF::F_R1LMap;
		break;
	case SVD_R1Vert:
		prefix = TEXT("_R1Vert");
		fvf = FVF::F_R1Vert;
		break;
	case SVD_XVert:
		prefix = TEXT("_XVert");
		fvf = FVF::F_XVert;
		break;
	case SVD_MUModel:
		prefix = TEXT("_MUModel");
		fvf = FVF::F_MUModel;
		break;
	default:
		BEAR_RASSERT(0);
		break;
	}
	BearStringPath NameVS;
	BearString::Printf(NameVS, TEXT("%s%s"), name_vs, prefix);
	BearString::ToLower(NameVS);
	SetInputLayout(Description, fvf);
	Description.Shaders.Pixel = GResourcesManager->GetPixelShader(name_ps);
	Description.Shaders.Vertex = GResourcesManager->GetVertexShader(NameVS);
	Description.RootSignature = RootSignature;
	BEAR_ASSERT(m_pipeline[FVFType].empty());
	m_pipeline[FVFType] = BearRenderInterface::CreatePipeline(Description);

}


inline int ParseName(LPCSTR N)
{
	if (0 == strcmp(N, "$null"))
		return -1;
	if (0 == strcmp(N, "$base0"))
		return 0;
	if (0 == strcmp(N, "$base1"))
		return 1;
	if (0 == strcmp(N, "$base2"))
		return 2;
	if (0 == strcmp(N, "$base3"))
		return 3;
	if (0 == strcmp(N, "$base4"))
		return 4;
	if (0 == strcmp(N, "$base5"))
		return 5;
	if (0 == strcmp(N, "$base6"))
		return 6;
	if (0 == strcmp(N, "$base7"))
		return 7;
	return -1;
}

shared_str XRayBlenderCompiler::GetTexture(shared_str name)
{
	int id = ParseName(name.c_str());
	if (id >= 0&&id<m_textures.size())
	{
		return m_textures[id];
	}
	return name;
}

/*
void XRayBlender::Compile(XRayShader & shader)
{
	BEAR_RASSERT(false);
}
*/