using BearBuildTool.Projects;
using System.IO;
using System;
public class stalker_r2 :Project
{
	public stalker_r2(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","r2","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Defines.Private.Add("XRRENDER_R1_EXPORTS"); 
		
		Projects.Private.Add("directx");
		Projects.Private.Add("stalker_engine");
		Projects.Private.Add("stalkercpu_pipe");
		Projects.Private.Add("stalkerparticles");
		Projects.Private.Add("openautomate");
		if (BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.Win32 ||
			BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.Win64)
		{
			Projects.Private.Add("nvapi");
		}
		Projects.Private.Add("loki");
		if(BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.Win32)
		{
			Projects.Private.Add("atimgpud");
		}

		LibrariesStatic.Private.Add("d3d9");
		LibrariesStatic.Private.Add("d3dx9");
		
		AddSourceFiles(Path.Combine(ProjectPath,"source","base"),true);
		AddSourceFiles(Path.Combine(ProjectPath,"source","directx9"),true);
		AddSourceFiles(Path.Combine(ProjectPath,"source","r2"),true);
  
		
	}
} 