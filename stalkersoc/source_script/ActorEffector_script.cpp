#include "pch_script.h"
#include "ai_space.h"
#include "script_engine.h"
#include "ActorEffector.h"
#include "engine/ObjectAnimator.h"

void CAnimatorCamEffectorScriptCB::ProcessIfInvalid(Fvector &p, Fvector &d, Fvector &n, float& fFov, float& fFar, float& fAspect)
{
	if(m_bAbsolutePositioning)
	{
		const Fmatrix& m			= m_objectAnimator->XFORM();
		d							= m.k;
		n							= m.j;
		p							= m.c;
	}
}

BOOL CAnimatorCamEffectorScriptCB::Valid()
{
	BOOL res = inherited::Valid();
	if(!res && cb_name.size() )
	{
		luabind::object			fl;
		R_ASSERT							(ai().script_engine().functor(*cb_name,fl));
		luabind::call_function<void>(fl);
		cb_name								= "";
	}
	return res;
}
