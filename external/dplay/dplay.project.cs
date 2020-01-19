using BearBuildTool.Projects;
using System.IO;
using System;
public class dplay :Project
{ 
	public dplay(string ProjectPath)
	{
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		LibrariesPath.Public.Add(Path.Combine(ProjectPath,"lib"));
		//LibrariesStatic.Public.Add("dxguid8.lib");
	}
} 