using BearBuildTool.Projects;
using System.IO;
using System;
public class luajit :Project
{ 
	public luajit(string ProjectPath)
	{
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		if(BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.Win64||BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.MinGW)
		{
					LibrariesPath.Public.Add(Path.Combine(ProjectPath,"lib","x64"));
					LibrariesDynamic.Add(Path.Combine(ProjectPath,"lib","x64","lua51.dll"));
		}
		else
		{
						LibrariesPath.Public.Add(Path.Combine(ProjectPath,"lib","x86"));
					LibrariesDynamic.Add(Path.Combine(ProjectPath,"lib","x86","lua51.dll"));
		}			
		Defines.Public.Add("LUA_BUILD_AS_DLL");
		
		LibrariesStatic.Public.Add("lua51.lib");
		
	}

} 