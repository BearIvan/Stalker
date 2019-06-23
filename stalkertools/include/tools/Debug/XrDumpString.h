#pragma once
#ifdef DEBUG
class XRCORE_API XrDumpString
{
	BEAR_CLASS_STATIC(XrDumpString)
public:
	static std::string get_string(bool v);
	static std::string get_string(const Fvector& v);
	static std::string get_string(const Fmatrix& dop);
	static std::string get_string(const Fbox& box);

	static std::string dump_string(LPCSTR name, const Fvector& v);
	static std::string dump_string(LPCSTR name, const Fmatrix& form);
	static void dump(LPCSTR name, const Fmatrix& form);
	static void dump(LPCSTR name, const Fvector& v);
};
#endif