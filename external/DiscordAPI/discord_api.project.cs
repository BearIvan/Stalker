using BearBuildTool.Projects;
using System.IO;
using System;
public class discord_api :Project
{
	public discord_api(string ProjectPath)
	{	
		OnlyAsStatic=true;
		AddSourceFiles(Path.Combine(ProjectPath,"src"));
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","discord"));
		Projects.Public.Add("rapidjson");
	}
}  