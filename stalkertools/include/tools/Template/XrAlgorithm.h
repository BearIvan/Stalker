#pragma once
template <class _Ty1, class _Ty2> inline std::pair<_Ty1, _Ty2> mk_pair(_Ty1 _Val1, _Ty2 _Val2) { return (std::pair<_Ty1, _Ty2>(_Val1, _Val2)); }
struct pred_str : public std::binary_function < char*, char*, bool >
{
	IC bool operator()(const char* x, const char* y) const { return xr_strcmp(x, y) < 0; }
};
struct pred_stri : public std::binary_function < char*, char*, bool >
{
	IC bool operator()(const char* x, const char* y) const { return _stricmp(x, y) < 0; }
};
