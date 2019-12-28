
#pragma once

typedef void crashhandler(void);
typedef void on_dialog(bool before);

class XRCORE_API XrDebug
{
private:
    crashhandler* handler;
    on_dialog* m_on_dialog;

public:
    void Initialize();
    void Destroy();

public:
    crashhandler* get_crashhandler() { return handler; };
    void set_crashhandler(crashhandler* _handler) { handler = _handler; };

    on_dialog* get_on_dialog() { return m_on_dialog; }
    void set_on_dialog(on_dialog* on_dialog) { m_on_dialog = on_dialog; }


    void gather_info(const char* expression, const char* description, const char* argument0, const char* argument1, const char* file, int line, const char* function, LPSTR assertion_info, unsigned int assertion_info_size);
    template <int count>
    inline void gather_info(const char* expression, const char* description, const char* argument0, const char* argument1, const char* file, int line, const char* function, char(&assertion_info)[count])
    {
        gather_info(expression, description, argument0, argument1, file, line, function, assertion_info, count);
    }
	IC const bchar*error2string(int32 hr)
	{
		static BearString64 error2string_str;
		BearString::Printf(error2string_str, "%Error Core:0x%x", hr);
		return error2string_str;
	}
    void fail(const char* e1, const char* file, int line, const char* function, bool& ignore_always);
    void fail(const char* e1, const std::string& e2, const char* file, int line, const char* function, bool& ignore_always);
    void fail(const char* e1, const char* e2, const char* file, int line, const char* function, bool& ignore_always);
    void fail(const char* e1, const char* e2, const char* e3, const char* file, int line, const char* function, bool& ignore_always);
    void fail(const char* e1, const char* e2, const char* e3, const char* e4, const char* file, int line, const char* function, bool& ignore_always);
    void error(int32 code, const char* e1, const char* file, int line, const char* function, bool& ignore_always);
    void error(int32 code, const char* e1, const char* e2, const char* file, int line, const char* function, bool& ignore_always);
    void fatal(const char* file, int line, const char* function, const char* F, ...);
    void backend(const char* reason, const char* expression, const char* argument0, const char* argument1, const char* file, int line, const char* function, bool& ignore_always);
    void do_exit(const std::string& message);
};

// warning
// this function can be used for debug purposes only
IC std::string __cdecl make_string(LPCSTR format, ...)
{
    va_list args;
    va_start(args, format);

    char temp[4096];
    BearString::PrintfVa(temp,format,args);

    return std::string(temp);
}

extern XRCORE_API XrDebug Debug;

//XRCORE_API void LogStackTrace(LPCSTR header);

#include "XrDebugDefines.h"
