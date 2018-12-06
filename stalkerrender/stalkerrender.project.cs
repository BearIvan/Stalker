using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerrender :Project
{
	public stalkerrender(string ProjectPath)
	{
		Include.Public.Add(Path.Combine(ProjectPath));
		Include.Public.Add(Path.Combine(ProjectPath,"xrRender"));
		Include.Public.Add(Path.Combine(ProjectPath,"xrRender","blenders"));
		Include.Public.Add(Path.Combine(ProjectPath,"xrRender","Debug"));
		Include.Public.Add(Path.Combine(ProjectPath,"xrRender","Utils"));
	}
} 