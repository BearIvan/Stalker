#ifndef PSystemH
#define PSystemH
#pragma once
#include "tools\xrCore.h"
#ifdef XR_PARTICLES_EXPORTS
	#define PARTICLES_API __declspec(dllexport)
#else
	#define PARTICLES_API __declspec(dllimport)
#endif

// Actually this must be < sqrt(MAXFLOAT) since we store this value squared.
#define P_MAXFLOAT	1.0e16f

#ifdef MAXbint
	#define P_MAXbint	MAXbint
#else
	#define P_MAXbint	0x7fffffff
#endif

#define drand48()		::Random.randF()
//#define drand48() (((float) rand())/((float) RAND_MAX))

namespace PAPI{
	class pVector	: public Fvector
	{
	public:
		IC 			pVector		(float ax, float ay, float az)	{ set(ax,ay,az);								}
		IC 			pVector		()								{}
		IC float 	length		() const						{	return  XrMath::sqrt(x*x+y*y+z*z);					}
		IC float 	length2		() const						{	return (x*x+y*y+z*z);						}
		IC float 	operator*	(const pVector &a) const		{	return x*a.x + y*a.y + z*a.z;				}
		IC pVector 	operator*	(const float s) const			{	return pVector(x*s, y*s, z*s);				}
		IC pVector 	operator/	(const float s) const			{	float invs = 1.0f / s;	return pVector(x*invs, y*invs, z*invs);	}
		IC pVector 	operator+	(const pVector& a) const		{	return pVector(x+a.x, y+a.y, z+a.z);		}
		IC pVector 	operator-	(const pVector& a) const		{	return pVector(x-a.x, y-a.y, z-a.z);		}
		IC pVector 	operator-	()								{	x = -x;	y = -y;	z = -z;	return *this;		}
		IC pVector& operator+=	(const pVector& a)				{	x += a.x;y += a.y;z += a.z;	return *this;	}
		IC pVector& operator-=	(const pVector& a)		 		{	x -= a.x;y -= a.y;z -= a.z;	return *this;	}
		IC pVector& operator*=	(const float a)					{	x *= a;	y *= a;	z *= a;	return *this;		}
		IC pVector& operator/=	(const float a)					{	float b = 1.0f / a;	x *= b;	y *= b;	z *= b;	return *this;		}
		IC pVector& operator=	(const pVector& a)				{	x = a.x;y = a.y;z = a.z;return *this;		}
		IC pVector 	operator^	(const pVector& b) const		{	return pVector(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);		}
	};
	// A single particle
	struct Rotation
	{
		float		x;
	};
	struct Particle
	{
		enum{
			ANIMATE_CCW	= (1<<0),
		};
		Rotation	rot;	// 4
		pVector		pos;	// 12
		pVector		posB;   // 12
		pVector		vel;    // 12  	
		pVector		size;   // 12
		u32			color;	// 4
		float		age;	// 4       
		u16			frame;	// 2
		Flags16		flags;	// 2
	};                  	// = 64

	typedef void (* OnBirthParticleCB)	(void* owner, bsize param, PAPI::Particle& P, bsize idx);
	typedef void (* OnDeadParticleCB)	(void* owner, bsize param, PAPI::Particle& P, bsize idx);
	//////////////////////////////////////////////////////////////////////
	// Type codes for domains
	enum PDomainEnum
	{
		PDPoint 	= 0,	// Single pobint
		PDLine 		= 1,	// Line segment
		PDTriangle 	= 2,	// Triangle
		PDPlane 	= 3,	// Arbitrarily-oriented plane
		PDBox 		= 4,	// Axis-aligned box
		PDSphere 	= 5,	// Sphere
		PDCylinder 	= 6,	// Cylinder
		PDCone 		= 7,	// Cone
		PDBlob 		= 8,	// Gaussian blob
		PDDisc 		= 9,	// Arbitrarily-oriented disc
		PDRectangle = 10,	// Rhombus-shaped planar region
		domain_enum_force_dword = bsize(-1)
	};
	//////////////////////////////////////////////////////////////////////
	// Type codes for all actions
	enum PActionEnum
	{
		PAAvoidID,			// Avoid entering the domain of space.
		PABounceID,			// Bounce particles off a domain of space.
		PACallActionListID_obsolette,	// 
		PACopyVertexBID,	// Set the secondary position from current position.
		PADampingID,		// Dampen particle velocities.
		PAExplosionID,		// An Explosion.
		PAFollowID,			// Accelerate toward the previous particle in the effect.
		PAGravitateID,		// Accelerate each particle toward each other particle.
		PAGravityID,		// Acceleration in the given direction.
		PAJetID,			// 
		PAKillOldID,		// 
		PAMatchVelocityID,	// 
		PAMoveID,			// 
		PAOrbitLineID,		// 
		PAOrbitPointID,		// 
		PARandomAccelID,	// 
		PARandomDisplaceID,	// 
		PARandomVelocityID,	// 
		PARestoreID,		// 
		PASinkID,			// 
		PASinkVelocityID,	// 
		PASourceID,			// 
		PASpeedLimitID,		// 
		PATargetColorID,	// 
		PATargetSizeID,		// 
		PATargetRotateID,	// 
		PATargetRotateDID,	// 
		PATargetVelocityID,	// 
		PATargetVelocityDID,// 
		PAVortexID,			// 
        PATurbulenceID,     //
        PAScatterID, 	    //
		action_enum_force_dword = bsize(-1)
	};
    struct ParticleAction;

    class IParticleManager{
    public:
		    						IParticleManager	(){}
        virtual						~IParticleManager	(){}

		// create&destroy
		virtual bint					CreateEffect		(bsize max_particles)=0;
		virtual void				DestroyEffect		(bint effect_id)=0;
		virtual bint					CreateActionList	()=0;
		virtual void				DestroyActionList	(bint alist_id)=0;

        // control
        virtual void				PlayEffect			(bint effect_id, bint alist_id)=0;
        virtual void				StopEffect			(bint effect_id, bint alist_id, BOOL deffered=TRUE)=0;

        // update&render
        virtual void				Update				(bint effect_id, bint alist_id, float dt)=0;
        virtual void				Render				(bint effect_id)=0;
        virtual void				Transform			(bint alist_id, const Fmatrix& m, const Fvector& velocity)=0;

        // effect
        virtual void				RemoveParticle		(bint effect_id, bsize p_id)=0;
        virtual void				SetMaxParticles		(bint effect_id, bsize max_particles)=0;
        virtual void				SetCallback			(bint effect_id, OnBirthParticleCB b, OnDeadParticleCB d, void* owner, bsize param)=0;
    	virtual void				GetParticles		(bint effect_id, Particle*& particles, bsize& cnt)=0;
    	virtual bsize					GetParticlesCount	(bint effect_id)=0;
        
        // action
        virtual ParticleAction*		CreateAction		(PActionEnum type)=0;
        virtual bsize					LoadActions			(bint alist_id, IReader& R)=0;
        virtual void				SaveActions			(bint alist_id, IWriter& W)=0;
    };

    PARTICLES_API IParticleManager* ParticleManager		();
};
#endif //PSystemH
