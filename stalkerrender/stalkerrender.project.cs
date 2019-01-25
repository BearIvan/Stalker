using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerrender :Project
{
	public stalkerrender(string ProjectPath)
	{
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","xrRender"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","xrRender","blenders"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","xrRender","Debug"));
		Include.Public.Add(Path.Combine(ProjectPath,"include","xrRender","Utils"));
	}
} 