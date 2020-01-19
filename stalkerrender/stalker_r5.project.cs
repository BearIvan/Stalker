using BearBuildTool.Projects;
using System.IO;
using System;
public class stalker_r5 :Project
{
	public stalker_r5(string ProjectPath)
	{
		PCHFile=Path.Combine(ProjectPath,"source","r5","pch.cpp");
		PCHIncludeFile="pch.h";
		Defines.Private.Add("XRRENDER_R5_EXPORTS"); 
		Projects.Private.Add("stalker_engine");
		Projects.Private.Add("bear_graphics");
		Projects.Private.Add("stalkercpu_pipe");
		Projects.Private.Add("stalkerparticles");
		Projects.Private.Add("loki");
		Projects.Private.Add("stalkerparticles");
		AddSourceFiles(Path.Combine(ProjectPath,"source","r5"),true);
  
		
	}
} 