using BearBuildTool.Projects;
using System.IO;
using System;
public class rapidjson :Project
{
	public rapidjson(string ProjectPath)
	{	
		OnlyAsStatic=true;
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
	}
} 