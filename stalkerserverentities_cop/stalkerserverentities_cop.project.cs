using BearBuildTool.Projects;
using System.IO;
using System;
public class stalkerserverentities_cop :Project
{
	public stalkerserverentities_cop(string ProjectPath)
	{
		OnlyAsStatic=true;
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
		PCHFile=Path.Combine(ProjectPath,"source_script","pch_script.cpp");
		PCHIncludeFile="pch_script.h";
		AddSourceFiles(Path.Combine(ProjectPath,"source"),true);
	}
} 