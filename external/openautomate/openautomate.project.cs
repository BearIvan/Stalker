using BearBuildTool.Projects;
using System.IO;
using System;
public class openautomate :Project
{
	public openautomate(string ProjectPath)
    {
        OnlyAsStatic = true;
        AddSourceFiles(Path.Combine(ProjectPath,"source"));
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","OpenAutomate"));
	}
} 