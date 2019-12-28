#ifndef LOGIN_MANAGER
#define LOGIN_MANAGER


#include <boost/noncopyable.hpp>
#include "mixed_delegate.h"
#include "account_manager.h"
#include "script_export_space.h"
#include "login_manager.h"
#include "queued_async_method.h"


namespace gamespy_gp
{

struct profile
{
	
	profile() {}
	char const *		unique_nick		() const { return "NONE"; };
	bool const			online			() const { return false; };
	//copy constructor is valid
	DECLARE_SCRIPT_REGISTER_FUNCTION
};//struct profile

//typedef XrFastDelegate<void (profile const *, shared_str const &)>	login_operation_cb;
typedef mixed_delegate<void (profile const *, char const *), mdut_login_operation_cb_tag>	login_operation_cb;

class login_manager : private boost::noncopyable
{
public:
	login_manager() {}
	~login_manager() {}

	void				login(char const * email,
		char const * nick,
		char const * password,
		login_operation_cb logincb) {}
	void				stop_login() {}

	void				login_offline(char const * nick, login_operation_cb logincb) {}
	void				logout() {}

	void				save_email_to_registry(char const * email) {}
	char const *		get_email_from_registry() {
		return "";
	}

	void				save_password_to_registry(char const * password) {}
	char const *		get_password_from_registry() { return ""; }

	void				save_nick_to_registry(char const * nickname)
	{}
	char const *		get_nick_from_registry() { return ""; }

	void				save_remember_me_to_registry(bool remember) {}
	bool				get_remember_me_from_registry() { return false; }


	void				set_unique_nick(char const * new_unick,
		login_operation_cb logincb) {}
	void				stop_setting_unique_nick() {}

	profile const *		get_current_profile() const { return &m_current_profile; };

	void				delete_profile_obj() {}	//deletes m_current_profile and clears m_login_operation_cb
	void				forgot_password(char const * url)
	{}
	DECLARE_SCRIPT_REGISTER_FUNCTION
private:
	profile m_current_profile;
}; //class login_manager

} //namespace gamespy_gp

typedef gamespy_gp::profile				gamespy_gp_profile;
typedef gamespy_gp::login_operation_cb	gamespy_gp_login_operation_cb;
typedef gamespy_gp::login_manager		gamespy_gp_login_manager;

add_to_type_list(gamespy_gp_profile)
#undef script_type_list
#define script_type_list save_type_list(gamespy_gp_profile)

add_to_type_list(gamespy_gp_login_operation_cb)
#undef script_type_list
#define script_type_list save_type_list(gamespy_gp_login_operation_cb)

add_to_type_list(gamespy_gp_login_manager)
#undef script_type_list
#define script_type_list save_type_list(gamespy_gp_login_manager)



#endif //#ifndef LOGIN_MANAGER