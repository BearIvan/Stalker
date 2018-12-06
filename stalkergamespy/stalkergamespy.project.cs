using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkergamespy :Project
{
	public stalkergamespy(string ProjectPath)
	{ 
		PCHFile=Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Defines.Private.Add("XRGAMESPY_EXPORTS");
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"source","gamespy"));
		AddSourceFiles(Path.Combine(ProjectPath,"source"),true);
	}
} 