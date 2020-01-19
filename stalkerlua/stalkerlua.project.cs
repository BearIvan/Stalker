using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerlua :Project
{
	public stalkerlua(string ProjectPath)
	{
		
		Defines.Private.Add("LUABIND_BUILDING");
		Defines.Private.Add("LUA_LIB");

		Defines.Public.Add("LUABIND_NO_EXCEPTIONS");

		Defines.Public.Add("LUABIND_DYNAMIC_LINK");
		Defines.Public.Add("LUA_BUILD_AS_DLL");
		Projects.Public.Add("luajit");
		AddSourceFiles(Path.Combine(ProjectPath,"luabind"),true);
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Projects.Private.Add("stalkertools");
	}
} 