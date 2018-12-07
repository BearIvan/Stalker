using BearBuildTool.Projects;
using System.IO;
using System;
public class crypto :Project
{
	public crypto(string ProjectPath)
	{	
		OnlyAsStatic=true;
		Projects.Private.Add("stalkertools");
		AddSourceFiles(Path.Combine(ProjectPath,"source"), true);
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","crypto"));
		Include.Private.Add(Path.Combine(ProjectPath,"source","openssl"));
	}
} 