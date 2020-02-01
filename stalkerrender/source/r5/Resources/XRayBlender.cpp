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
	Pipeline.clear();
	DescriptorHeap.clear();
}

void XRayShaderElement::Copy(const XRayShaderElement& right)
{
	Clear();
	bear_copy(Textures, right.Textures, 16);
	for (bsize i = 0; i < 16; i++)if (Textures[i])Textures[i]->Counter++;
	Pipeline.copy(right.Pipeline);
	DescriptorHeap.copy(right.DescriptorHeap);
	TypeTransformation = right.TypeTransformation;
}

void XRayShaderElement::Swap(XRayShaderElement& right)
{
	bear_swap(Textures, right.Textures, 16);
	Pipeline.swap(right.Pipeline);
	DescriptorHeap.swap(right.DescriptorHeap);
	bear_swap(TypeTransformation, right.TypeTransformation);
}
