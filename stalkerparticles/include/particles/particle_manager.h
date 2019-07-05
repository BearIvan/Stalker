//---------------------------------------------------------------------------
#ifndef particle_managerH
#define particle_managerH
//---------------------------------------------------------------------------
#include "particle_actions.h"

namespace PAPI{
    class CParticleManager: public IParticleManager
    {
		// These are static because all threads access the same effects.
		// All accesses to these should be locked.
		DEFINE_VECTOR				(ParticleEffect*,ParticleEffectVec,ParticleEffectVecIt);
		DEFINE_VECTOR				(ParticleActions*,ParticleActionsVec,ParticleActionsVecIt);
		ParticleEffectVec			effect_vec;
		ParticleActionsVec			m_alist_vec;
    public:
		    						CParticleManager	();
        virtual						~CParticleManager	();
		// Return an index binto the list of particle effects where
		ParticleEffect*				GetEffectPtr		(bint effect_id);
		ParticleActions*			GetActionListPtr	(bint alist_id);

		// create&destroy
		virtual bint					CreateEffect		(bsize max_particles);
		virtual void				DestroyEffect		(bint effect_id);
		virtual bint					CreateActionList	();
		virtual void				DestroyActionList	(bint alist_id);

        // control
        virtual void				PlayEffect			(bint effect_id, bint alist_id);
        virtual void				StopEffect			(bint effect_id, bint alist_id, BOOL deffered=TRUE);

        // update&render
        virtual void				Update				(bint effect_id, bint alist_id, float dt);
        virtual void				Render				(bint effect_id);
        virtual void				Transform			(bint alist_id, const Fmatrix& m, const Fvector& velocity);

        // effect
        virtual void				RemoveParticle		(bint effect_id, bsize p_id);
        virtual void				SetMaxParticles		(bint effect_id, bsize max_particles);
        virtual void				SetCallback			(bint effect_id, OnBirthParticleCB b, OnDeadParticleCB d, void* owner, bsize param);
    	virtual void				GetParticles		(bint effect_id, Particle*& particles, bsize& cnt);
    	virtual bsize					GetParticlesCount	(bint effect_id);

        // action
        virtual ParticleAction*		CreateAction		(PActionEnum action_id);
        virtual bsize					LoadActions			(bint alist_id, IReader& R);
        virtual void				SaveActions			(bint alist_id, IWriter& W);
    };
};
//---------------------------------------------------------------------------
#endif
