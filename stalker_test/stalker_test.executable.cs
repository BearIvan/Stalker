using BearBuildTool.Projects;
using System.IO;
using System;
public class stalker_test :Executable
{
	public stalker_test(string ProjectPath)
	{
		Projects.Public.Add("stalkertools");
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
	//	Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Console = true;
   
	}	
	public override void StartBuild()
	{
		BearBuildTool.Config.Global.UNICODE=false;
       /* BearBuildTool.Config.Global.WithoutWarning = true;*/

	}
} 