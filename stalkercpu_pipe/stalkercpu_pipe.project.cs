using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkercpu_pipe :Project
{
	public stalkercpu_pipe(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Projects.Private.Add("stalkertools");		
		IncludeInProject.Private.Add("stalker_engine");
		IncludeInProject.Private.Add("stalkerapi");
		IncludeInProject.Private.Add("stalkercpu_pipe");
		IncludeInProject.Private.Add("stalkercdb");
		IncludeInProject.Private.Add("stalkersound");
		Projects.Private.Add("stalkerrender");
				 IncludeInProject.Public.Add("bear_graphics");
		IncludeInProject.Public.Add("bear_ui");
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
	}
} 