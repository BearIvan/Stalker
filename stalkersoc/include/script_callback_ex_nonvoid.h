////////////////////////////////////////////////////////////////////////////
//	Module 		: script_callback_ex_non_void.h
//	Created 	: 06.02.2004
//  Modified 	: 11.01.2005
//	Author		: Sergey Zhemeitsev and Dmitriy Iassenev
//	Description : Script callbacks with return value
////////////////////////////////////////////////////////////////////////////

#pragma once

template <typename _return_type>
class CScriptCallbackEx : public CScriptCallbackEx_<_return_type>
{
    using self = CScriptCallbackEx_<_return_type>;
public:
    template <typename... Args>
    IC _return_type operator()(Args... args) 
    {
        if (self::m_functor)
        {
            VERIFY(self::m_functor.is_valid());
            if (self::m_object.is_valid())
            {
                VERIFY(self::m_object.is_valid());
                return (luabind::call_function<_return_type>(self::m_functor, self::m_object, args...));
            }
            else
                return (luabind::call_function<_return_type>(self::m_functor, args...));
        }

        return _return_type();
    }
    template <typename... Args>
    IC _return_type operator()(Args... args)  const 
    {
        if (self::m_functor)
        {
            VERIFY(self::m_functor.is_valid());
            if (self::m_object.is_valid())
            {
                VERIFY(self::m_object.is_valid());
                return (luabind::call_function<_return_type>(self::m_functor, self::m_object, args...));
            }
            else
                return (luabind::call_function<_return_type>(self::m_functor, args...));
        }

        return _return_type();
    }
};