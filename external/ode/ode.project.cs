using BearBuildTool.Projects;
using System.IO;
using System;
public class ode :Project
{
	public ode(string ProjectPath)
    {
        OnlyAsStatic = true;
        Defines.Public.Add("dSINGLE");
		AddSourceFiles(Path.Combine(ProjectPath,"source"),true);
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","ode"));
        Projects.Private.Add("bear_core");
    }
} 