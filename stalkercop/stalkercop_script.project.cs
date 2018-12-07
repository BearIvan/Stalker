using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkercop_script:Project
{
	public stalkercop_script(string ProjectPath)
	{
        OnlyAsStatic = true;
		PCHFile=Path.Combine(ProjectPath,"source_script","pch_script.cpp");
		PCHIncludeFile="pch_script.h";
		Defines.Public.Add("XRGAME_EXPORTS"); 
		
		Include.Public.Add(Path.Combine(ProjectPath,"include"));

        IncludeInProject.Private.Add("stalkergamespy");
        Projects.Public.Add("cs");
        Projects.Public.Add("crypto");
       Projects.Public.Add("stalker");
		Projects.Public.Add("dplay");
		Projects.Public.Add("directx");
		Projects.Public.Add("loki");
		Projects.Public.Add("openautomate");
		Projects.Public.Add("stalkerphysics");
		Projects.Public.Add("stalkerxmlparser");
		AddSourceFiles(Path.Combine(ProjectPath,"source_script"),true);
	}
}