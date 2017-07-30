///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright 2015 David Martin. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "functions.h"


int main(int argc,char *argv[])
{
	int error=0;

	PrintLogo();

	if(argc < 2)
	{
		PrintUsage();
		return 0;
	}

	error = captureAttr(argv,g_ptrSourceFilename,1);

	exeCheck(g_ptrSourceFilename,1);

	error =captureAttr(argv,g_ptr_ExeFilename,2);

	exeCheck(g_ptr_ExeFilename,2);

	createExecutableName(error, g_ptrSourceFilename,g_ptr_ExeFilename);

	Init();

	LoadSourceFile();

	printf("Assembly %s...\n\n",g_ptrSourceFilename);

	AssmblSourceFile();

	BuildXSE();

	PrintAssmblStats();

	ShutDown();
		


	return 0;

}