using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerapi :Project
{
	public stalkerapi(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Defines.Private.Add("XRAPI_EXPORTS");
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","api"));
	}
} 