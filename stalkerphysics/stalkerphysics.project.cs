using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerphysics :Project
{
	public stalkerphysics(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Defines.Private.Add("XRPHYSICS_EXPORTS");
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
		
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","physics"));

		Projects.Public.Add("ode");
		Projects.Private.Add("stalker");
	}
} 