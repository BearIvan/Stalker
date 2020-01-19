using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkercdb :Project
{
	public stalkercdb(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Defines.Private.Add("XRCDB_EXPORTS"); 
		
		Projects.Private.Add("stalkertools");
		Projects.Private.Add("stalkerapi");
		
		
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","cdb"));
		  
		 IncludeInProject.Public.Add("bear_graphics");
		IncludeInProject.Private.Add("stalker_engine");
        IncludeInProject.Private.Add("stalkercpu_pipe");
		IncludeInProject.Private.Add("stalkersound");
        Projects.Private.Add("stalkerrender");
	}
} 