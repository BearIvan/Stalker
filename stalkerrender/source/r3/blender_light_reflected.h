#pragma once

class CBlender_accum_reflected : public IBlender  
{
public:
	virtual		LPCSTR		getComment()	{ return "INTERNAL: accumulate reflected light";	}
	virtual		BOOL		canBeDetailed()	{ return FALSE;	}
	virtual		BOOL		canBeLMAPped()	{ return FALSE;	}

	virtual		void		Compile			(CBlender_Compile& C);

	CBlender_accum_reflected();
	virtual ~CBlender_accum_reflected();
};

class CBlender_accum_reflected_msaa : public IBlender  
{
public:
	virtual		LPCSTR		getComment()	{ return "INTERNAL: accumulate reflected light";	}
	virtual		BOOL		canBeDetailed()	{ return FALSE;	}
	virtual		BOOL		canBeLMAPped()	{ return FALSE;	}

	virtual		void		Compile			(CBlender_Compile& C);

	CBlender_accum_reflected_msaa();
	virtual ~CBlender_accum_reflected_msaa();
	virtual   void    SetDefine( LPCSTR Name1, LPCSTR Definition1 )
		{
		this->Name = Name1;
		this->Definition = Definition1;
		}
	LPCSTR Name;
	LPCSTR Definition;
};
