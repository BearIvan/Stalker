using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkersound :Project
{
	public stalkersound(string ProjectPath)
	{
		//PCHFile=Path.Combine(ProjectPath,"source","stdafx.cpp");
		//PCHIncludeFile="stdafx.h";
		Defines.Private.Add("XRSOUND_EXPORTS");
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
		
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","sound"));

		Projects.Private.Add("stalkertools");
		Projects.Private.Add("stalkercdb");
		Projects.Private.Add("stalkerapi");
		Projects.Private.Add("openal");
		Projects.Private.Add("directx");
		LibrariesStatic.Private.Add("dsound");
		Projects.Private.Add("eax");
        IncludeInProject.Private.Add("stalker_engine");
        Projects.Private.Add("vorbis");
	}
} 