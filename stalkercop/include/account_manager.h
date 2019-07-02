#ifndef ACCOUNT_MANAGER
#define ACCOUNT_MANAGER

#include <boost/noncopyable.hpp>
#include "mixed_delegate.h"
#include "script_export_space.h"
#include "queued_async_method.h"


namespace gamespy_gp
{

struct new_profile_data
{
	shared_str	nick;
	shared_str	unique_nick;
	shared_str	email;
	shared_str	password;
	//shared_str	cd_key;
}; //struct new_account_data

typedef mixed_delegate<
	void (bool, char const *),
	account_operation_cb_tag>			account_operation_cb;

typedef mixed_delegate<
	void (u32 const, char const *),
	account_profiles_cb_tag>			account_profiles_cb;

typedef mixed_delegate<
	void (bool, char const *),
	found_emails_cb_tag>				found_email_cb;

typedef mixed_delegate<
	void (u32 const, char const *),
	suggest_nicks_cb_tag>				suggest_nicks_cb;


class account_manager : private boost::noncopyable
{
public:
	account_manager() {}
	~account_manager() {}
	bool		is_get_account_profiles_active() const { return false; };
	bool		is_suggest_unique_nicks_active() const { return false; }
	void		search_for_email(char const * email,
		found_email_cb found_email_cb) {}
	typedef	xr_vector<shared_str>	suggested_nicks_t;
	typedef xr_vector<char const*>	suggested_nicks_ptrs_t;
	void		suggest_unique_nicks(char const * unick,suggest_nicks_cb sncb) {}
	void		stop_suggest_unique_nicks() {}
	suggested_nicks_ptrs_t temp1;
	suggested_nicks_ptrs_t const &	get_suggested_unicks() const { return temp1; };
	void		create_profile(char const * nick,
		char const * unique_nick,
		char const * email,
		char const * password,
		account_operation_cb opcb) {}
	void		delete_profile(account_operation_cb dpcb) {}



	void		get_account_profiles(char const * email,
		char const * password,
		account_profiles_cb profiles_cb) {}
	void		stop_fetching_account_profiles() {}

	typedef xr_vector<char const*>	profiles_nicks_ptrs_t;
	profiles_nicks_ptrs_t temp;
	profiles_nicks_ptrs_t const &	get_found_profiles() const {
		return temp;
	};
	bool		verify_unique_nick(char const * unick) { return false; }
	bool		verify_email(char const * email) { return false; }
	bool		verify_password(char const * pass) { return false; }
	char const*	get_verify_error_descr() const { return ""; }

	bool		is_email_searching_active() const { return false; }

	void		stop_searching_email() {}
	void		reinit_email_searching() {}
			
	DECLARE_SCRIPT_REGISTER_FUNCTION
}; //class account_manager


} //namespace gamespy_gp

typedef gamespy_gp::account_manager			gamespy_gp_account_manager;
typedef gamespy_gp::suggest_nicks_cb		gamespy_gp_suggest_nicks_cb;
typedef gamespy_gp::account_operation_cb	gamespy_gp_account_operation_cb;
typedef gamespy_gp::account_profiles_cb		gamespy_gp_account_profiles_cb;
typedef gamespy_gp::found_email_cb			gamespy_gp_found_email_cb;

add_to_type_list(gamespy_gp_suggest_nicks_cb)
#undef script_type_list
#define script_type_list save_type_list(gamespy_gp_suggest_nicks_cb)

add_to_type_list(gamespy_gp_account_operation_cb)
#undef script_type_list
#define script_type_list save_type_list(gamespy_gp_account_operation_cb)

add_to_type_list(gamespy_gp_account_profiles_cb)
#undef script_type_list
#define script_type_list save_type_list(gamespy_gp_account_profiles_cb)

add_to_type_list(gamespy_gp_found_email_cb)
#undef script_type_list
#define script_type_list save_type_list(gamespy_gp_found_email_cb)

add_to_type_list(gamespy_gp_account_manager)
#undef script_type_list
#define script_type_list save_type_list(gamespy_gp_account_manager)


#endif //#ifndef ACCOUNT_MANAGER
