#ifndef GAMESPY_PROFILE_STORE
#define GAMESPY_PROFILE_STORE

#include <boost/noncopyable.hpp>
#include "engine/ISheduled.h"
#include "profile_data_types.h"
#include "script_export_space.h"
#include "queued_async_method.h"


//class CGameSpy_ATLAS;

namespace gamespy_profile
{


	class profile_store :
		public ISheduled,
		private boost::noncopyable
	{
	public:
			profile_store() {}
		~profile_store() {}
	
	void			set_current_profile			(int profileId, char const * loginTicket){}
	void			load_current_profile(store_operation_cb progress_indicator_cb,
		store_operation_cb complete_cb) {}
	void			stop_loading				(){}

	virtual void			shedule_Update	(u32 dt){}
	virtual	shared_str		shedule_Name	() const	{ return shared_str("gamespy_sake_updator"); };
	virtual bool			shedule_Needed	()			{ return true; };
	virtual float			shedule_Scale	()			{ return 1.0f; };
	all_awards_t t;
	all_awards_t const &		get_awards		() {return t;}
	all_best_scores_t t1;
	all_best_scores_t const &	get_best_scores	() {return t1;}



	DECLARE_SCRIPT_REGISTER_FUNCTION
};//class profile_store

} //namespace gamespy_profile

typedef gamespy_profile::profile_store	gamespy_profile_profile_store;

add_to_type_list(gamespy_profile_profile_store)
#undef script_type_list
#define script_type_list save_type_list(gamespy_profile_profile_store)

#endif //#ifndef GAMESPY_PROFILE_STORE