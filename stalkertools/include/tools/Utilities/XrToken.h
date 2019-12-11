#pragma once
struct xr_rtoken
{
    shared_str	name;
    int	   	id;
           	xr_rtoken	(LPCSTR _nm, int _id){name=_nm;id=_id;}
public:
    void	rename		(LPCSTR _nm)		{name=_nm;}
    bool	equal		(LPCSTR _nm)		{return (0==BearString::Compare(*name,_nm));}
};
DEFINE_VECTOR	(xr_rtoken,RTokenVec,RTokenVecIt);

struct  xr_token
{
	LPCSTR name;
	int id;
};

IC LPCSTR get_token_name(xr_token* tokens, int key)
{
	for (int k = 0; tokens[k].name; k++)
		if (key == tokens[k].id) return tokens[k].name;
	return "";
}

IC int get_token_id(xr_token* tokens, LPCSTR key)
{
	for (int k = 0; tokens[k].name; k++)
		if (_stricmp(tokens[k].name, key) == 0)
			return tokens[k].id;
	return -1;
}

struct  xr_token2
{
	LPCSTR name;
	LPCSTR info;
	int id;
};

// generic

