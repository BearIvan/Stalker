using BearBuildTool.Projects;
using System.IO;
using System;
public class stalker_r4 :Project
{
	public stalker_r4(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","r4","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Defines.Private.Add("XRRENDER_R4_EXPORTS"); 
		Defines.Private.Add("USE_DX11");
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
		LibrariesStatic.Private.Add("dxgi.lib");
		LibrariesStatic.Private.Add("dxguid.lib");
		LibrariesStatic.Private.Add("d3d11.lib");
		LibrariesStatic.Private.Add("d3dx11.lib");
		LibrariesStatic.Private.Add("d3dcompiler.lib");
		
	
		AddSourceFiles(Path.Combine(ProjectPath,"source","base"),true);
		AddSourceFiles(Path.Combine(ProjectPath,"source","directx10"),true);
		AddSourceFiles(Path.Combine(ProjectPath,"source","r4"),true);
  
		
	}
} 