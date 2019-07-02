using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkersoc:Project
{
	public stalkersoc(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","StdAfx.cpp");
		PCHIncludeFile="StdAfx.h";
		
		//Include.Private.Add(Path.Combine(ProjectPath,"include"));

		Projects.Private.Add("stalkersoc_script");
		
		AddSourceFiles(Path.Combine(ProjectPath,"source"),true);
	}
}