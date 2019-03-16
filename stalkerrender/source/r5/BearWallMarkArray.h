
class BearWallMarkArray:public IWallMarkArray 
{
public:
	BearWallMarkArray();
	virtual void Copy(IWallMarkArray &_in) ;

	virtual void	AppendMark(LPCSTR s_textures);
	virtual void	clear();
	virtual bool	empty() ;
	virtual wm_shader GenerateWallmark();
};
