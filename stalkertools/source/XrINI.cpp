#include "stdafx.h"
#pragma hdrstop
#include "api/XrGameVersionController.h"

bool isspace_(int ch)
{
	return ((ch == 0x09) || (ch == 0x0A) || (ch == 0x0B) ||
		(ch == 0x0B) || (ch == 0x0C) || (ch == 0x0D) || (ch == 0x20)) ? true : false;
}

bool isdigit_(int ch)
{
	return ((ch == '0') || (ch == '1') || (ch == '2') || (ch == '3') || (ch == '4') ||
		(ch == '5') || (ch == '6') || (ch == '7') || (ch == '8') || (ch == '9')) ? true : false;
}
XRCORE_API CInifile const* pSettings = NULL;
XRCORE_API CInifile const* pSettingsAuth = NULL;

CInifile* CInifile::Create(LPCSTR FsPath, const char* szFileName, BOOL ReadOnly)
{
    return xr_new<CInifile>(FsPath,szFileName, ReadOnly);
}
CInifile * CInifile::Create(LPCSTR szFileName)
{
	IReader*F = XRayBearReader::Create(szFileName);
	auto result= xr_new<CInifile>(F, (const bchar*)0,*BearFileManager::GetPathFile(szFileName));
	XRayBearReader::Destroy(F);
	return result;
}
void CInifile::Destroy(CInifile* ini)
{
    xr_delete(ini);
}

bool sect_pred(const CInifile::Sect* x, LPCSTR val)
{
    return BearString::Compare(*x->Name, val) < 0;
};

bool item_pred(const CInifile::Item& x, LPCSTR val)
{
    if ((!x.first) || (!val)) return x.first < val;
    else return  BearString::Compare(*x.first, val) < 0;
}

//------------------------------------------------------------------------------
//Тело функций Inifile
//------------------------------------------------------------------------------

XRCORE_API BOOL _parseSOC(LPSTR dest, LPCSTR src)
{
	if (src) {
		BOOL bInsideSTR = false;
		while (*src) {
			if (isspace_((u8)*src)) {
				if (bInsideSTR) { *dest++ = *src++; continue; }
				while (*src && isspace_(*src)) src++;
				continue;
			}
			else if (*src == '"') {
				bInsideSTR = !bInsideSTR;
			}
			*dest++ = *src++;
		}
	}
	*dest = 0;
	return 0;
}


XRCORE_API BOOL _parseOther(LPSTR dest, LPCSTR src)
{
    BOOL bInsideSTR = false;
    if (src)
    {
        while (*src)
        {
            if (isspace_((u8)*src))
            {
                if (bInsideSTR)
                {
                    *dest++ = *src++;
                    continue;
                }
                while (*src && isspace_(*src))
                {
                    ++src;
                }
                continue;
            }
            else if (*src == '"')
            {
                bInsideSTR = !bInsideSTR;
            }
            *dest++ = *src++;
        }
    }
    *dest = 0;
    return bInsideSTR;
}

BOOL(*_parse)(LPSTR dest, LPCSTR src) = 0;


XRCORE_API void _decorate(LPSTR dest, LPCSTR src)
{
    if (src)
    {
        BOOL bInsideSTR = false;
        while (*src)
        {
            if (*src == ',')
            {
                if (bInsideSTR) { *dest++ = *src++; }
                else { *dest++ = *src++; *dest++ = ' '; }
                continue;
            }
            else if (*src == '"')
            {
                bInsideSTR = !bInsideSTR;
            }
            *dest++ = *src++;
        }
    }
    *dest = 0;
}
//------------------------------------------------------------------------------

BOOL CInifile::Sect::line_exist(LPCSTR L, LPCSTR* val)
{
    SectCIt A = std::lower_bound(Data.begin(), Data.end(), L, item_pred);
    if (A != Data.end() && BearString::Compare(*A->first, L) == 0)
    {
        if (val) *val = *A->second;
        return TRUE;
    }
    return FALSE;
}
//------------------------------------------------------------------------------

CInifile::CInifile(IReader* F, LPCSTR FsPath, LPCSTR path
#ifndef _EDITOR
                   , allow_include_func_t allow_include_func
#endif
                  )
{
    m_file_name[0] = 0;
    m_flags.zero();
    m_flags.set(eSaveAtEnd, FALSE);
    m_flags.set(eReadOnly, TRUE);
    m_flags.set(eOverrideNames, FALSE);
    Load(F, FsPath, path
#ifndef _EDITOR
         , allow_include_func
#endif
        );
}

CInifile::CInifile(LPCSTR FsPath,LPCSTR szFileName,
                   BOOL ReadOnly,
                   BOOL bLoad,
                   BOOL SaveAtEnd,
	bsize sect_count
#ifndef _EDITOR
                   , allow_include_func_t allow_include_func
#endif
                  )

{
    if (szFileName && strstr(szFileName, "system"))
        Msg("-----loading %s", szFileName);

    m_file_name[0] = 0;
    m_flags.zero();
    if (szFileName)
        BearString::Copy(m_file_name, sizeof(m_file_name), szFileName);

    m_flags.set(eSaveAtEnd, SaveAtEnd);
    m_flags.set(eReadOnly, ReadOnly);

	if (bLoad)
	{
		BearString path = BearFileManager::GetPathFile(m_file_name);
		if (FS.ExistFile(FsPath, szFileName))
		{
			IReader* R = XRayBearReader::Create(FS.Read(FsPath, szFileName));
			if (sect_count)
				DATA.reserve(sect_count);
			Load(R, FsPath, *path
#ifndef _EDITOR
				, allow_include_func
#endif
			);
			XRayBearReader::Destroy(R);
		}
    }
}

CInifile::~CInifile()
{
    if (!m_flags.test(eReadOnly) && m_flags.test(eSaveAtEnd))
    {
        if (!save_as())
            Msg("!Can't save inifile:%s", m_file_name);
    }

    RootIt I = DATA.begin();
    RootIt E = DATA.end();
    for (; I != E; ++I)
        xr_delete(*I);
}

static void insert_item(CInifile::Sect* tgt, const CInifile::Item& I)
{
    CInifile::SectIt_ sect_it = std::lower_bound(tgt->Data.begin(), tgt->Data.end(), *I.first, item_pred);
    if (sect_it != tgt->Data.end() && sect_it->first.equal(I.first))
    {
        sect_it->second = I.second;
        //#ifdef DEBUG
        // sect_it->comment= I.comment;
        //#endif
    }
    else
    {
        tgt->Data.insert(sect_it, I);
    }
}

IC BOOL is_empty_line_now(IReader* F)
{
    char* a0 = (char*)F->pointer() - 4;
    char* a1 = (char*)(F->pointer()) - 3;
    char* a2 = (char*)F->pointer() - 2;
    char* a3 = (char*)(F->pointer()) - 1;

    return (*a0 == 13) && (*a1 == 10) && (*a2 == 13) && (*a3 == 10);
};

void CInifile::Load(IReader* F, LPCSTR FsPath,LPCSTR path
#ifndef _EDITOR
                    , allow_include_func_t allow_include_func
#endif
                   )
{
    R_ASSERT(F);
    Sect* Current = 0;
    string4096 str;
    string4096 str2;

    BOOL bInsideSTR = FALSE;

    while (!F->eof())
    {
        F->r_string(str, sizeof(str));
		XrTrims::Trim(str);
        LPSTR comm = strchr(str, ';');
        LPSTR comm_1 = strchr(str, '/');

        if (comm_1 && (*(comm_1 + 1) == '/') && ((!comm) || (comm && (comm_1 < comm))))
        {
            comm = comm_1;
        }

#ifdef DEBUG
        LPSTR comment = 0;
#endif
        if (comm)
        {
            //."bla-bla-bla;nah-nah-nah"
            char quot = '"';
            bool in_quot = false;

            LPCSTR q1 = strchr(str, quot);
            if (q1 && q1 < comm)
            {
                LPCSTR q2 = strchr(++q1, quot);
                if (q2 && q2 > comm)
                    in_quot = true;
            }

            if (!in_quot)
            {
                *comm = 0;
#ifdef DEBUG
                comment = comm + 1;
#endif
            }
        }

        if (str[0] && (str[0] == '#') && strstr(str, "#include")) //handle includes
        {
            string_path inc_name;
            if (XrTrims::GetItem(str, 1, inc_name, '"'))
            {
                string_path fn;
		
				strconcat(sizeof(fn), fn, path,BEAR_PATH, inc_name);
				BearString path_inc = BearFileManager::GetPathFile(fn);
#ifndef _EDITOR
				if (!allow_include_func || allow_include_func(fn))
#endif
				{
					IReader* R = 0;
					if (FsPath)
					{
							R=	XRayBearReader::Create(FS.Read(FsPath, fn));
					}
					else
					{
						R = XRayBearReader::Create(fn);
					}
					
						Load(R, FsPath, *path_inc
#ifndef _EDITOR
							, allow_include_func
#endif
						);
					XRayBearReader::Destroy(R);



				}
            }
        }
        else if (str[0] && (str[0] == '[')) //new section ?
        {
            // insert previous filled section
            if (Current)
            {
                //store previous section
                RootIt I = std::lower_bound(DATA.begin(), DATA.end(), *Current->Name, sect_pred);
                if ((I != DATA.end()) && ((*I)->Name == Current->Name))
                    Debug.fatal(DEBUG_INFO, "Duplicate section '%s' found.", *Current->Name);
                DATA.insert(I, Current);
            }
            Current = xr_new<Sect>();
            Current->Name = 0;
            // start new section
            R_ASSERT3(strchr(str, ']'), "Bad ini section found: ", str);
            LPCSTR inherited_names = strstr(str, "]:");
            if (0 != inherited_names)
            {
                VERIFY2(m_flags.test(eReadOnly), "Allow for readonly mode only.");
                inherited_names += 2;
                bsize cnt = XrTrims::GetItemCount(inherited_names);
				bsize total_count = 0;
				bsize k = 0;
                for (k = 0; k < cnt; ++k)
                {
                    string512 tmp;
                    XrTrims::GetItem(inherited_names, k, tmp);
                    Sect& inherited_section = r_section(tmp);
                    total_count += inherited_section.Data.size();
                }

                Current->Data.reserve(Current->Data.size() + total_count);

                for (k = 0; k < cnt; ++k)
                {
                    string512 tmp;
                    XrTrims::GetItem(inherited_names, k, tmp);
                    Sect& inherited_section = r_section(tmp);
                    for (SectIt_ it = inherited_section.Data.begin(); it != inherited_section.Data.end(); it++)
                        insert_item(Current, *it);
                }
            }
            *strchr(str, ']') = 0;
		
             BearString::ToLower(str + 1);
			 Current->Name = str + 1;
        }
        else // name = value
        {
            if (Current)
            {
                string4096 value_raw;
                char* name = str;
                char* t = strchr(name, '=');
                if (t)
                {
                    *t = 0;
                    XrTrims::Trim(name);
                    ++t;
                    BearString::Copy(value_raw, sizeof(value_raw), t);
                    bInsideSTR = _parse(str2, value_raw);
                    if (bInsideSTR)//multiline str value
                    {
                        while (bInsideSTR)
                        {
							BearString::Contact(value_raw, sizeof(value_raw), "\r\n");
                            string4096 str_add_raw;
                            F->r_string(str_add_raw, sizeof(str_add_raw));
                            R_ASSERT2(
                                BearString::GetSize(value_raw) + BearString::GetSize(str_add_raw) < sizeof(value_raw),
                                make_string(
                                    "Incorrect inifile format: section[%s], variable[%s]. Odd number of quotes (\") found, but should be even.",
                                    Current->Name.c_str(),
                                    name
                                )
                            );
							BearString::Contact(value_raw, sizeof(value_raw), str_add_raw);
                            bInsideSTR = _parse(str2, value_raw);
                            if (bInsideSTR)
                            {
                                if (is_empty_line_now(F))
									BearString::Contact(value_raw, sizeof(value_raw), "\r\n");
                            }
                        }
                    }
                }
                else
                {
					XrTrims::Trim(name);
                    str2[0] = 0;
                }

                Item I;
                I.first = (name[0] ? name : NULL);
                I.second = (str2[0] ? str2 : NULL);
                //#ifdef DEBUG
                // I.comment = m_flags.test(eReadOnly)?0:comment;
                //#endif

                if (m_flags.test(eReadOnly))
                {
                    if (*I.first) insert_item(Current, I);
                }
                else
                {
                    if (
                        *I.first
                        || *I.second
                        //#ifdef DEBUG
                        // || *I.comment
                        //#endif
                    )
                        insert_item(Current, I);
                }
            }
        }
    }
    if (Current)
    {
        RootIt I = std::lower_bound(DATA.begin(), DATA.end(), *Current->Name, sect_pred);
        if ((I != DATA.end()) && ((*I)->Name == Current->Name))
            Debug.fatal(DEBUG_INFO, "Duplicate section '%s' found.", *Current->Name);
        DATA.insert(I, Current);
    }
}

void CInifile::save_as(IWriter& writer, bool bcheck) const
{
    string4096 temp, val;
    for (RootCIt r_it = DATA.begin(); r_it != DATA.end(); ++r_it)
    {
		BearString::Printf(temp, "[%s]", (*r_it)->Name.c_str());
        writer.w_string(temp);
        if (bcheck)
        {
            BearString::Printf(temp, "; %d %d %d", (*r_it)->Name._get()->dwCRC,
                       (*r_it)->Name._get()->dwReference,
                       (*r_it)->Name._get()->dwLength);
            writer.w_string(temp);
        }

        for (SectCIt s_it = (*r_it)->Data.begin(); s_it != (*r_it)->Data.end(); ++s_it)
        {
            const Item& I = *s_it;
            if (*I.first)
            {
                if (*I.second)
                {
                    _decorate(val, *I.second);
                    // only name and value
                    BearString::Printf(temp, "%8s%-32s = %-32s", " ", I.first.c_str(), val);
                }
                else
                {
                    // only name
                    BearString::Printf(temp, "%8s%-32s = ", " ", I.first.c_str());
                }
            }
            else
            {
                // no name, so no value
                temp[0] = 0;
            }
			XrTrims::TrimRight(temp);
            if (temp[0]) writer.w_string(temp);
        }
        writer.w_string(" ");
    }
}

bool CInifile::save_as(LPCSTR new_fname)
{
	if (new_fname)
	{


		// save if needed
		if (new_fname && new_fname[0])
			BearString::Copy(m_file_name, sizeof(m_file_name), new_fname);

		R_ASSERT(m_file_name&&m_file_name[0]);
		auto fs = bear_new<BearFileStream>();
		if (!fs->Open(new_fname, fs->M_Write))
		{
			bear_free(fs);
		}
		IWriter*F = XRayBearWriter::Create(fs);

		save_as(*F);
		XRayBearWriter::Destroy(F);
	}
	else
	{
		IWriter*F = XRayBearWriter::Create(FS.Write(TEXT("%user%"), m_file_name,0));
		save_as(*F);
		XRayBearWriter::Destroy(F);
	}
    return (true);
}

BOOL CInifile::section_exist(LPCSTR S)const
{
    RootCIt I = std::lower_bound(DATA.begin(), DATA.end(), S, sect_pred);
    return (I != DATA.end() && BearString::Compare(*(*I)->Name, S) == 0);
}

BOOL CInifile::line_exist(LPCSTR S, LPCSTR L)const
{
    if (!section_exist(S)) return FALSE;
    Sect& I = r_section(S);
    SectCIt A = std::lower_bound(I.Data.begin(), I.Data.end(), L, item_pred);
    return (A != I.Data.end() && BearString::Compare(*A->first, L) == 0);
}

bsize CInifile::line_count(LPCSTR Sname)const
{
    Sect& S = r_section(Sname);
    SectCIt I = S.Data.begin();
	bsize C = 0;
    for (; I != S.Data.end(); I++) if (*I->first) C++;
    return C;
}

bsize CInifile::section_count()const
{
    return DATA.size();
}


//--------------------------------------------------------------------------------------
CInifile::Sect& CInifile::r_section(const shared_str& S)const { return r_section(*S); }
BOOL CInifile::line_exist(const shared_str& S, const shared_str& L)const { return line_exist(*S, *L); }
bsize CInifile::line_count(const shared_str& S)const { return line_count(*S); }
BOOL CInifile::section_exist(const shared_str& S)const { return section_exist(*S); }

//--------------------------------------------------------------------------------------
// Read functions
//--------------------------------------------------------------------------------------
CInifile::Sect& CInifile::r_section(LPCSTR S)const
{
    char section[256];
    BearString::Copy(section, sizeof(section), S);
	BearString::ToLower(section);
    RootCIt I = std::lower_bound(DATA.begin(), DATA.end(), section, sect_pred);
    if (!(I != DATA.end() && BearString::Compare(*(*I)->Name, section) == 0))
    {

        //g_pStringContainer->verify();

        //string_path ini_dump_fn, path;
        //strconcat (sizeof(ini_dump_fn), ini_dump_fn, Core.ApplicationName, "_", Core.UserName, ".ini_log");
        //
        //FS.update_path (path, "$logs$", ini_dump_fn);
        //IWriter* F = FS.w_open_ex(path);
        //save_as (*F);
        //F->w_string ("shared strings:");
        //g_pStringContainer->dump(F);
        //FS.w_close (F);

        Debug.fatal(DEBUG_INFO, "Can't open section '%s'. Please attach [*.ini_log] file to your bug report", S);
    }
    return **I;
}

LPCSTR CInifile::r_string(LPCSTR S, LPCSTR L)const
{
    Sect const& I = r_section(S);
    SectCIt A = std::lower_bound(I.Data.begin(), I.Data.end(), L, item_pred);
    if (A != I.Data.end() && BearString::Compare(*A->first, L) == 0) return *A->second;
    else
        Debug.fatal(DEBUG_INFO, "Can't find variable %s in [%s]", L, S);
    return 0;
}

shared_str CInifile::r_string_wb(LPCSTR S, LPCSTR L)const
{
    LPCSTR _base = r_string(S, L);

    if (0 == _base) return shared_str(0);

    string4096 _original;
    BearString::Copy(_original, sizeof(_original), _base);
	bsize _len = BearString::GetSize(_original);
    if (0 == _len) return shared_str("");
    if ('"' == _original[_len - 1]) _original[_len - 1] = 0; // skip end
    if ('"' == _original[0]) return shared_str(&_original[0] + 1); // skip begin
    return shared_str(_original);
}

u8 CInifile::r_u8(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    return u8(atoi(C));
}

u16 CInifile::r_u16(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    return u16(atoi(C));
}

u32 CInifile::r_u32(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    return u32(atoi(C));
}

u64 CInifile::r_u64(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
#ifndef _EDITOR
    return _strtoui64(C, NULL, 10);
#else
    return (u64)_atoi64(C);
#endif
}

s64 CInifile::r_s64(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    return _atoi64(C);
}

s8 CInifile::r_s8(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    return s8(atoi(C));
}

s16 CInifile::r_s16(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    return s16(atoi(C));
}

s32 CInifile::r_s32(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    return s32(atoi(C));
}

float CInifile::r_float(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    return float(atof(C));
}

Fcolor CInifile::r_fcolor(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    Fcolor V = {0, 0, 0, 0};
    BearString::Scanf(C, "%f,%f,%f,%f", &V.r, &V.g, &V.b, &V.a);
    return V;
}

u32 CInifile::r_color(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    u32 r = 0, g = 0, b = 0, a = 255;
	BearString::Scanf(C, "%d,%d,%d,%d", &r, &g, &b, &a);
    return XrColor::color_rgba(r, g, b, a);
}

Ivector2 CInifile::r_ivector2(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    Ivector2 V = {0, 0};
	BearString::Scanf(C, "%d,%d", &V.x, &V.y);
    return V;
}

Ivector3 CInifile::r_ivector3(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    Ivector V = {0, 0, 0};
	BearString::Scanf(C, "%d,%d,%d", &V.x, &V.y, &V.z);
    return V;
}

Ivector4 CInifile::r_ivector4(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    Ivector4 V = {0, 0, 0, 0};
	BearString::Scanf(C, "%d,%d,%d,%d", &V.x, &V.y, &V.z, &V.w);
    return V;
}

Fvector2 CInifile::r_fvector2(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    Fvector2 V = {0.f, 0.f};
	BearString::Scanf(C, "%f,%f", &V.x, &V.y);
    return V;
}

Fvector3 CInifile::r_fvector3(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    Fvector3 V = {0.f, 0.f, 0.f};
	BearString::Scanf(C, "%f,%f,%f", &V.x, &V.y, &V.z);
    return V;
}

Fvector4 CInifile::r_fvector4(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    Fvector4 V = {0.f, 0.f, 0.f, 0.f};
	BearString::Scanf(C, "%f,%f,%f,%f", &V.x, &V.y, &V.z, &V.w);
    return V;
}

BOOL CInifile::r_bool(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    VERIFY2(
        BearString::GetSize(C) <= 5,
        make_string(
            "\"%s\" is not a valid bool value, section[%s], line[%s]",
            C,
            S,
            L
        )
    );
    char B[8];
    strncpy_s(B, sizeof(B), C, 7);
    B[7] = 0;
    BearString::ToLower(B);
    return IsBOOL(B);
}

CLASS_ID CInifile::r_clsid(LPCSTR S, LPCSTR L)const
{
    LPCSTR C = r_string(S, L);
    return XrClsid::String2Clsid(C);
}

int CInifile::r_token(LPCSTR S, LPCSTR L, const xr_token* token_list)const
{
    LPCSTR C = r_string(S, L);
    for (int i = 0; token_list[i].name; i++)
        if (!_stricmp(C, token_list[i].name))
            return token_list[i].id;
    return 0;
}

BOOL CInifile::r_line(LPCSTR S, bsize L, const char** N, const char** V)const
{
    Sect& SS = r_section(S);
    if (L >= SS.Data.size() || L < 0) return FALSE;
    for (SectCIt I = SS.Data.begin(); I != SS.Data.end(); I++)
        if (!(L--))
        {
            *N = *I->first;
            *V = *I->second;
            return TRUE;
        }
    return FALSE;
}

BOOL CInifile::r_line(const shared_str& S, bsize L, const char** N, const char** V)const
{
    return r_line(*S, L, N, V);
}

//--------------------------------------------------------------------------------------------------------
// Write functions
//--------------------------------------------------------------------------------------
void CInifile::w_string(LPCSTR S, LPCSTR L, LPCSTR V, LPCSTR comment)
{
    R_ASSERT(!m_flags.test(eReadOnly));

    // section
    string256 sect;
    _parse(sect, S);
    BearString::ToLower(sect);

    if (!section_exist(sect))
    {
        // create _new_ section
        Sect* NEW = xr_new<Sect>();
        NEW->Name = sect;
        RootIt I = std::lower_bound(DATA.begin(), DATA.end(), sect, sect_pred);
        DATA.insert(I, NEW);
    }

    // parse line/value
    string4096 line;
    _parse(line, L);
    string4096 value;
    _parse(value, V);

    // duplicate & insert
    Item I;
    Sect& data = r_section(sect);
    I.first = (line[0] ? line : 0);
    I.second = (value[0] ? value : 0);

    //#ifdef DEBUG
    // I.comment = (comment?comment:0);
    //#endif
    SectIt_ it = std::lower_bound(data.Data.begin(), data.Data.end(), *I.first, item_pred);

    if (it != data.Data.end())
    {
        // Check for "first" matching
        if (0 == BearString::Compare(*it->first, *I.first))
        {
            BOOL b = m_flags.test(eOverrideNames);
            R_ASSERT2(b, make_string("name[%s] already exist in section[%s]", line, sect).c_str());
            *it = I;
        }
        else
        {
            data.Data.insert(it, I);
        }
    }
    else
    {
        data.Data.insert(it, I);
    }
}
void CInifile::w_u8(LPCSTR S, LPCSTR L, u8 V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%d", V);
    w_string(S, L, temp, comment);
}
void CInifile::w_u16(LPCSTR S, LPCSTR L, u16 V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%d", V);
    w_string(S, L, temp, comment);
}
void CInifile::w_u32(LPCSTR S, LPCSTR L, u32 V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%d", V);
    w_string(S, L, temp, comment);
}

void CInifile::w_u64(LPCSTR S, LPCSTR L, u64 V, LPCSTR comment)
{
    string128 temp;
#ifndef _EDITOR
    _ui64toa_s(V, temp, sizeof(temp), 10);
#else
    _ui64toa(V, temp, 10);
#endif
    w_string(S, L, temp, comment);
}

void CInifile::w_s64(LPCSTR S, LPCSTR L, s64 V, LPCSTR comment)
{
    string128 temp;
#ifndef _EDITOR
    _i64toa_s(V, temp, sizeof(temp), 10);
#else
    _i64toa(V, temp, 10);
#endif
    w_string(S, L, temp, comment);
}

void CInifile::w_s8(LPCSTR S, LPCSTR L, s8 V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%d", V);
    w_string(S, L, temp, comment);
}
void CInifile::w_s16(LPCSTR S, LPCSTR L, s16 V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%d", V);
    w_string(S, L, temp, comment);
}
void CInifile::w_s32(LPCSTR S, LPCSTR L, s32 V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%d", V);
    w_string(S, L, temp, comment);
}
void CInifile::w_float(LPCSTR S, LPCSTR L, float V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%f", V);
    w_string(S, L, temp, comment);
}
void CInifile::w_fcolor(LPCSTR S, LPCSTR L, const Fcolor& V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%f,%f,%f,%f", V.r, V.g, V.b, V.a);
    w_string(S, L, temp, comment);
}

void CInifile::w_color(LPCSTR S, LPCSTR L, u32 V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%d,%d,%d,%d", XrColor::color_get_R(V), XrColor::color_get_G(V), XrColor::color_get_B(V), XrColor::color_get_A(V));
    w_string(S, L, temp, comment);
}

void CInifile::w_ivector2(LPCSTR S, LPCSTR L, const Ivector2& V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%d,%d", V.x, V.y);
    w_string(S, L, temp, comment);
}

void CInifile::w_ivector3(LPCSTR S, LPCSTR L, const Ivector3& V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%d,%d,%d", V.x, V.y, V.z);
    w_string(S, L, temp, comment);
}

void CInifile::w_ivector4(LPCSTR S, LPCSTR L, const Ivector4& V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%d,%d,%d,%d", V.x, V.y, V.z, V.w);
    w_string(S, L, temp, comment);
}
void CInifile::w_fvector2(LPCSTR S, LPCSTR L, const Fvector2& V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%f,%f", V.x, V.y);
    w_string(S, L, temp, comment);
}

void CInifile::w_fvector3(LPCSTR S, LPCSTR L, const Fvector3& V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%f,%f,%f", V.x, V.y, V.z);
    w_string(S, L, temp, comment);
}

void CInifile::w_fvector4(LPCSTR S, LPCSTR L, const Fvector4& V, LPCSTR comment)
{
    string128 temp;
    BearString::Printf(temp,  "%f,%f,%f,%f", V.x, V.y, V.z, V.w);
    w_string(S, L, temp, comment);
}

void CInifile::w_bool(LPCSTR S, LPCSTR L, BOOL V, LPCSTR comment)
{
    w_string(S, L, V ? "on" : "off", comment);
}

void CInifile::remove_line(LPCSTR S, LPCSTR L)
{
    R_ASSERT(!m_flags.test(eReadOnly));

    if (line_exist(S, L))
    {
        Sect& data = r_section(S);
        SectIt_ A = std::lower_bound(data.Data.begin(), data.Data.end(), L, item_pred);
        R_ASSERT(A != data.Data.end() && BearString::Compare(*A->first, L) == 0);
        data.Data.erase(A);
    }
}

void CInifile::Initialize()
{
	if (gameVersionController->getGame() == GameVersionController::SOC)
	{
		_parse = _parseSOC;
	}
	else
	{
		_parse = _parseOther;
	}
}

