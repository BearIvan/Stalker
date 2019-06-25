#pragma once
#define Msg(a,...) 	BearCore::BearLog::Printf(a,##__VA_ARGS__)
#define VPUSH(a)	((a).x), ((a).y), ((a).z)
IC void 			Log(LPCSTR msg)
{
	Msg(msg);
}
IC void 			Log(LPCSTR msg, LPCSTR			dop)
{
	Msg("%s %s",msg,dop);
}
IC void 			Log(LPCSTR msg, u32			dop)
{
	Msg("%s %u", msg, dop);
}
IC void 			Log(LPCSTR msg, int  			dop)
{
	Msg("%s %d", msg, dop);
}
IC void 			Log(LPCSTR msg, float			dop)
{
	Msg("%s %f", msg, dop);
}
IC void 			Log(LPCSTR msg, const Fvector& dop)
{
	Msg("%s (%f,%f,%f)", msg, VPUSH(dop));
}
IC void 			Log(LPCSTR msg, const Fmatrix& dop)
{
		Msg("	%s:\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n", msg,
			dop.i.x, dop.i.y, dop.i.z, dop._14_,
			dop.j.x, dop.j.y, dop.j.z, dop._24_,
			dop.k.x, dop.k.y, dop.k.z, dop._34_,
			dop.c.x, dop.c.y, dop.c.z, dop._44_);
}
IC void 			LogWinErr(LPCSTR msg, int32 			err_code)
{
	Msg("%s: %d", msg, err_code);
}