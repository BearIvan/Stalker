using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerxmlparser :Project
{
	public stalkerxmlparser(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		
		Defines.Private.Add("XRXMLPARSER_EXPORTS"); 

		AddSourceFiles(Path.Combine(ProjectPath,"source"));
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","xmlparser"));

        Projects.Private.Add("stalkerapi");
        Projects.Private.Add("stalkertools");
	}
} 