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
		IncludeInProject.Private.Add("stalkergamespy");
		Projects.Private.Add("directx");
		Projects.Private.Add("dplay");
		LibrariesStatic.Private.Add("dxguid.lib");
		LibrariesStatic.Private.Add("Ws2_32.lib");
		LibrariesStatic.Private.Add("dxerr.lib");
	}
} 