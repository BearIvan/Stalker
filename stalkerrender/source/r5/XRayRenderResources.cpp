#include "pch.h"
#include "Blenders/XRayIDofBlenders.h"
XRayRenderResource* GXRayRenderResource=0;
XRayRenderResource::XRayRenderResource():m_CurrentVertexState(0), m_blend_current(0xFFFF)
{
	{
		BearGraphics::BearShaderConstantsInitializer Initializer;
		Initializer.Constants.push_back(BearGraphics::CF_MATRIX);
		Initializer.Constants.push_back(BearGraphics::CF_MATRIX);
		Initializer.Constants.push_back(BearGraphics::CF_MATRIX);
		Initializer.Constants.push_back(BearGraphics::CF_MATRIX);
		m_MatrixConstant = BearGraphics::BearShaderConstantsRef(Initializer);
	}
	{
		BearGraphics::BearShaderConstantsInitializer Initializer;
		Initializer.Constants.push_back(BearGraphics::CF_R32G32B32A32_FLOAT);
		m_ScreenResolution = BearGraphics::BearShaderConstantsRef(Initializer);
		
	}
	{
		BearGraphics::BearBlendStateInitializer blend;
		blend.RenderTarget[0].Enable = false;
		m_blend_default = BearGraphics::BearBlendStateRef(blend);
	}
	m_MView.identity();
	m_MProject.identity();
	m_MWorld.identity();
	m_MVPW.identity();
	m_MWV.identity();
}

XRayRenderResource::~XRayRenderResource()
{
	auto begin = m_MapOfBlenders.begin();
	auto end = m_MapOfBlenders.end();
	while (begin != end)
	{
		begin->second->Destroy();
		begin++;
	}
}

void XRayRenderResource::LoadShaders(const bchar * name)
{
	string32	ID = "shENGINE";
	string32	id;
	IReader*	F = XRayBearReader::Create(FS.Read(TEXT("%content%"), name));
	F->r(&id, 8);
	if (0 == strncmp(id, ID, 8))
	{
		FATAL("Unsupported blender library. Compressed?");
	}


	IReader*	fs = 0;
	// Load constants
	fs = F->open_chunk(0);
	if (fs) {
		while (!fs->eof()) {
			BearString256 const_name;
			fs->r_stringZ(const_name, sizeof(const_name));
			XRayConstant&	C = GetConstant(const_name);
			C.Load(fs);
		}
		fs->close();
	}

	// Load constants
	fs = F->open_chunk(1);
	if (fs) {
		while (!fs->eof()) {
			BearString256 const_name;
			fs->r_stringZ(const_name, sizeof(const_name));
			XRayMatrix&	C = GetConstantMatrix(const_name);
			C.Load(fs);
		}
		fs->close();
	}
	fs = F->open_chunk(2);
	if (fs) {
		IReader*	chunk = NULL;
		int			chunk_id = 0;

		while ((chunk = fs->open_chunk(chunk_id)) != NULL) {
			XRayBlenderDescription	desc;
			chunk->r(&desc, sizeof(desc));
			XRayBlender*		B = CreateBlender(desc.ID);
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
				m_MapOfBlenders.insert(desc.Name, B);
			}
			BearLog::DebugPrintf(TEXT("register blender [%s]"), desc.Name);
			chunk->close();
			chunk_id += 1;
		}
		fs->close();
	}
	XRayBearReader::Destroy(F);
}

bsize XRayRenderResource::GetStride(uint32 VertexState)
{
	static int32 StrideArray[]=
	{
#define REGISTER(name,byte,stride,a1,a2) byte,stride,
#include "D3D9VertexState.h"
#undef REGISTER
	};
	bsize Stride = 0;
	for (bsize i = 0; i < D3DFVF_COUNT; i++)
	{
		if ((StrideArray[i * 2] & int32(VertexState)) == StrideArray[i * 2])
			Stride += StrideArray[i * 2 - 1];
	}
	return Stride;
}
XRayMatrix & XRayRenderResource::GetConstantMatrix(const bchar * name)
{
	auto item = m_MapOfConstantMatrix.find(BearStringConteniar(name, false));
	if (item == m_MapOfConstantMatrix.end())
	{
		m_MapOfConstantMatrix.insert(name);
		item = m_MapOfConstantMatrix.find(BearStringConteniar(name, false));
	}
	return item->second;
}
void XRayRenderResource::CreateShader(const bchar*name, XRayShader & shader)
{
	GetBlender(name)->Compile(shader);
}
XRayBlender * XRayRenderResource::GetBlender(const bchar * name)
{
	auto item = m_MapOfBlenders.find(BearStringConteniar(name, false));
	BearStringPath name_ltx;
	BearString::Copy(name_ltx,name);
	{
		bchar*temp = BearString::ToChar(name_ltx, TEXT("\\")[0]);
		while (temp)
		{
			temp[0] = TEXT('_');
			temp = BearString::ToChar(temp, TEXT("\\")[0]);
		}
	}
	if (item == m_MapOfBlenders.end()&& FS.ExistFile(TEXT("%cur_shaders%"), name_ltx, TEXT(".ltx")))
	{
		m_MapOfBlenders.insert(name, bear_new<XRayBlenderLTX>(name_ltx));
		item = m_MapOfBlenders.find(BearStringConteniar(name, false));
	}
	BEAR_RASSERT(item != m_MapOfBlenders.end());
	return item->second;
}
void XRayRenderResource::CreateBlend(XRayShader & shader)
{
	BearGraphics::BearBlendStateInitializer initializer;
	BEAR_ASSERT(shader.m_blend);
	initializer.RenderTarget[0].Enable = true;
	switch (shader.m_blend_dst)
	{
	case XRayShader::BF_ZERO:
		initializer.RenderTarget[0].ColorDst = BearGraphics::BF_ZERO;
		break;
	case XRayShader::BF_ONE:
		initializer.RenderTarget[0].ColorDst = BearGraphics::BF_ONE;
		break;
	case XRayShader::BF_SRC_COLOR:
		initializer.RenderTarget[0].ColorDst = BearGraphics::BF_SRC_COLOR;
		break;
	case XRayShader::BF_INV_SRC_COLOR:
		initializer.RenderTarget[0].ColorDst = BearGraphics::BF_INV_SRC_COLOR;
		break;
	case XRayShader::BF_SRC_ALPHA:
		initializer.RenderTarget[0].ColorDst = BearGraphics::BF_SRC_ALPHA;
		break;
	case XRayShader::BF_INV_SRC_ALPHA:
		initializer.RenderTarget[0].ColorDst = BearGraphics::BF_INV_SRC_ALPHA;
		break;
	case XRayShader::BF_DEST_ALPHA:
		initializer.RenderTarget[0].ColorDst = BearGraphics::BF_DEST_ALPHA;
		break;
	case XRayShader::BF_INV_DEST_ALPHA:
		initializer.RenderTarget[0].ColorDst = BearGraphics::BF_INV_DEST_ALPHA;
		break;
	case XRayShader::BF_DEST_COLOR:
		initializer.RenderTarget[0].ColorDst = BearGraphics::BF_DEST_COLOR;
		break;
	case XRayShader::BF_INV_DEST_COLOR:
		initializer.RenderTarget[0].ColorDst = BearGraphics::BF_INV_DEST_COLOR;
		break;
	default:
		break;
	}
	switch (shader.m_blend_src)
	{
	case XRayShader::BF_ZERO:
		initializer.RenderTarget[0].ColorSrc = BearGraphics::BF_ZERO;
		break;
	case XRayShader::BF_ONE:
		initializer.RenderTarget[0].ColorSrc = BearGraphics::BF_ONE;
		break;
	case XRayShader::BF_SRC_COLOR:
		initializer.RenderTarget[0].ColorSrc = BearGraphics::BF_SRC_COLOR;
		break;
	case XRayShader::BF_INV_SRC_COLOR:
		initializer.RenderTarget[0].ColorSrc = BearGraphics::BF_INV_SRC_COLOR;
		break;
	case XRayShader::BF_SRC_ALPHA:
		initializer.RenderTarget[0].ColorSrc = BearGraphics::BF_SRC_ALPHA;
		break;
	case XRayShader::BF_INV_SRC_ALPHA:
		initializer.RenderTarget[0].ColorSrc = BearGraphics::BF_INV_SRC_ALPHA;
		break;
	case XRayShader::BF_DEST_ALPHA:
		initializer.RenderTarget[0].ColorSrc = BearGraphics::BF_DEST_ALPHA;
		break;
	case XRayShader::BF_INV_DEST_ALPHA:
		initializer.RenderTarget[0].ColorSrc = BearGraphics::BF_INV_DEST_ALPHA;
		break;
	case XRayShader::BF_DEST_COLOR:
		initializer.RenderTarget[0].ColorSrc = BearGraphics::BF_DEST_COLOR;
		break;
	case XRayShader::BF_INV_DEST_COLOR:
		initializer.RenderTarget[0].ColorSrc = BearGraphics::BF_INV_DEST_COLOR;
		break;
	default:
		break;
	}
	uint16 id = (static_cast<uint16>(shader.m_blend_dst) << 8) | (static_cast<uint16>(shader.m_blend_src));
	m_blend_map.insert(id, initializer);
}
XRayConstant & XRayRenderResource::GetConstant(const bchar * name)
{
	auto item = m_MapOfConstant.find(BearStringConteniar(name,false));
	if (item == m_MapOfConstant.end())
	{
		m_MapOfConstant.insert(name);
		item = m_MapOfConstant.find(BearStringConteniar(name, false));
	}
	return item->second;
}
static bsize GetSize(BearGraphics::BearVertexFormat format)
{
	switch (format)
	{
	case BearGraphics::VF_R32_FLOAT:
		return sizeof(float) * 1;
	case BearGraphics::VF_R32G32_FLOAT:
		return sizeof(float) * 2;
	case BearGraphics::VF_R32G32B32_FLOAT:
		return sizeof(float) * 3;
	case BearGraphics::VF_R32G32B32A32_FLOAT:
		return sizeof(float) * 4;
	case BearGraphics::VF_R32_INT:
		return sizeof(int32);
	case BearGraphics::VF_R8:
		return sizeof(int8) * 1;
	case BearGraphics::VF_R8G8:
		return sizeof(int8) * 2;
	case BearGraphics::VF_R8G8B8A8:
		return sizeof(int8) * 4;
	default:
		return 0;
	}
}
void XRayRenderResource::SetVertexState(uint32 VertexState, const bchar*vs)
{
	if (m_CurrentVertexState == VertexState)
		return;
	auto item = m_MapOfVertexState.find(VertexState);
	if (item == m_MapOfVertexState.end())
	{
		m_MapOfVertexState.insert(VertexState, CreateVertexState(VertexState,vs));
		item = m_MapOfVertexState.find(VertexState);
	}
	BearGraphics::BearRenderInterface::SetVertexState(item->second);
	m_CurrentVertexState = VertexState;
}

void XRayRenderResource::SetView(const Fmatrix & mat)
{
	m_MView = mat; UpdateMatrix();
}

void XRayRenderResource::SetProject(const Fmatrix & mat)
{
	m_MProject = mat; UpdateMatrix();
}

void XRayRenderResource::SetWorld(const Fmatrix & mat)
{
	m_MWorld = mat; UpdateMatrix();
}

void XRayRenderResource::SetShader(XRayShader & shader)
{
	BEAR_ASSERT(shader.m_vertex_state);
	GXRayRenderResource->SetVertexState(shader.m_vertex_state, shader.m_name_vertex);
	BearGraphics::BearRenderInterface::SetPixelShader(shader.m_pixel);
	BearGraphics::BearRenderInterface::SetVertexShader(shader.m_vertex);
	for (bsize i = 0; i < 16 && shader.m_vertex_textures[i]; i++)
	{
		if (shader.m_vertex_textures[i]->GetType() != XRayTexture::TT_Default)shader.m_vertex_textures[i]->Update();
		BearGraphics::BearRenderInterface::SetVertexShaderResource(i, shader.m_vertex_textures[i]->m_texture, BearGraphics::BearDefaultManager::GetSamplerState());
	}
	for (bsize i = 0; i < 16 && shader.m_pixel_textures[i]; i++)
	{
		if (shader.m_pixel_textures[i]->GetType() != XRayTexture::TT_Default)shader.m_pixel_textures[i]->Update();
		BearGraphics::BearRenderInterface::SetPixelShaderResource(i, shader.m_pixel_textures[i]->m_texture, BearGraphics::BearDefaultManager::GetSamplerState());
	}

	switch (shader.m_transform_type)
	{
	case XRayShader::TT_Screen:
		BearGraphics::BearRenderInterface::SetVertexShaderConstants(0, m_ScreenResolution);
		break;
	default:
		BearGraphics::BearRenderInterface::SetVertexShaderConstants(0, m_MatrixConstant);
		break;
	}


	if (shader.m_blend)
	{
		uint16 id = ((static_cast<uint16>(shader.m_blend_dst) << 8) | (static_cast<uint16>(shader.m_blend_src)));
		if (m_blend_current != id)
		{
			m_blend_current = id;

			auto item = m_blend_map.find(id);
			if (item == m_blend_map.end())
			{
				CreateBlend(shader);
				item = m_blend_map.find(id);
			}
			BearGraphics::BearRenderInterface::SetBlendState(item->second, BearCore::BearColor::Transparent);
		}

	}
	else
	{
		if (m_blend_current != 0xFFFF)
		{
			BearGraphics::BearRenderInterface::SetBlendState(m_blend_default, BearCore::BearColor::Transparent);
			m_blend_current = 0xFFFF;
		}
	}

}



void XRayRenderResource::Resize(float wigth, float hieght)
{
	float*screen = (float*)m_ScreenResolution.Lock();
	screen[0] = wigth;
	screen[1] = hieght;
	screen[2] = 1.f / wigth;
	screen[3] = 1.f / hieght;
	m_ScreenResolution.Unlock();
}


struct ContatMatrix
{
	Fmatrix View;
	Fmatrix Project;
	Fmatrix World;
	Fmatrix VPW;
};
void XRayRenderResource::UpdateMatrix()
{
	m_MWV.mul_43(m_MView,m_MWorld);
	m_MVPW.mul(m_MProject, m_MWV);
	ContatMatrix *matrix=(ContatMatrix*)m_MatrixConstant.Lock();
	matrix->View = m_MView;
	matrix->Project = m_MProject;
	matrix->World = m_MWorld;
	matrix->VPW = m_MVPW;
	m_MatrixConstant.Unlock();
}

BearGraphics::BearVertexShaderRef XRayRenderResource::GetVertexShader(const bchar * name)
{
	auto item = m_MapOfVertexShader.find(BearStringConteniar(name,false));
	if (item == m_MapOfVertexShader.end())
	{
		BearGraphics::BearVertexShaderRef var1;
		{
			BearString out;
			if (!var1.CompileFromStream(**FS.Read(TEXT("%cur_shaders%"), name, TEXT(".vs")), BearEncoding::UTF8, out))
			{
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BearLog::Printf(*out);
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
			}
		}
		m_MapOfVertexShader.insert(name, var1);
		item = m_MapOfVertexShader.find(BearStringConteniar(name, false));
	}
	return item->second;
}

BearGraphics::BearPixelShaderRef XRayRenderResource::GetPixelShader(const bchar * name)
{
	auto item = m_MapOfPixelShader.find(BearStringConteniar(name, false));
	if (item == m_MapOfPixelShader.end())
	{
		BearGraphics::BearPixelShaderRef var1;
		{
			BearString out;
			if (!var1.CompileFromStream(**FS.Read(TEXT("%cur_shaders%"), name, TEXT(".ps")), BearEncoding::UTF8, out))
			{
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
				BearLog::Printf(*out);
				BearLog::Printf(TEXT("------------------------------------------------------------------------"));
			}
		}
		m_MapOfPixelShader.insert(name, var1);
		item = m_MapOfPixelShader.find(BearStringConteniar(name, false));
	}
	return item->second;
}

XRayTexture*XRayRenderResource::GetTexture(const bchar * name)
{
	auto item = m_MapOfTextures.find(BearStringConteniar(name, false));
	if (item == m_MapOfTextures.end())
	{
		XRayTexture* texture = bear_new<XRayTexture>(name);
		m_MapOfTextures.insert(name, texture);
		return texture;
	}
	else
	{
		item->second->m_count++;
	}
	return item->second;
}

void XRayRenderResource::Unload(XRayTexture * texture)
{
	texture->m_count--;
	if (texture->m_count == 0)
	{
		auto item = m_MapOfTextures.find(BearStringConteniar(texture->GetName(), false));
		BEAR_ASSERT(item != m_MapOfTextures.end());
		m_MapOfTextures.erase(item);
		bear_delete(texture);
	}
}

BearGraphics::BearVertexStateRef XRayRenderResource::CreateVertexState(uint32 VertexState, const bchar*vs)
{
	int32 Count = 0;
	BearGraphics::BearVertexStateInitializer Initializer;
	struct
	{
		int32 bits;
		bsize size;
		BearGraphics::BearVertexFormat type;
		const bchar8*name;
	}
	StrideArray[] =
	{
#define REGISTER(name,byte,stride,sh_name,sh_type) {byte,stride,sh_type,sh_name},
#include "D3D9VertexState.h"
#undef REGISTER
	};
	for (bsize i = 0; i < D3DFVF_COUNT; i++)
	{
		if ((StrideArray[i].bits &int32( VertexState)) == StrideArray[i].bits)
		{
			Initializer.Elements[Count].Type = StrideArray[i].type;
			Initializer.Elements[Count].Name = StrideArray[i].name;
			if (Count == 0)
				Initializer.Elements[Count].Offset = 0;
			else
			{
				Initializer.Elements[Count].Offset = Initializer.Elements[Count - 1].Offset + GetSize(Initializer.Elements[Count-1].Type);
			}
			Count += 1;
		}

	}
	


	BearGraphics::BearShaderVertexCompilerRef Compiler;
	{
		BearString out;
		if (!Compiler.CompileFromStream(**FS.Read(TEXT("%cur_shaders%"), vs,TEXT(".vs")), BearEncoding::UTF8, out))
		{
			BearLog::Printf(TEXT("------------------------------------------------------------------------"));
			BearLog::Printf(*out);
			BearLog::Printf(TEXT("------------------------------------------------------------------------"));
		}
	}
	return BearGraphics::BearVertexStateRef(Initializer, Compiler.Begin(), Compiler.GetSize());
	
}

XRayBlender * XRayRenderResource::CreateBlender(CLASS_ID ID)
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
		BEAR_FATALERROR(false, TEXT("Неизветный класс бендера[" BEAR_PRINT_CURRENT "]"), name);
		return 0;
	}
	}
}

void XRayRenderResource::LoadShaders()
{
}

void XRayRenderResource::LoadShader(const bchar * name)
{
}
