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
		Compile(shader.E[i]);
	}
	
}
void XRayBlenderCompiler::SetInputLayout(BearPipelineDescription& Description, u32 VertexState)
{
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
}

BearFactoryPointer<BearRHI::BearRHIDescriptorHeap> XRayBlenderCompiler::CreateDescriptorHeap(BearFactoryPointer<BearRHI::BearRHIRootSignature>& RootSignature)
{
	BearDescriptorHeapDescription DescriptorHeapDescription;
	DescriptorHeapDescription.RootSignature = RootSignature;
	return BearRenderInterface::CreateDescriptorHeap(DescriptorHeapDescription);
}

void XRayBlenderCompiler::SetTexture(XRayShaderElement& shader, bsize id, shared_str name)
{
	shader.Textures[id] = GResourcesManager->GetTexture(GetTexture(name));
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