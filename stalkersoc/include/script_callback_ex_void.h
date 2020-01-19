////////////////////////////////////////////////////////////////////////////
//	Module 		: script_callback_ex_void.h
//	Created 	: 06.02.2004
//  Modified 	: 11.01.2005
//	Author		: Sergey Zhemeitsev and Dmitriy Iassenev
//	Description : Script callbacks with return value but specialized with void
////////////////////////////////////////////////////////////////////////////

#pragma once

template <>
class CScriptCallbackEx<void> : public CScriptCallbackEx_<void>
 {
    using self = CScriptCallbackEx_<void>;
public:
 template <typename... Args>
    IC void operator()(Args... args) 
    {
        if (self::m_functor)
        {
            VERIFY(self::m_functor.is_valid());
            if (self::m_object.is_valid())
            {
                VERIFY(self::m_object.is_valid());
                luabind::call_function<void>(self::m_functor, self::m_object, args...);
            }
            else
                luabind::call_function<void>(self::m_functor, args...);
        }

    }
    template <typename... Args>
    IC void operator()(Args... args)  const 
    {
        if (self::m_functor)
        {
            VERIFY(self::m_functor.is_valid());
            if (self::m_object.is_valid())
            {
                VERIFY(self::m_object.is_valid());
                luabind::call_function<void>(self::m_functor, self::m_object, args...);
            }
            else
                luabind::call_function<void>(self::m_functor, args...);
        }
    }
};