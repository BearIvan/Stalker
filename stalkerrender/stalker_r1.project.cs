using BearBuildTool.Projects;
using System.IO;
using System;
public class stalker_r1 :Project
{
	public stalker_r1(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","r1","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Defines.Private.Add("XRRENDER_R1_EXPORTS"); 
		
		Projects.Private.Add("directx");
		Projects.Private.Add("stalker");
		Projects.Private.Add("stalkercpu_pipe");
		Projects.Private.Add("stalkerparticles");
		Projects.Private.Add("openautomate");
		Projects.Private.Add("nvapi");
		Projects.Private.Add("loki");
		if(BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.Win32)
		{
			Projects.Private.Add("atimgpud");
		}
		LibrariesStatic.Private.Add("d3d9.lib");
		LibrariesStatic.Private.Add("d3dx9.lib");
		
		AddSourceFiles(Path.Combine(ProjectPath,"source","base"),true);
		AddSourceFiles(Path.Combine(ProjectPath,"source","directx9"),true);
		AddSourceFiles(Path.Combine(ProjectPath,"source","r1"),true);
  
		
	}
} 