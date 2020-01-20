////////////////////////////////////////////////////////////////////////////
//	Module 		: script_engine_help.cpp
//	Created 	: 01.04.2004
//  Modified 	: 01.04.2004
//	Author		: Dmitriy Iassenev
//	Description : Script Engine help
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include  "BindingsDumper.hpp"
#ifdef DEBUG



void print_help							(lua_State *L)
{
    BindingsDumper dumper;
    BindingsDumper::Options options = {};
    options.ShiftWidth = 4;
    options.IgnoreDerived = true;
    options.StripThis = true;
    BearString str;
    dumper.Dump(L, str, options);
    BearString8192 line;
    bsize count = str.count_element(TEXT('\n'));
    for (bsize i = 0; i < count; i++)
    {
        str.get_element(TEXT('\n'), i, line);
        Msg(line);
    }
  

   // Msg("%s",*str);
}
#else
void print_help							(lua_State *L)
{
	Msg					("! Release build doesn't support lua-help :(");
}
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
