using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkernetserver :Project
{
	public stalkernetserver(string ProjectPath)
    {
        PCHFile = Path.Combine(ProjectPath, "source", "stdafx.cpp");
        PCHIncludeFile = "stdafx.h";
        Defines.Private.Add("XR_NETSERVER_EXPORTS");
		Include.Private.Add(Path.Combine(ProjectPath,"include","netserver"));
		AddSourceFiles(Path.Combine(ProjectPath,"source"));
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Projects.Private.Add("stalkertools");
		IncludeInProject.Private.Add("stalkerlua");
		//IncludeInProject.Private.Add("stalkergamespy");
		Projects.Private.Add("directx");
		Projects.Private.Add("dplay");
		LibrariesStatic.Private.Add("dxguid.lib");
		
		if (BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.Win64 || BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.Win32)
		{
			LibrariesStatic.Private.Add("dxerr.lib");
		}
		else if (BearBuildTool.Config.Global.Platform == BearBuildTool.Config.Platform.MinGW)
		{
			LibrariesStatic.Private.Add("dxerr8");
		}
	}
} 