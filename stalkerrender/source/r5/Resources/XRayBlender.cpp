#include "pch.h"
XRayShaderElement::~XRayShaderElement()
{
	for (bsize i = 0; i < 16; i++)
		GResourcesManager->Free(Textures[i]);
}

void XRayShaderElement::Clear()
{
	for (bsize i = 0; i < 16; i++)
		GResourcesManager->Free(Textures[i]);
	for (bsize i = 0; i < 16; i++)
		Textures[i] = 0;
	for (bsize i = 0; i < 16; i++)
	Pipeline[i].clear();
	DescriptorHeap.clear();
	RootSignature.clear();
	TransformationBuffer.clear();
}

void XRayShaderElement::Copy(const XRayShaderElement& right)
{
	Clear();
	bear_copy(Textures, right.Textures, 16);
	for (bsize i = 0; i < 16; i++)if (Textures[i])Textures[i]->Counter++;
	for (bsize i = 0; i < SVD_Count; i++)
		Pipeline[i].copy(right.Pipeline[i]);
	RootSignature.copy(right.RootSignature);
	TypeTransformation = right.TypeTransformation;
	for (bsize i = 0; i < 16; i++)SamplerStates[i] = right.SamplerStates[i];
}

void XRayShaderElement::Swap(XRayShaderElement& right)
{
	bear_swap(Textures, right.Textures, 16);
	for (bsize i = 0; i < SVD_Count; i++)
		Pipeline[i].swap(right.Pipeline[i]);
	for (bsize i = 0; i < 16; i++)bear_swap(SamplerStates[i] , right.SamplerStates[i]);
	RootSignature.swap(right.RootSignature);
	DescriptorHeap.swap(right.DescriptorHeap);
	bear_swap(TypeTransformation, right.TypeTransformation);
}

void XRayShaderElement::CreateDescriptorHeap()
{
	if (!DescriptorHeap.empty())return;
	BearDescriptorHeapDescription DescriptorHeapDescription;
	DescriptorHeapDescription.RootSignature = RootSignature;
	DescriptorHeap = BearRenderInterface::CreateDescriptorHeap(DescriptorHeapDescription);
	for (bsize i = 0; i < 16; i++)

	{
		switch (SamplerStates[i])
		{
		case SSS_Default:
			DescriptorHeap->SetSampler(i, GResourcesManager->SamplerDefault);
			break;
		default:
			break;
		}
	}
}
