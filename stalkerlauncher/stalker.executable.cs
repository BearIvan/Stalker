using BearBuildTool.Projects;
using System.IO;
using System;
public class stalker :Executable
{
	public stalker(string ProjectPath)
	{
		Projects.Private.Add("stalker_engine");
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
        /*   if (BearBuildTool.Config.Global.Configure == BearBuildTool.Config.Configure.Debug)
        {*/
            Console = true;
     /*   }*/
	}	
} 