#pragma	once


XRPHYSICS_API bool			valid_pos( const Fvector &P );
XRPHYSICS_API const Fbox	&ph_boundaries();
#ifdef DEBUG
class IPhysicsShellHolder;
XRPHYSICS_API std::string dbg_valide_pos_string( const Fvector &pos,const Fbox &bounds, const IPhysicsShellHolder *obj, LPCSTR msg );
XRPHYSICS_API std::string dbg_valide_pos_string( const Fvector &pos, const IPhysicsShellHolder *obj, LPCSTR msg );

#define	DEBUGFATALERROR1_BOUNDARIES2(pos,bounds,obj,msg) DEBUGFATALERROR12(  valid_pos( pos, bounds ), dbg_valide_pos_string( pos, bounds, obj, msg ) )
#define	DEBUGFATALERROR1_BOUNDARIES(pos,bounds,obj)	DEBUGFATALERROR1_BOUNDARIES2(pos,bounds,obj,"	")

#else
#define	DEBUGFATALERROR1_BOUNDARIES(pos,bounds,obj)
#define	DEBUGFATALERROR1_BOUNDARIES2(pos,bounds,obj,msg)
#endif

