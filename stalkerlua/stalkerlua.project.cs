using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerlua :Project
{
	public stalkerlua(string ProjectPath)
	{
		OnlyAsStatic = true;
		Defines.Private.Add("LUABIND_BUILDING");

		Defines.Public.Add("LUABIND_NO_EXCEPTIONS");

		Projects.Public.Add("luajit");
		AddSourceFiles(Path.Combine(ProjectPath,"luabind"),true);
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Projects.Private.Add("stalkertools");
	}
} 