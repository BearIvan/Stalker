#pragma once
#include "../monster_state_manager.h"

class CBurer;

class CStateManagerBurer : public CMonsterStateManager<CBurer> {
	typedef CMonsterStateManager<CBurer> inherited;
public:
					CStateManagerBurer		(CBurer *monster); 
	virtual void	execute					();
	virtual void	setup_substates			();
	virtual void	remove_links			(CObject* object1) { inherited::remove_links(object1);}
};

