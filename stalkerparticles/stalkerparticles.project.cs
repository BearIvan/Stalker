using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerparticles :Project
{
	public stalkerparticles(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Defines.Private.Add("XR_PARTICLES_EXPORTS"); 
		
	//	Projects.Private.Add("stalkertools");
		Projects.Private.Add("stalker_engine");
		Projects.Private.Add("stalkercpu_pipe");
		/*IncludeInProject.Private.Add("stalkerapi");
		IncludeInProject.Private.Add("stalkercdb");
		IncludeInProject.Private.Add("stalkersound");
		IncludeInProject.Private.Add("stalkerrender");
		IncludeInProject.Private.Add("stalkercpu_pipe");*/
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","particles"));
		
	}
} 