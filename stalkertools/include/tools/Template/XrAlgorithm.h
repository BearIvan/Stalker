#pragma once
struct pred_str : public std::binary_function < char*, char*, bool >
{
	IC bool operator()(const char* x, const char* y) const { return BearCore::BearString::Compare(x, y) < 0; }
};
struct pred_stri : public std::binary_function < char*, char*, bool >
{
	IC bool operator()(const char* x, const char* y) const { return BearCore::BearString::CompareWithoutCase(x, y) < 0; }
};
