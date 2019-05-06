using BearBuildTool.Projects;
using System.IO;
using System;
public class stalker :Executable
{
	public stalker(string ProjectPath)
	{

        PCHFile =Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		
		Projects.Public.Add("bear_ui");
		
		Projects.Public.Add("stalkertools");
		Projects.Public.Add("stalkerlua");
		Projects.Public.Add("stalkersound");
		Projects.Public.Add("stalkercdb");
		Projects.Public.Add("stalkerapi");
	    Projects.Public.Add("stalkerrender");
		Projects.Private.Add("stalkermodloader");
		Projects.Private.Add("directx");
		Projects.Public.Add("stalkernetserver");
		
		Projects.Private.Add("discord_api");


        IncludeInProject.Public.Add("stalkercpu_pipe");
		IncludeInProject.Private.Add("stalkergamespy");
		
		LibrariesStatic.Private.Add("winmm.lib");
		LibrariesStatic.Private.Add("vfw32.lib");
		LibrariesStatic.Private.Add("dxgi.lib");
		LibrariesStatic.Private.Add("dxguid.lib");
		LibrariesStatic.Private.Add("dinput8.lib");
		LibrariesStatic.Private.Add("d3d9.lib");
		LibrariesStatic.Private.Add("d3dx9.lib");
		Projects.Private.Add("theora");
		Projects.Private.Add("dplay");
		Projects.Private.Add("openautomate");
		Defines.Private.Add("ENGINE_BUILD");
		
		
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","engine"));
		
		IncludeAutonomousProjects.Add("stalkercpu_pipe");
		IncludeAutonomousProjects.Add("stalker_r1");
        IncludeAutonomousProjects.Add("stalker_r2");
		//IncludeAutonomousProjects.Add("stalker_r5");
		IncludeAutonomousProjects.Add("stalker_r3");
		IncludeAutonomousProjects.Add("stalker_r4");
		
        IncludeAutonomousProjects.Add("stalkersoc");
		IncludeAutonomousProjects.Add("stalkercs");
		IncludeAutonomousProjects.Add("stalkercop");
        IncludeAutonomousProjects.Add("stalkergamespy");
		Console = false;
        ResourceFile = Path.Combine(ProjectPath, "resource", "stalker.rc");
		//ProjectsToDynamicLibrary = true;
	}	
	public override void StartBuild()
	{
		BearBuildTool.Config.Global.UNICODE=false;
		BearBuildTool.Config.Global.WithoutWarning=true;
	}
} 