#include "pch_script.h"
#include "fs_registrator.h"

template<bsize sizeBuffer>
inline static void FSGetPath(bchar(&dst)[sizeBuffer], const  bchar *path)
{
	if (path[0] == TEXT('$') && BearCore::BearString::Find(path, TEXT("$game_")) != 0)
	{
		dst[0] = 0;
		BearCore::BearString::Contact(dst, TEXT("%"));
		BearCore::BearString::Contact(dst, path + 6);
		dst[BearCore::BearString::GetSize(dst) - 1] = TEXT('%');
	}
	else
	{
		BearCore::BearString::Copy(dst, path);
	}
}

LPCSTR get_file_age_str(BearCore::BearFileSystem* fs, LPCSTR nm);
BearCore::BearFileSystem* getFS()
{
	return &FS;
}


LPCSTR update_path_script(BearCore::BearFileSystem* fs, LPCSTR initial, LPCSTR src)
{
	BearCore::BearStringPath			temp;
	shared_str			temp_2;
	fs->Update(initial, src, temp);
	temp_2 = temp;
	return *temp_2;
}

class FS_file_list {
	BearCore::BearVector<BearCore::BearString>*	m_p;
public:
	FS_file_list(const BearCore::BearVector<BearCore::BearString>& p) :m_p(BearCore::bear_new<BearCore::BearVector<BearCore::BearString>>(p)) { }
	u32			Size() { return m_p->size(); }
	LPCSTR		GetAt(u32 idx) { return *m_p->at(idx); }
	void		Free() { BearCore::bear_delete(m_p); };
};

struct FS_item
{
	BearCore::BearStringPath name;
	u32			size;
	BearCore::BearFileManager::FileTime			modif;
	string256	buff;

	LPCSTR		NameShort() { return name; }
	LPCSTR		NameFull() { return name; }
	u32			Size() { return size; }
	LPCSTR		Modif()
	{
		BearCore::BearString::Printf(buff, TEXT("%d %d %d %d %d:%d"), uint32(modif.Day), uint32(modif.Month), uint32(modif.Year), uint32(modif.Hour), uint32(modif.Minute), uint32(modif.Second));
		return		buff;
	}

	LPCSTR		ModifDigitOnly()
	{

		BearCore::BearString::Printf(buff, TEXT("%02d:%02d:%4d %02d:%02d"), uint32(modif.Day), uint32(modif.Month), uint32(modif.Year), uint32(modif.Hour), uint32(modif.Minute));
		return		buff;
	}
};

template<bool b>
bool sizeSorter(const FS_item& itm1, const FS_item& itm2) {
	if (b) return	(itm1.size < itm2.size);
	return			(itm2.size < itm1.size);
}
template<bool b>
bool modifSorter(const FS_item& itm1, const FS_item& itm2) {
	if (b) return	(itm1.modif < itm2.modif);
	return			(itm2.modif < itm1.modif);
}
template<bool b>
bool nameSorter(const FS_item& itm1, const FS_item& itm2) {
	if (b) return	(xr_strcmp(itm1.name, itm2.name) < 0);
	return			(xr_strcmp(itm2.name, itm1.name) < 0);
}

class FS_file_list_ex {
	xr_vector<FS_item>	m_file_items;
public:
	enum {
		eSortByNameUp = 0,
		eSortByNameDown,
		eSortBySizeUp,
		eSortBySizeDown,
		eSortByModifUp,
		eSortByModifDown
	};
	FS_file_list_ex(LPCSTR path, u32 flags, LPCSTR mask);

	u32			Size() { return m_file_items.size(); }
	FS_item		GetAt(u32 idx) { return m_file_items[idx]; }
	void		Sort(u32 flags);
};

enum FSFlags
{
	FS_ListFiles = 1,
	FS_ListFolders = 2,
	FS_ClampExt = 4,
	FS_RootOnly = 8,
};
FS_file_list_ex::FS_file_list_ex(LPCSTR path, u32 flags, LPCSTR mask)
{
	BearCore::BearStringPath new_path;
	FSGetPath(new_path, reinterpret_cast<const bchar*>(path));
	BearCore::BearVector<BearCore::BearString> list;
	if (flags&FS_ListFiles)
	{
		FS.GetFiles(list, new_path, mask, !(flags&FS_RootOnly));
	}
	else
	{
		FS.GetDirectories(list, new_path);
	}
	for (auto it = list.begin(); it != list.end(); ++it) {
		m_file_items.push_back(FS_item());
		FS_item& itm = m_file_items.back();
		ZeroMemory(itm.name, sizeof(itm.name));
		BearCore::BearString::Contact(itm.name, **it);
		BearCore::BearStringPath file_path;
		FS.Update(new_path, **it, file_path);

		itm.modif = BearCore::BearFileManager::GetFileLastWriteTime(file_path);
		itm.size = FS.Read(new_path, **it)->Size();
	}

}

void FS_file_list_ex::Sort(u32 flags)
{
	if (flags == eSortByNameUp)		std::sort(m_file_items.begin(), m_file_items.end(), nameSorter<true>);
	else if (flags == eSortByNameDown)	std::sort(m_file_items.begin(), m_file_items.end(), nameSorter<false>);
	else if (flags == eSortBySizeUp)	std::sort(m_file_items.begin(), m_file_items.end(), sizeSorter<true>);
	else if (flags == eSortBySizeDown)	std::sort(m_file_items.begin(), m_file_items.end(), sizeSorter<false>);
	else if (flags == eSortByModifUp)	std::sort(m_file_items.begin(), m_file_items.end(), modifSorter<true>);
	else if (flags == eSortByModifDown)std::sort(m_file_items.begin(), m_file_items.end(), modifSorter<false>);
}

FS_file_list_ex file_list_open_ex(BearCore::BearFileSystem* fs, LPCSTR path, u32 flags, LPCSTR mask)
{
	BearCore::BearStringPath new_path;
	FSGetPath(new_path, reinterpret_cast<const bchar*>(path));
	return FS_file_list_ex(new_path, flags, mask);
}

FS_file_list file_list_open_script(BearCore::BearFileSystem* fs, LPCSTR initial, u32 flags)
{
	BearCore::BearStringPath new_path;
	FSGetPath(new_path, reinterpret_cast<const bchar*>(initial));
	BearCore::BearVector<BearCore::BearString> list;
	if (flags&FS_ListFiles)
	{
		FS.GetFiles(list, new_path, "*", !(flags&FS_RootOnly));
	}
	else
	{
		FS.GetDirectories(list, new_path);
	}
	return FS_file_list(list);
}

FS_file_list file_list_open_script_2(BearCore::BearFileSystem* fs, LPCSTR initial, LPCSTR path, u32 flags)
{
	BearCore::BearVector<BearCore::BearString> list;
	if (flags&FS_ListFiles)
	{
		FS.GetFiles(list, initial, "*", !(flags&FS_RootOnly));
	}
	else
	{
		FS.GetDirectories(list, initial);
	}
	return FS_file_list(list);
}

void dir_delete_script_2(BearCore::BearFileSystem* fs, LPCSTR path, LPCSTR nm, int remove_files)
{
	BearCore::BearStringPath path_;
	BearCore::BearStringPath new_path;
	FSGetPath(new_path, reinterpret_cast<const bchar*>(path));
	FS.UpdatePath(new_path, 0, path_);
	BearCore::BearString::Contact(path_, BEAR_PATH);
	BearCore::BearString::Contact(path_, nm);
	BearCore::BearFileManager::DirectoryDelete(path_, remove_files);
}

void dir_delete_script(BearCore::BearFileSystem* fs, LPCSTR full_path, int remove_files)
{
	BearCore::BearFileManager::DirectoryDelete(full_path, remove_files);
}

LPCSTR get_file_age_str(BearCore::BearFileSystem* fs, LPCSTR nm)
{
	return "";
}
u32 get_file_age(BearCore::BearFileSystem* fs, LPCSTR nm)
{
	return 0;
}
inline int path_exist(BearCore::BearFileSystem*fs, const bchar*str)
{
	return FS.ExistPath(str);
}
static BearCore::BearStringPath get_path_str;
inline const bchar* get_path(BearCore::BearFileSystem*fs, const bchar*str)
{
	FS.UpdatePath(str, 0, get_path_str);
	return get_path_str;
}
inline void append_path(BearCore::BearFileSystem*fs, const bchar*new_path, const bchar*root, const bchar*add, bool recursive)
{
	FS.AppendPath(new_path, add, root, 0);

}
inline int file_delete2(BearCore::BearFileSystem*fs, const bchar*path, const bchar*file)
{
	BearCore::BearStringPath new_path;
	FSGetPath(new_path, reinterpret_cast<const bchar*>(path));
	return FS.Delete(new_path, file);
}
inline void file_delete(BearCore::BearFileSystem*fs, const bchar*file)
{
	BearCore::BearFileManager::FileDelete(file);
}
inline void file_rename(BearCore::BearFileSystem*fs, const bchar*old_file, const bchar*new_file, int)
{
	BearCore::BearFileManager::FileMove(old_file, new_file);
}

inline void file_copy(BearCore::BearFileSystem*fs, const bchar*old_file, const bchar*new_file)
{
	BearCore::BearFileManager::FileCopy(old_file, new_file);
}

inline int file_length(BearCore::BearFileSystem*fs, const bchar*file)
{
	return BearCore::BearFileManager::GetFileSize(file);
}

struct FS_File
{

};
FS_File Ftrue;
inline const FS_File *exist1(BearCore::BearFileSystem*fs, const bchar*file)
{
	if (BearCore::BearFileManager::FileExists(file))
	{
		return &Ftrue;
	}
	return 0;
}
inline const FS_File* exist2(BearCore::BearFileSystem*fs, const bchar*path, const bchar*file)
{
	BearCore::BearStringPath new_path;
	FSGetPath(new_path, reinterpret_cast<const bchar*>(path));
	if (FS.ExistFile(new_path, file))
	{
		return &Ftrue;
	}
	return 0;
}

inline IReader* r_open(BearCore::BearFileSystem*fs, const bchar*file)
{
	return XRayBearReader::Create(file);
}

inline IReader* r_open2(BearCore::BearFileSystem*fs, const bchar*path, const bchar*file)
{
	BearCore::BearStringPath new_path;
	FSGetPath(new_path, reinterpret_cast<const bchar*>(path));
	return XRayBearReader::Create(FS.Read(new_path, file));
}
inline void r_close(BearCore::BearFileSystem*fs, IReader*F)
{
	XRayBearReader::Destroy(F);
}


inline IWriter* w_open(BearCore::BearFileSystem*fs, const bchar*file)
{
	return XRayBearWriter::Create(file);
}

inline IWriter* w_open2(BearCore::BearFileSystem*fs, const bchar*path, const bchar*file)
{
	BearCore::BearStringPath new_path;
	FSGetPath(new_path, reinterpret_cast<const bchar*>(path));
	return XRayBearWriter::Create(FS.Write(new_path, file, 0));
}
inline void w_close(BearCore::BearFileSystem*fs, IWriter*F)
{
	XRayBearWriter::Destroy(F);
}


using namespace luabind;

#pragma optimize("s",on)
void fs_registrator::script_register(lua_State *L)
{
	module(L)
		[
			class_<FS_item>("FS_item")
			.def("NameFull", &FS_item::NameFull)
		.def("NameShort", &FS_item::NameShort)
		.def("Size", &FS_item::Size)
		.def("ModifDigitOnly", &FS_item::ModifDigitOnly)
		.def("Modif", &FS_item::Modif),

		class_<FS_file_list_ex>("FS_file_list_ex")
		.def("Size", &FS_file_list_ex::Size)
		.def("GetAt", &FS_file_list_ex::GetAt)
		.def("Sort", &FS_file_list_ex::Sort),

		class_<FS_file_list>("FS_file_list")
		.def("Size", &FS_file_list::Size)
		.def("GetAt", &FS_file_list::GetAt)
		.def("Free", &FS_file_list::Free),

		/*		class_<FS_Path>("FS_Path")
					.def_readonly("m_Path",						&FS_Path::m_Path)
					.def_readonly("m_Root",						&FS_Path::m_Root)
					.def_readonly("m_Add",						&FS_Path::m_Add)
					.def_readonly("m_DefExt",					&FS_Path::m_DefExt)
					.def_readonly("m_FilterCaption",			&FS_Path::m_FilterCaption),
		*/
		/*	class_<BearCore::BearFileSystem::file>("fs_file")
				.def_readonly("name",						&BearCore::BearFileSystem::file::name)
				.def_readonly("vfs",						&BearCore::BearFileSystem::file::vfs)
				.def_readonly("ptr",						&BearCore::BearFileSystem::file::ptr)
				.def_readonly("size_real",					&BearCore::BearFileSystem::file::size_real)
				.def_readonly("size_compressed",			&BearCore::BearFileSystem::file::size_compressed)
				.def_readonly("modif",						&BearCore::BearFileSystem::file::modif),
				*/
		class_<FS_File>("fs_file"),
		class_<BearCore::BearFileSystem>("FS")
		.enum_("FS_sort_mode")
		[
			value("FS_sort_by_name_up", int(FS_file_list_ex::eSortByNameUp)),
			value("FS_sort_by_name_down", int(FS_file_list_ex::eSortByNameDown)),
			value("FS_sort_by_size_up", int(FS_file_list_ex::eSortBySizeUp)),
			value("FS_sort_by_size_down", int(FS_file_list_ex::eSortBySizeDown)),
			value("FS_sort_by_modif_up", int(FS_file_list_ex::eSortByModifUp)),
			value("FS_sort_by_modif_down", int(FS_file_list_ex::eSortByModifDown))
		]
	.enum_("FS_List")
		[
			value("FS_ListFiles", int(FS_ListFiles)),
			value("FS_ListFolders", int(FS_ListFolders)),
			value("FS_ClampExt", int(FS_ClampExt)),
			value("FS_RootOnly", int(FS_RootOnly))
		]
	.def("path_exist", &path_exist)
		.def("update_path", &update_path_script)
		.def("get_path", &get_path)
		.def("append_path", &append_path)

		.def("file_delete", &file_delete)
		.def("file_delete", &file_delete2)

		.def("dir_delete", &dir_delete_script)
		.def("dir_delete", &dir_delete_script_2)

		.def("file_rename", &file_rename)
		.def("file_length", &file_length)
		.def("file_copy", &file_copy)

		.def("exist", &exist1)
		.def("exist", &exist2)

		.def("get_file_age", &get_file_age)
		.def("get_file_age_str", &get_file_age_str)
		.def("r_open", &r_open)
		.def("r_open", &r_open2)
		.def("r_close", &r_close)

		.def("w_open", &w_open)
		.def("w_open", &w_open2)
		.def("w_close", &w_close)

		.def("file_list_open", &file_list_open_script)
		.def("file_list_open", &file_list_open_script_2)
		.def("file_list_open_ex", &file_list_open_ex),

		def("getFS", getFS)
		];
}
