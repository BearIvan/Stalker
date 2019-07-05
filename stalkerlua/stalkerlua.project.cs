using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerlua :Project
{
	public stalkerlua(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"luabind","pch.cpp");
		PCHIncludeFile="pch.h";
		
		Defines.Private.Add("LUABIND_BUILDING");
		Defines.Private.Add("LUA_LIB");
		Defines.Public.Add("LUA_BUILD_AS_DLL");
			Projects.Public.Add("luajit");
		AddSourceFiles(Path.Combine(ProjectPath,"luabind"),true);
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Projects.Private.Add("stalkertools");
	}
} 