using BearBuildTool.Projects;
using System.IO;
using System;
public class cs :Project
{ 
	public cs(string ProjectPath)
	{
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		//LibrariesStatic.Public.Add("dxguid8.lib");
	}
} 