using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerserverentities_cop :Project
{
	public stalkerserverentities_cop(string ProjectPath)
	{
		
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
	}
} 