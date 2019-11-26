using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkermodloader :Project
{
	public stalkermodloader(string ProjectPath)
	{ 
		OnlyAsStatic=true;
		Projects.Private.Add("stalkerapi");
		Projects.Private.Add("stalkertools");
        //Projects.Private.Add("bear_ui");
        PCHFile =Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		AddSourceFiles(Path.Combine(ProjectPath,"source"),true);
	}
} 