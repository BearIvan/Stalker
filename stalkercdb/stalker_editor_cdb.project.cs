using BearBuildTool.Projects;
using System.IO;
using System;
public class stalker_editor_cdb :Project
{
	public stalker_editor_cdb(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","stdafx.cpp");
		PCHIncludeFile="stdafx.h";
		Defines.Private.Add("XRCDB_EXPORTS"); 
		
		Projects.Private.Add("stalkertools");
		Projects.Private.Add("stalkerapi");
		
		Sources.Add(Path.Combine(ProjectPath,"source","Frustum.cpp"));
		Sources.Add(Path.Combine(ProjectPath,"source","xrCDB_Collector.cpp"));

		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		Include.Private.Add(Path.Combine(ProjectPath,"include","cdb"));
		  
	}
} 