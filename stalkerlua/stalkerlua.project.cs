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
		AddSourceFiles(Path.Combine(ProjectPath,"src"),true);
		AddSourceFiles(Path.Combine(ProjectPath,"luabind"),true);
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","lua"));
		Include.Private.Add(Path.Combine(ProjectPath,"dynasm"));
		Projects.Private.Add("stalkertools");
	}
} 