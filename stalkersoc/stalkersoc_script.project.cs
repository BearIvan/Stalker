using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkersoc_script:Project
{
	public stalkersoc_script(string ProjectPath)
	{
        OnlyAsStatic = true;
		PCHFile=Path.Combine(ProjectPath,"source_script","pch_script.cpp");
		PCHIncludeFile="pch_script.h";
		Defines.Public.Add("XRGAME_EXPORTS"); 
		
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		
		Projects.Public.Add("stalker_engine");
		Projects.Public.Add("dplay");
		Projects.Public.Add("directx");
		Projects.Public.Add("loki");
		Projects.Public.Add("openautomate");
		Projects.Public.Add("ode");
		Projects.Public.Add("stalkerxmlparser");
		AddSourceFiles(Path.Combine(ProjectPath,"source_script"),true);
	}
}