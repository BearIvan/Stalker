#pragma once
class XRayRenderTarget :public IRender_Target
{
public:
	XRayRenderTarget();
	~XRayRenderTarget();

	virtual void set_blur(float f);
	virtual void set_gray(float f);
	virtual void set_duality_h(float f);
	virtual void set_duality_v(float f);
	virtual void set_noise(float f);
	virtual void set_noise_scale(float f);
	virtual void set_noise_fps(float f);
	virtual void set_color_base(u32 f);
	virtual void set_color_gray(u32 f);
	//virtual void set_color_add (u32 f) = 0;
	virtual void set_color_add(const Fvector& f);
	virtual bsize get_width();
	virtual bsize get_height();;
	virtual void set_cm_imfluence(float f);
	virtual void set_cm_interpolate(float f);
	virtual void set_cm_textures(const shared_str& tex0, const shared_str& tex1);
		
};