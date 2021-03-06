using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkertools :Project
{
	public stalkertools(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		
		Projects.Public.Add("bear_core");
        Projects.Private.Add("stalkerapi");

        Defines.Private.Add("XRCORE_EXPORTS"); 

		Defines.Public.Add("_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS");
		
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","tools"));
		
		Projects.Private.Add("directx");
		if(BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.Win64||BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.Win32)
		LibrariesStatic.Private.Add("dxerr.lib");
		//Projects.Private.Add("blackbox");
	}
} 