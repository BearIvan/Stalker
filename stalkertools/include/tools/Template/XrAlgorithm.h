#pragma once


struct pred_str 
{
	IC bool operator()(const char* x, const char* y) const { return BearString::Compare(x, y) < 0; }
};
struct pred_stri
{
	IC bool operator()(const char* x, const char* y) const { return BearString::CompareWithoutCase(x, y) < 0; }
};
