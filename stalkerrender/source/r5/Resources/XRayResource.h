#pragma once
class XRayResource
{
	BEAR_CLASS_WITHOUT_COPY(XRayResource);
public:

	XRayResource() :Counter(0) {}
	bsize Counter;
};
class XRayResourceNamed:public XRayResource
{
	BEAR_CLASS_WITHOUT_COPY(XRayResourceNamed);
public:

	XRayResourceNamed() {}
	shared_str GetName() { return Name; }
protected:
	shared_str Name;
};