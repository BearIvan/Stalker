#pragma once
struct pred_str : public std::binary_function < char*, char*, bool >
{
	IC bool operator()(const char* x, const char* y) const { return xr_strcmp(x, y) < 0; }
};
struct pred_stri : public std::binary_function < char*, char*, bool >
{
	IC bool operator()(const char* x, const char* y) const { return _stricmp(x, y) < 0; }
};
