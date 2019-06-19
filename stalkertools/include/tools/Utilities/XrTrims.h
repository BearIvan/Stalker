#pragma once
struct xr_token;
class XRCORE_API XrTrims
{
public:
	static bsize GetItemCount(const bchar*, char separator = ',');
	static bchar* GetItem(const bchar*, bsize, bchar*, bsize const dst_size, char separator = ',', const bchar* = "", bool trim = true);

	template <bsize count>
	static inline bchar* GetItem(const bchar* src, bsize index, char(&dst)[count], char separator = ',', const bchar* def = "", bool trim = true)
	{
		return GetItem(src, index, dst, count, separator, def, trim);
	}

	static bchar* GetItems(const bchar*, bsize, bsize, bchar*, char separator = ',');
	static const bchar* SetPos(const bchar* src, bsize pos, char separator = ',');
	static const bchar* CopyVal(const bchar* src, bchar* dst, char separator = ',');
	static bchar* Trim(bchar* str);
	static bchar* TrimLeft(bchar* str);
	static bchar* TrimRight(bchar* str);
	static bchar* ChangeSymbol(bchar* name, char src, char dest);
	static bsize ParseItem(const bchar* src, xr_token* token_list);
	static bsize ParseItem(bchar* src, bsize ind, xr_token* token_list);
	static bchar* ReplaceItem(const bchar* src, bsize index, const bchar* new_item, bchar* dst, char separator);
	static bchar* ReplaceItems(const bchar* src, bsize idx_start, bsize idx_end, const bchar* new_items, bchar* dst, char separator);
	static void SequenceToList(LPSTRVec& lst, const bchar* in, char separator = ',');
	static void SequenceToList(RStringVec& lst, const bchar* in, char separator = ',');
	static void SequenceToList(SStringVec& lst, const bchar* in, char separator = ',');

	static xr_string& Trim(xr_string& src);
	static xr_string& TrimLeft(xr_string& src);
	static xr_string& TrimRight(xr_string& src);
	static xr_string& ChangeSymbol(xr_string& name, char src, char dest);
	static const bchar* CopyVal(const bchar* src, xr_string& dst, char separator = ',');
	static const bchar* GetItem(const bchar* src, bsize, xr_string& p, char separator = ',', const bchar* = "", bool trim = true);
	static xr_string ListToSequence(const SStringVec& lst);
	static shared_str ListToSequence(const RStringVec& lst);
};