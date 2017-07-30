///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright 2015 David Martin. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "global.h"

// ---- Functions -----------------------------------------------------------------------------
/********************************************************************
*     exeCheck
*     checks to see if the extention is at the end, if not add one.
*     Arguments: fileName
*	  Return: Void
/*******************************************************************
*/
void createExecutableName(int error,char* sourcefileName,char* exefilename)
{
	int offset = strrchr(sourcefileName,'.') - sourcefileName;
	strcpy_s(exefilename,strlen(sourcefileName)+1,sourcefileName);
	exefilename[offset] = '\0';
	strcat_s(exefilename,sizeof(exefilename)+5, EXEC_FILE_EXT);
}

/********************************************************************
*     exeCheck
*     checks to see if the extention is at the end, if not add one.
*     Arguments: fileName
*	  Return: Void
/*******************************************************************
*/
void exeCheck(char fileName[],int column)
{
char* ptr = 0;
	if(column == 1)
	{
		if(!strstr(fileName,SOURCE_FILE_EXT))
		{
		strcat_s(fileName,strlen(fileName)+6,(SOURCE_FILE_EXT));
		}
	}
	else if(column == 2)
	{		
		if(!strstr(fileName,EXEC_FILE_EXT))
		{
		strcat_s(fileName,strlen(fileName)+5,EXEC_FILE_EXT);
		}

	}
	


}


/********************************************************************
*     captureFileName
*     graps filename from the command line args
*     Arguments: cLine, argument column number
*	  Return: Void
/*******************************************************************
*/
int captureAttr(char* cLine[],char fileName[],int column)
{	
	int length = 0;
	
	if(column == 1)
	{
		if(cLine[1])
		{
			strcpy_s ( fileName,strlen(cLine[1])+1,cLine[1]);
			_strupr_s ( fileName, strlen(fileName)+1 );
		}
		else
			return 1;
		
	}
	else if(column == 2)
	{
		if(cLine[2])
		{
			
			strcpy_s ( fileName,strlen(cLine[ 2 ])+1, cLine[ 2 ] );
			_strupr_s ( fileName,strlen(fileName)+1 );
		}
		else
			return 2;
		
	}
	return -1;
		
} 




/******************************************************************************************
    *
    *   GetInstrByMnemonic ()
    *
    *   Returns a pointer to the instruction definition corresponding to the specified mnemonic.
    */

    int GetInstrByMnemonic(char* pstrMnemonic, InstrLookup* pInstr)
{	int test1=0, test2=0;
	for(int iCurrInstrIndex = 0; iCurrInstrIndex < MAX_INSTRUCTION_LOOKUP_COUNT; ++iCurrInstrIndex)
		if(strcmp(g_InstructionTable[iCurrInstrIndex].pstrInstr,pstrMnemonic) == 0)
		{
			pInstr->iOpCode = g_InstructionTable[iCurrInstrIndex].iOpcode;
			pInstr->iOpCount = g_InstructionTable[iCurrInstrIndex].iOpcount;
			pInstr->OpList = g_InstructionTable[iCurrInstrIndex].OpList;
			test1 = strlen(pInstr->pstrMnemonic);
			test2 = strlen(g_InstructionTable[iCurrInstrIndex].pstrInstr);
			strcpy_s(pInstr->pstrMnemonic,strlen(g_InstructionTable[iCurrInstrIndex].pstrInstr)+1, g_InstructionTable[iCurrInstrIndex].pstrInstr);
			return true;
		}
	return false;
}
/******************************************************************************************
    *
    *   SkipToNextLine ()
    *
    *   Skips to the next line in the character stream. Returns false the end of the source code
    *   has been reached, TRUE otherwise.
    */
int SkipToNextLine()
{
	++g_Lexer.iCurrSourceLine;

	if(g_Lexer.iCurrSourceLine >= g_iSourceCodeSize)
		return false;

	g_Lexer.iIndex0 = 0;
	g_Lexer.iIndex1 = 0;

	g_Lexer.iCurrLexState = LEX_STATE_NO_STRING;

	return true;
}
   
    /******************************************************************************************
    *
    *   SetOpType ()
    *
    *   Sets the operand type for the specified operand in the specified instruction.
    */

    void SetOpType ( int iInstrIndex, int iOpIndex, OpTypes iOpType )
    {
        g_InstructionTable [ iInstrIndex ].OpList [ iOpIndex ] = iOpType;
    }
 /******************************************************************************************
    *
    *   AddInstrLookup ()
    *
    *   Adds an instruction to the instruction lookup table.
    */

   int AddinstructionRef( char * pstrInstr, int iOpcode, int iOpCount)
{
	static int iInstructionIndex = 0;

	if(iInstructionIndex > MAX_INSTRUCTION_LOOKUP_COUNT)
		return -1;

	strcpy_s(g_InstructionTable[iInstructionIndex].pstrInstr,pstrInstr);
	_strupr_s(g_InstructionTable[iInstructionIndex].pstrInstr);

	g_InstructionTable[iInstructionIndex].iOpcode = iOpcode;
	g_InstructionTable[iInstructionIndex].iOpcount = iOpCount;

	g_InstructionTable[iInstructionIndex].OpList = (OpTypes*)malloc(iOpCount*sizeof(OpTypes));

	int IReternInstructionIndex = iInstructionIndex;

	++iInstructionIndex;

	return IReternInstructionIndex;
}

 /******************************************************************************************
    *
    *   ExitOnError ()
    *
    *   Prints an error message and exits.
    */

    void ExitOnError(char* pstrErrorMssg)
{
	printf("Fatal Error: %s.\n", pstrErrorMssg);

	Exit();
}

    /******************************************************************************************
    *
    *   ExitOnCodeError ()
    *
    *   Prints an error message relating to the source code and exits.
    */

    void ExitOnCodeError(char* pstrErrorMssg)
{
	printf("Error: %s.\n\n", pstrErrorMssg);
	printf("Line %d\n", g_Lexer.iCurrSourceLine);

	char pstrSourceLine[MAX_SOURCE_LINE_SIZE];
	strcpy_s(pstrSourceLine, g_ppstrSourceCode[g_Lexer.iCurrSourceLine]);

	for(unsigned int iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrSourceLine);++iCurrCharIndex)
		if(pstrSourceLine[iCurrCharIndex] == '\t')
			pstrSourceLine[iCurrCharIndex] = ' ';

	printf("%s", pstrSourceLine);

	for(unsigned int iCurrSpace = 0;  iCurrSpace < g_Lexer.iIndex0;++iCurrSpace)
		printf(" ");
	printf("^\n");
	printf("Could not assemble %s.", g_ptr_ExeFilename);

	Exit();

}

	/******************************************************************************************
	*
	*	ExitOnCharExpectedError ()
	*
	*	Exits because a specific character was expected but not found.
	*/

	void ExitOnCharExpectedError(char cChar)
{
	char* pstrErrorMssg = (char*)malloc(strlen("' ' expected"));
	sprintf_s(pstrErrorMssg, sizeof(cChar),"'%c' expected", cChar);
	ExitOnCodeError(pstrErrorMssg);

}

   

    

	/******************************************************************************************
	*
	*	StripComments ()
	*
	*	Strips the comments from a given line of source code, ignoring comment symbols found
	*	inside strings. The new string is shortended to the index of the comment symbol
	*	character.
	*/

	void StripComments(char* pstrSourceLine)
{
	unsigned int iCurrCharIndex;
	int iInString;

	iInString = 0;
	for(iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrSourceLine)-1; ++iCurrCharIndex)
	{
		if(pstrSourceLine[iCurrCharIndex] == '"')
			if(iInString)
				iInString = 0;
			else
				iInString = 1;
		if(pstrSourceLine[iCurrCharIndex] == ';')
		{
			if(!iInString)
			{
				pstrSourceLine[iCurrCharIndex] = '\n';
				pstrSourceLine[iCurrCharIndex + 1] = '\0';
				break;
			}
		}
	}
}

	/******************************************************************************************
	*
	*	IsCharWhitespace ()
	*
	*	Returns a nonzero if the given character is whitespace, or zero otherwise.
	*/

	int IsCharWhitespace ( char cChar )
	{

		if ( cChar == ' ' || cChar == '\t' )
			return true;
		else
			return false;
	}

	

	/******************************************************************************************
	*
	*	IsCharNumeric ()
	*
	*	Returns a nonzero if the given character is numeric, or zero otherwise.
	*/

	int IsCharNumeric ( char cChar )
	{
		if ( cChar >= '0' && cChar <= '9' )
			return true;
		else
			return false;
	}

	/******************************************************************************************
	*
	*	IsCharIdentifier ()
	*
	*	Returns a nonzero if the given character is part of a valid identifier, meaning it's an
	*	alphanumeric or underscore. Zero is returned otherwise.
	*/

	int IsCharIdent(char cChar)
{
	if((cChar >= '0' && cChar <= '9') ||
		(cChar >= 'A' && cChar <= 'Z') ||
		(cChar >= 'a' && cChar <= 'z') || cChar == '_')
		return true;
	else
		return false;
}

	/******************************************************************************************
	*
	*	IsCharDelimiter ()
	*
	*	Return a nonzero if the given character is a token delimeter, and return zero otherwise
	*/

int IsCharDelimiter(char cChar)
{
	if(cChar == ':' || cChar == ',' || cChar == '"' ||
		cChar == '[' || cChar == ']' || 
		cChar == '{' || cChar == '}' ||
		IsCharWhitespace(cChar) || cChar == '\n')
		return true;
	else
		return false;
}

	/******************************************************************************************
	*
	*	TrimWhitespace ()
	*
	*	Trims whitespace off both sides of a string.
	*/

	void TrimWhitespace(char* pstrSourceLine)
{
	unsigned int iStringLength = strlen(pstrSourceLine);
	unsigned int iPadLength;
	unsigned int iCurrCharIndex;

	if(iStringLength > 1)
	{
		for(iCurrCharIndex = 0; iCurrCharIndex < iStringLength; ++iCurrCharIndex)
		{
			if(!IsCharWhitespace(pstrSourceLine[iCurrCharIndex]))
				break;

			iPadLength = iCurrCharIndex;
			if(iPadLength)
			{
				for(iCurrCharIndex = iPadLength;iCurrCharIndex < iStringLength; ++iCurrCharIndex)
					pstrSourceLine[iCurrCharIndex - iPadLength] = pstrSourceLine[iCurrCharIndex];

				for(iCurrCharIndex = iStringLength - iPadLength; iCurrCharIndex < iStringLength; ++iCurrCharIndex)
					pstrSourceLine[iCurrCharIndex] = ' ';
			}
			for(iCurrCharIndex = iStringLength - 1; iCurrCharIndex > 0; --iCurrCharIndex)
				if(!IsCharWhitespace(pstrSourceLine[iCurrCharIndex]))
				{
					pstrSourceLine[iCurrCharIndex + 1] = '\0';
					break;
				}
		}
	}
}


	/******************************************************************************************
	*
	*	IsStringWhitespace ()
	*
	*	Returns a nonzero if the given string is whitespace, or zero otherwise.
	*/

	int IsStringWhitespace ( char * pstrString )
	{
		return true;
	}

	/******************************************************************************************
	*
	*	IsStringIdentifier ()
	*
	*	Returns a nonzero if the given string is composed entirely of valid identifier
	*	characters and begins with a letter or underscore. Zero is returned otherwise.
	*/

	int IsStringIdent(char* pstrString)
{
	if(!pstrString)
		return false;

	if(strlen(pstrString) == 0)
		return false;

	if(pstrString[0] >= '0' && pstrString[0] <= '9')
		return false;
int test = strlen(pstrString);
	for(unsigned int iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrString);++iCurrCharIndex)
		if(!IsCharIdent(pstrString[iCurrCharIndex]))
			return false;
	return true;
}

	/******************************************************************************************
	*
	*	IsStringInteger ()
	*
	*	Returns a nonzero if the given string is composed entire of integer characters, or zero
	*	otherwise.
	*/

	int IsStringInteger(char* pstrString)
{
	if(!pstrString)
		return false;

	if(strlen(pstrString) == 0)
		return false;

	unsigned int iCurrCharIndex;

	for(iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
		if(!IsCharNumeric(pstrString[iCurrCharIndex]) && !(pstrString[iCurrCharIndex] == '-'))
			return false;

	for(iCurrCharIndex = 1; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
		if(pstrString[iCurrCharIndex] == '-')
			return false;

	return true;

}

	/******************************************************************************************
	*
	*	IsStringFloat ()
	*
	*	Returns a nonzero if the given string is composed entire of float characters, or zero
	*	otherwise.
	*/

	int IsStringFloat(char* pstrString)
{
	if(!pstrString)
		return false;

	if(strlen(pstrString) == 0)
		return false;

	unsigned int iCurrCharIndex;

	for(iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrString);++iCurrCharIndex)
		if(!IsCharNumeric(pstrString[iCurrCharIndex]) && !(pstrString[iCurrCharIndex] == '.') && !(pstrString[iCurrCharIndex] == '-'))
			return false;

	int iRadixPointFound = 0;

	for(iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
		if(pstrString[iCurrCharIndex] == '.')
			if(iRadixPointFound)
				return false;
			else
				iRadixPointFound = 1;

	for(iCurrCharIndex = 1; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
		if(pstrString[iCurrCharIndex] == '-')
			return false;

	if(iRadixPointFound)
		return true;
	else
		return false;
}

    /******************************************************************************************
    *
    *   PrintLogo ()
    *
    *   Prints out logo/credits information.
    */

    void PrintLogo()
{
	printf("VASM\n");
	printf("ViperScript Assembler Version %d.%d\n",VERSION_MAJOR,VERSION_MINOR);
	printf("Developed by David Martin\n");
	printf("\n");

}

    /******************************************************************************************
    *
    *   PrintUsage ()
    *
    *   Prints out usage information.
    */

   void PrintUsage()
{
	printf("Usage:\tVASM Source.VASM [Executable.VSE]");
	printf("\n");
	printf("\t-File extentions are not reqired\n");
	printf("Executable name is optional;source name is used by default.\n");
}

    /******************************************************************************************
    *
    *   LoadSourceFile ()
    *
    *   Loads the source file into memory.
    */

    void LoadSourceFile()
{
errno_t err;
	if(fopen_s(&g_pSourceFile,g_ptrSourceFilename,"rb"))
		ExitOnError("Could not open file");

	while(!feof(g_pSourceFile))
		if(fgetc(g_pSourceFile) == '\n')
			++g_iSourceCodeSize;
	++g_iSourceCodeSize;

	fclose(g_pSourceFile);

	if(err = fopen_s(&g_pSourceFile,g_ptrSourceFilename, "r"))
		ExitOnError("Could not open source file");

	if(!(g_ppstrSourceCode = (char**)malloc(g_iSourceCodeSize*sizeof(char*))))
		ExitOnError("Could not allocate space for source code");

	for(int iCurrLineIndex = 0; iCurrLineIndex < g_iSourceCodeSize; ++iCurrLineIndex)
	{
		if(!(g_ppstrSourceCode[iCurrLineIndex] = (char*)malloc(MAX_SOURCE_LINE_SIZE + 1)))
			ExitOnError("Could not allocate for space line");

		fgets(g_ppstrSourceCode[iCurrLineIndex],MAX_SOURCE_LINE_SIZE, g_pSourceFile);

		StripComments(g_ppstrSourceCode[iCurrLineIndex]);
		TrimWhitespace(g_ppstrSourceCode[iCurrLineIndex]);

		int iNewLineIndex = strlen(g_ppstrSourceCode[iCurrLineIndex]) - 1;
		if(g_ppstrSourceCode[iCurrLineIndex][iNewLineIndex] != '\n')
		{
			g_ppstrSourceCode[iCurrLineIndex][iNewLineIndex + 1] = '\n';
			g_ppstrSourceCode[iCurrLineIndex][iNewLineIndex + 2] = '\0';
		}
	}

	fclose(g_pSourceFile);
}

    /******************************************************************************************
    *
    *   InitInstrTable ()
    *
    *   Initializes the master instruction lookup table.
    */

    void initInstructionTable()
{
	//TODO
	int iInstructionIndex= 0;

	iInstructionIndex = AddinstructionRef("MOV",INSTR_MOV,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("ADD",INSTR_ADD,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("SUB",INSTR_SUB,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("MUL",INSTR_MUL,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("DIV",INSTR_DIV,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("MOD",INSTR_MOD,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("EXP",INSTR_EXP,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("NEG",INSTR_NEG,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	
	iInstructionIndex = AddinstructionRef("INC",INSTR_INC,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	
	iInstructionIndex = AddinstructionRef("DEC",INSTR_DEC,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	
	iInstructionIndex = AddinstructionRef("AND",INSTR_AND,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("OR",INSTR_OR,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("XOR",INSTR_XOR,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("NOT",INSTR_NOT,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	

	iInstructionIndex = AddinstructionRef("SHR",INSTR_SHR,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("SHL",INSTR_SHL,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
///concat
	iInstructionIndex = AddinstructionRef("CBW",INSTR_CBW,2);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	//get char
	iInstructionIndex = AddinstructionRef("CDQ",INSTR_CDQ,3);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG | OP_FLAG_TYPE_MEM_STRING);
	SetOpType(iInstructionIndex,2,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
///set char
	iInstructionIndex = AddinstructionRef("SETnn",INSTR_SET,3);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG | OP_FLAG_TYPE_MEM_INT);
	SetOpType(iInstructionIndex,2,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG | OP_FLAG_TYPE_MEM_STRING);

	iInstructionIndex = AddinstructionRef("JMP",INSTR_JMP,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_LINE_LABEL);
	
	iInstructionIndex = AddinstructionRef("JE",INSTR_JE,3);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,2,OP_FLAG_TYPE_LINE_LABEL);

	iInstructionIndex = AddinstructionRef("JNE",INSTR_JNE,3);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,2,OP_FLAG_TYPE_LINE_LABEL);

	iInstructionIndex = AddinstructionRef("JG",INSTR_JG,3);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,2,OP_FLAG_TYPE_LINE_LABEL);

	iInstructionIndex = AddinstructionRef("JL",INSTR_JL,3);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,2,OP_FLAG_TYPE_LINE_LABEL);

	iInstructionIndex = AddinstructionRef("JLE",INSTR_JLE,3);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,2,OP_FLAG_TYPE_LINE_LABEL);

	iInstructionIndex = AddinstructionRef("JGE",INSTR_JGE,3);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,1,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	SetOpType(iInstructionIndex,2,OP_FLAG_TYPE_LINE_LABEL);

	iInstructionIndex = AddinstructionRef("PUSH",INSTR_PUSH,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	
	iInstructionIndex = AddinstructionRef("POP",INSTR_POP,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);
	
	iInstructionIndex = AddinstructionRef("CALL",INSTR_CALL,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_FUNC_NAME);

	iInstructionIndex = AddinstructionRef("RET",INSTR_RET,0);
	
//call host
	iInstructionIndex = AddinstructionRef("INT",INSTR_INT,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_HOST_API_CALL);
	
	iInstructionIndex = AddinstructionRef("WAIT",INSTR_WAIT,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	iInstructionIndex = AddinstructionRef("END",INSTR_END,1);
	SetOpType(iInstructionIndex,0,OP_FLAG_TYPE_MEM_INT | OP_FLAG_TYPE_FLOAT | OP_FLAG_TYPE_MEM_STRING | OP_FLAG_TYPE_MEM_REF | OP_FLAG_TYPE_REG);

	




}

   
	/******************************************************************************************
	*
	*	AddString ()
	*
	*	Adds a string to a linked list, blocking duplicate entries
	*/

	int AddString ( DList<char*>* pList, char * pstrString )
	{
		int counter=0;
		DLItr<char*> pItr = pList->GetIt();

		for ( pItr.Start(); pItr.Valid(); pItr.Forth() )
		{
			

			if(strcmp((char*)pItr.Item(),pstrString)==0)
			return counter;
		counter++;
		}

		char* pstrStringNode = (char*)malloc(strlen(pstrString)+1);
	strcpy_s(pstrStringNode,strlen(pstrString)+1,pstrString);

		
		pList->Append(pstrStringNode);
		return pList->count;
	}



	/******************************************************************************************
	*
	*	Init ()
	*
	*	Initializes the assembler.
	*/

	void Init ()
	{
        // Initialize the master instruction lookup table

        initInstructionTable();

        // Initialize tables

        //InitLinkedList ( & g_SymbolTable );
        //InitLinkedList ( & g_LabelTable );
        //InitLinkedList ( & g_FuncTable );
		//InitLinkedList ( & g_StringTable );
        //InitLinkedList ( & g_HostAPICallTable );
	}

    /******************************************************************************************
    *
    *   ShutDown ()
    *
    *   Frees any dynamically allocated resources back to the system.
    */

    void ShutDown()
	{
		for(int iCurrLineIndex = 0; iCurrLineIndex < g_iSourceCodeSize;++iCurrLineIndex)
			free(g_ppstrSourceCode[iCurrLineIndex]);

		free(g_ppstrSourceCode);

		if(g_pInstrStream)
		{
			for(int iCurrInstrIndex = 0; iCurrInstrIndex < g_iInstrStreamSize;++iCurrInstrIndex)
				if(g_pInstrStream[iCurrInstrIndex].pOpList)
					free(g_pInstrStream[iCurrInstrIndex].pOpList);
			free(g_pInstrStream);
		}

		//FreeLinkedList(g_RefSymbols);
		//FreeLinkedList(g_RefLabels);
		//FreeLinkedList(g_RefFunctions);
		//FreeLinkedList(g_RefHostInit);
	}

    /******************************************************************************************
    *
    *   ResetLexer ()
    *
    *   Resets the lexer to the beginning of the source file by setting the current line and
    *   indices to zero.
    */

    void ResetLexer()
{
	g_Lexer.iCurrSourceLine = 0;
	g_Lexer.iIndex0 = 0;
	g_Lexer.iIndex1 = 0;
	g_Lexer.CurrToken = TOKEN_TYPE_INVALID;
	g_Lexer.iCurrLexState = LEX_STATE_NO_STRING;

}

    /******************************************************************************************
    *
    *   GetNextToken ()
    *
    *   Extracts and returns the next token from the character stream. Also makes a copy of
    *   the current lexeme for use with GetCurrLexeme ().
    */

    Token GetNextToken()
{
	g_Lexer.iIndex0 = g_Lexer.iIndex1;

	if(g_Lexer.iIndex0 >= strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceLine]))
	{
		if(!SkipToNextLine())
		return END_OF_TOKEN_STREAM;
	}

	if(g_Lexer.iCurrLexState == LEX_STATE_END_STRING)
		g_Lexer.iCurrLexState = LEX_STATE_NO_STRING;

	if(g_Lexer.iCurrLexState != LEX_STATE_IN_STRING)
	{
		while(true)
		{
			if(!IsCharWhitespace(g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex0]))
				break;

			++g_Lexer.iIndex0;
		}
	}

	g_Lexer.iIndex1 = g_Lexer.iIndex0;

	while(true)
	{
		if(g_Lexer.iCurrLexState == LEX_STATE_IN_STRING)
		{
			if(g_Lexer.iIndex1 >= strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceLine]))
			{
				g_Lexer.CurrToken = TOKEN_TYPE_INVALID;
				return g_Lexer.CurrToken;
			}
			if(g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex0] == '\\')
			{	g_Lexer.iIndex0 += 2;
			return g_Lexer.iIndex0 += 2;
			}
			if(g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex1] == '"')
				break;
			++g_Lexer.iIndex1;
		}
		else
		{
			if(g_Lexer.iIndex1 >= strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceLine]))
				break;
			if(IsCharDelimiter(g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex1]))
				break;
			++g_Lexer.iIndex1;
		}
	}if(g_Lexer.iIndex1 - g_Lexer.iIndex0 == 0)
			++g_Lexer.iIndex1;

		unsigned int iCurrDestIndex = 0;
		for(unsigned int iCurrSourceIndex = g_Lexer.iIndex0; iCurrSourceIndex < g_Lexer.iIndex1; ++iCurrSourceIndex)
		{
			if(g_Lexer.iCurrLexState == LEX_STATE_IN_STRING)
				if(g_ppstrSourceCode[g_Lexer.iCurrSourceLine][iCurrSourceIndex] == '\\')
					++iCurrSourceIndex;
			g_Lexer.pstrCurrLexeme[iCurrDestIndex] = g_ppstrSourceCode[g_Lexer.iCurrSourceLine][iCurrSourceIndex];
			++iCurrDestIndex;
		}
		g_Lexer.pstrCurrLexeme[iCurrDestIndex] = '\0';
		if(g_Lexer.iCurrLexState != LEX_STATE_IN_STRING)
			_strupr_s(g_Lexer.pstrCurrLexeme);

		g_Lexer.CurrToken = TOKEN_TYPE_INVALID;

		if(strlen(g_Lexer.pstrCurrLexeme) > 1 || g_Lexer.pstrCurrLexeme[0] != '"')
		{
			if(g_Lexer.iCurrLexState == LEX_STATE_IN_STRING)
			{
				g_Lexer.CurrToken = TOKEN_TYPE_STRING;
				return TOKEN_TYPE_STRING;
			}
		}

		if(strlen(g_Lexer.pstrCurrLexeme) == 1)
		{
			switch(g_Lexer.pstrCurrLexeme[0])
			{
			case '"':
				switch(g_Lexer.iCurrLexState)
				{
				case LEX_STATE_NO_STRING:
					g_Lexer.iCurrLexState = LEX_STATE_IN_STRING;
					break;
				case LEX_STATE_IN_STRING:
					g_Lexer.iCurrLexState = LEX_STATE_END_STRING;
					break;
				}
				g_Lexer.CurrToken = TOKEN_TYPE_QUOTE;
				break;

			case ',':
				g_Lexer.CurrToken = TOKEN_TYPE_COMMA;
				break;
			case ':':
				g_Lexer.CurrToken = TOKEN_TYPE_COLON;
				break;
			case '[':
				g_Lexer.CurrToken = TOKEN_TYPE_OPEN_BRACKET;
				break;
			case ']':
				g_Lexer.CurrToken = TOKEN_TYPE_CLOSE_BRACKET;
				break;
			case '{':
				g_Lexer.CurrToken = TOKEN_TYPE_OPEN_BRACE;
				break;
			case '}':
				g_Lexer.CurrToken = TOKEN_TYPE_CLOSE_BRACE;
				break;
			case '\n':
				g_Lexer.CurrToken = TOKEN_TYPE_NEWLINE;
				break;
			}
		}
		if(IsStringInteger(g_Lexer.pstrCurrLexeme))
			g_Lexer.CurrToken = TOKEN_TYPE_INT;

		if(IsStringFloat(g_Lexer.pstrCurrLexeme))
			g_Lexer.CurrToken = TOKEN_TYPE_FLOAT;
		if(IsStringIdent(g_Lexer.pstrCurrLexeme))
			g_Lexer.CurrToken = TOKEN_TYPE_IDENT;

		if(strcmp(g_Lexer.pstrCurrLexeme, "SETSTACKSIZE") == 0)
			g_Lexer.CurrToken = TOKEN_TYPE_SETSTACKSIZE;

		if(strcmp(g_Lexer.pstrCurrLexeme, "SETPRIORITY") == 0)
			g_Lexer.CurrToken = TOKEN_TYPE_SETPRIORITY;

		if(strcmp(g_Lexer.pstrCurrLexeme, "VAR") == 0)
			g_Lexer.CurrToken = TOKEN_TYPE_VAR;
		if(strcmp(g_Lexer.pstrCurrLexeme, "FUNC") == 0)
			g_Lexer.CurrToken = TOKEN_TYPE_FUNC;
		if(strcmp(g_Lexer.pstrCurrLexeme, "PARAM") == 0)
			g_Lexer.CurrToken = TOKEN_TYPE_PARAM;
		if(strcmp(g_Lexer.pstrCurrLexeme, "_RETVAL") == 0)
			g_Lexer.CurrToken = TOKEN_TYPE_REG_RETVAL;
		InstrLookup Instr;
		if(GetInstrByMnemonic(g_Lexer.pstrCurrLexeme, &Instr))
			g_Lexer.CurrToken = TOKEN_TYPE_INSTR;
		
		
		return g_Lexer.CurrToken;
	
}

    /******************************************************************************************
    *
    *   GetCurrLexeme ()
    *
    *   Returns a pointer to the current lexeme.
    */

    char* GetCurrLexeme()
{
	return g_Lexer.pstrCurrLexeme;
	
}

    /******************************************************************************************
    *
    *   GetLookAheadChar ()
    *
    *   Returns the look-ahead character. which is the first character of the next lexeme in
    *   the stream.
    */

    char GetLookAheadChar()
{
	int iCurrSourceLine = g_Lexer.iCurrSourceLine;
	unsigned int iIndex = g_Lexer.iIndex1;

	if(g_Lexer.iCurrLexState != LEX_STATE_IN_STRING)
	{
		while(true)
		{
			if(iIndex >= strlen(g_ppstrSourceCode[iCurrSourceLine]))
			{
				iCurrSourceLine += 1;
				if(iCurrSourceLine >= g_iSourceCodeSize)
					return 0;
				iIndex = 0;
			}
			if(!IsCharWhitespace(g_ppstrSourceCode[iCurrSourceLine][iIndex]))
				break;

			++iIndex;
		}
	}
	return g_ppstrSourceCode[iCurrSourceLine][iIndex];

}

    

   

    /******************************************************************************************
    *
    *   GetFuncByName ()
    *
    *   Returns a FuncNode structure pointer corresponding to the specified name.
    */

    FuncNode* GetFuncByName(char* pstrName)
{
	if(!g_RefFunctions.getCount())
		return NULL;
	DLItr<FuncNode*> ptr = g_RefFunctions.GetIt();

	for(ptr.Start();ptr.Valid();ptr.Forth())
	{
		if(strcmp(ptr.Item()->pstrName,pstrName) == 0)
			return ptr.Item();
	}

	return NULL;
}

    /******************************************************************************************
    *
    *   AddFunc ()
    *
    *   Adds a function to the function table.
    */

    int AddFunc(char * pstrName, int iEntryPoint)
{
	if(GetFuncByName(pstrName))
		return -1;

	FuncNode * pNewFunc = (FuncNode*)malloc(sizeof(FuncNode));

	strcpy_s(pNewFunc->pstrName, pstrName);

	pNewFunc->iEntryPoint = iEntryPoint;

	g_RefFunctions.Append(pNewFunc);

	pNewFunc->iIndex = g_RefFunctions.getCount();

	return g_RefFunctions.getCount();

}

    /******************************************************************************************
    *
    *   SetFuncInfo ()
    *
    *   Fills in the remaining fields not initialized by AddFunc ().
    */

    void SetFuncInfo(char* pstrName,int iParamCount, int iLocalDataSize)
{
	FuncNode* pFunc = GetFuncByName(pstrName);
	pFunc->iParamCount = iParamCount;
	pFunc->iLocalDataSize = iLocalDataSize;
}

    /******************************************************************************************
    *
    *   GetLabelByIdent ()
    *
    *   Returns a pointer to the label structure corresponding to the identifier and function
    *   index.
    */

    LabelNode * GetLabelByIdent(char* pstrIdent, int iFuncIndex)
{

	if(!g_RefLabels.getCount())
		return NULL;

	DLItr<LabelNode*> pCurrNode = g_RefLabels.GetIt();

	for(pCurrNode.Start(); pCurrNode.Valid(); pCurrNode.Forth())
	{
		LabelNode* pCurrLabel = (LabelNode*)pCurrNode.Item();

		if(strcmp(pCurrLabel->pstrIdent,pstrIdent) == 0 && pCurrLabel->iFuncIndex == iFuncIndex)
			return pCurrLabel;
	}

	return NULL;

}

    /******************************************************************************************
    *
    *   AddLabel ()
    *
    *   Adds a label to the label table.
    */

    int AddLabel(char* pstrIdent, int iTargetIndex, int iFuncIndex)
{
	return 0;
}

    /******************************************************************************************
    *
    *   GetSymbolByIdent ()
    *
    *   Returns a pointer to the symbol structure corresponding to the identifier and function
    *   index.
    */

    SymbolNode* GetSymbolByIdent(char* pstrIdent, int iFuncIndex)
{

	if(!g_RefSymbols.getCount())
		return NULL;

	DLItr<SymbolNode*> trans = g_RefSymbols.GetIt();

	for(trans.Start();trans.Valid();trans.Forth())
	{
		if(strcmp(trans.Item()->pstrIdent,pstrIdent) == 0)
			if(trans.Item()->iFuncIndex == iFuncIndex || trans.Item()->iStackIndex >= 0)
				return trans.Item();
	}
	return NULL;
}

	/******************************************************************************************
	*
	*	GetStackIndexByIdent ()
	*
	*	Returns a symbol's stack index based on its identifier and function index.
	*/

	int GetStackIndexByIdent ( char * pstrIdent, int iFuncIndex )
	{
		SymbolNode* pSymbol = GetSymbolByIdent(pstrIdent, iFuncIndex);////problem
return pSymbol->iStackIndex;
	}

	/******************************************************************************************
	*
	*	GetSizeByIndent ()
	*
	*	Returns a variable's size based on its identifier.
	*/

	int GetSizeByIdent ( char * pstrIdent, int iFuncIndex )
	{
		SymbolNode* pSymbol = GetSymbolByIdent(pstrIdent, iFuncIndex);

return pSymbol->iSize;
	}

    /******************************************************************************************
    *
    *   AddSymbol ()
    *
    *   Adds a symbol to the symbol table.
    */

    int AddSymbol ( char * pstrIdent, int iSize, int iStackIndex, int iFuncIndex )
    {
        

        if(GetSymbolByIdent(pstrIdent, iFuncIndex))
		return -1;
        

        SymbolNode* pNewSymbol = (SymbolNode*)malloc(sizeof(SymbolNode));
	strcpy_s(pNewSymbol->pstrIdent, pstrIdent);
	
        pNewSymbol->iSize = iSize;
        pNewSymbol->iStackIndex = iStackIndex;
        pNewSymbol->iFuncIndex = iFuncIndex;
		g_RefSymbols.Append(pNewSymbol);
		pNewSymbol->iIndex = g_RefSymbols.getCount();

		return g_RefSymbols.getCount();
    }

    /******************************************************************************************
    *
    *   AssmblSourceFile ()///stop right here
    *
    *   Initializes the master instruction lookup table.
    */

   void AssmblSourceFile ()
    {
        g_ScriptHeader.iStackSize = 0;
	g_ScriptHeader.iIsMainFuncPresent = false;
	
	g_iInstrStreamSize = 0;
	g_iIsSetStackSizeFound = false;
	g_iIsSetPriorityFound = false;
	g_ScriptHeader.iGlobalDataSize = 0;

	int iIsFuncActive = false;
	FuncNode* pCurrFunc = 0;
	int iCurrFuncIndex = 0;
	char pstrCurrFuncName[MAX_IDENT_SIZE];
	int iCurrFuncParamCount = 0;
	int iCurrFuncLocalDataSize = 0;

	InstrLookup CurrInstr;

	ResetLexer();

        

       while(true)
	{
		if(GetNextToken() == END_OF_TOKEN_STREAM)
			break;
	

		switch(g_Lexer.CurrToken)
	{
	case TOKEN_TYPE_SETSTACKSIZE:
		if(iIsFuncActive)
			ExitOnCodeError(ERROR_MSSG_LOCAL_SETSTACKSIZE);
		if(g_iIsSetStackSizeFound)
			ExitOnCodeError(ERROR_MSSG_MULTIPLE_SETSTACKSIZES);
		if(GetNextToken() != TOKEN_TYPE_INT)
			ExitOnCodeError(ERROR_MSSG_INVALID_STACK_SIZE);
		g_ScriptHeader.iStackSize = atoi(GetCurrLexeme());
		g_iIsSetStackSizeFound = true;
		break;

                // SetPriority

               case TOKEN_TYPE_SETPRIORITY:
		if(iIsFuncActive)
			ExitOnCodeError(ERROR_MSSG_LOCAL_SETPRIORITY);
		if(g_iIsSetPriorityFound)
			ExitOnCodeError(ERROR_MSSG_MULTIPLE_SETPRIORITIES);//ERROR_MSSG_MULTIPLE_SETPRIORITIES);
		GetNextToken();
		switch(g_Lexer.CurrToken)
		{
						// An integer lexeme means the user is defining a specific priority

						case TOKEN_TYPE_INT:
			g_ScriptHeader.iUserPriority = atoi(GetCurrLexeme());
			g_ScriptHeader.iStackSize = PRIORITY_USER;
			break;

						case TOKEN_TYPE_IDENT:

							// Determine which rank was specified

							if ( stricmp ( g_Lexer.pstrCurrLexeme, PRIORITY_LOW_KEYWORD ) == 0 )
								g_ScriptHeader.iPriorityType = PRIORITY_LOW;
							else if ( stricmp ( g_Lexer.pstrCurrLexeme, PRIORITY_MED_KEYWORD ) == 0 )
								g_ScriptHeader.iPriorityType = PRIORITY_MED;
							else if ( stricmp ( g_Lexer.pstrCurrLexeme, PRIORITY_HIGH_KEYWORD ) == 0 )
								g_ScriptHeader.iPriorityType = PRIORITY_HIGH;
							else
								ExitOnCodeError ( ERROR_MSSG_INVALID_PRIORITY );

							break;

						default:
			ExitOnCodeError(ERROR_MSSG_INVALID_PRIORITY);
		}

					g_iIsSetPriorityFound = true;
		break;

	case TOKEN_TYPE_VAR:
	{
		if(GetNextToken() != TOKEN_TYPE_IDENT)
			ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
		char pstrIdent[MAX_IDENT_SIZE];
		strcpy_s(pstrIdent,GetCurrLexeme());
		int iSize =1;
		if(GetLookAheadChar() == '[')
		{
			if(GetNextToken() != TOKEN_TYPE_OPEN_BRACKET)
				ExitOnCharExpectedError('[');
			if(GetNextToken() != TOKEN_TYPE_INT)
				ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_SIZE);
			iSize = atoi(GetCurrLexeme());
			if(iSize <= 0)
				ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_SIZE);
			if(GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET)
				ExitOnCharExpectedError(']');
		}
		int iStackIndex;
		if(iIsFuncActive)
			iStackIndex = -(iCurrFuncLocalDataSize + 2);
		else
			iStackIndex = g_ScriptHeader.iGlobalDataSize;
		if(AddSymbol(pstrIdent, iSize,iStackIndex, iCurrFuncIndex) == -1)
			ExitOnCodeError(ERROR_MSSG_IDENT_REDEFINITION);
		if(iIsFuncActive)
			iCurrFuncLocalDtaSize += iSize;
		else
			g_ScriptHeader.iGlobalDataSize += iSize;
		break;
	}

                // Func

                case TOKEN_TYPE_FUNC:
		{
			if(iIsFuncActive)
				ExitOnCodeError(ERROR_MSSG_NESTED_FUNC);
			if(GetNextToken() != TOKEN_TYPE_IDENT)
				ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);

			char * pstrFuncName = GetCurrLexeme();

			int iEntryPoint = g_iInstrStreamSize;

			int iFuncIndex = AddFunc(pstrFuncName,iEntryPoint);
			if(iFuncIndex == -1)
				ExitOnCodeError(ERROR_MSSG_FUNC_REDEFINITION);
			if(strcmp(pstrFuncName,MAIN_FUNC_NAME) == 0)
			{
				g_ScriptHeader.iIsMainFuncPresent = true;
				g_ScriptHeader.iMainFuncIndex = iFuncIndex;
			}

			iIsFuncActive = true;
			strcpy_s(pstrCurrFuncName,pstrFuncName);
			iCurrFuncIndex = iFuncIndex;
			iCurrFuncParamCount = 0;
			iCurrFuncLocalDataSize = 0;

			while(GetNextToken() == TOKEN_TYPE_NEWLINE);
				if(g_Lexer.CurrToken !=  TOKEN_TYPE_OPEN_BRACE)
					ExitOnCharExpectedError('{');
			++g_iInstrStreamSize;
			break;
		}

                case TOKEN_TYPE_CLOSE_BRACE:
		if(!iIsFuncActive)
			ExitOnCharExpectedError('}');
		SetFuncInfo(pstrCurrFuncName,iCurrFuncParamCount,iCurrFuncLocalDataSize);
		iIsFuncActive = false;
		break;
	case TOKEN_TYPE_PARAM:
	{	if(!iIsFuncActive)
			ExitOnCodeError(ERROR_MSSG_GLOBAL_PARAM);
		if(strcmp(pstrCurrFuncName,MAIN_FUNC_NAME) == 0)
			ExitOnCodeError(ERROR_MSSG_MAIN_PARAM);
		if(GetNextToken() != TOKEN_TYPE_IDENT)
			ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
		++iCurrFuncParamCount;
		break;
	}
	case TOKEN_TYPE_INSTR:
	{
		if(!iIsFuncActive)
			ExitOnCodeError(ERROR_MSSG_GLOBAL_INSTR);
		++g_iInstrStreamSize;
		break;
	}
	case TOKEN_TYPE_IDENT:
	{
		if(GetLookAheadChar() != ':')
			ExitOnCodeError(ERROR_MSSG_INVALID_INSTR);
		if(!iIsFuncActive)
			ExitOnCodeError(ERROR_MSSG_GLOBAL_LINE_LABEL);
		char * pstrIdent = GetCurrLexeme();
		int iTargetIndex = g_iInstrStreamSize -1;
		int iFuncIndex = iCurrFuncIndex;

		if(AddLabel(pstrIdent,iTargetIndex,iFuncIndex) == -1)
			ExitOnCodeError(ERROR_MSSG_LINE_LABEL_REDEFINITION);
		break;
	}
	default:
		if(g_Lexer.CurrToken != TOKEN_TYPE_NEWLINE)
			ExitOnCodeError(ERROR_MSSG_INVALID_INPUT);
		
	}


		if(!SkipToNextLine())
			break;
	}
	g_pInstrStream = (Instr*)malloc(g_iInstrStreamSize * sizeof(Instr));
	for(int iCurrInstrIndex = 0;iCurrInstrIndex < g_iInstrStreamSize;++iCurrInstrIndex)
		g_pInstrStream[iCurrInstrIndex].pOpList = NULL;

	g_iCurrInstrIndex = 0;

	ResetLexer();

	while(true)
	{
		if(GetNextToken() == END_OF_TOKEN_STREAM)
			break;

		switch(g_Lexer.CurrToken)
		{
		case TOKEN_TYPE_FUNC:
		{
			GetNextToken();

			pCurrFunc = GetFuncByName(GetCurrLexeme());
			iIsFuncActive = true;
			iCurrFuncParamCount = 0;

			iCurrFuncIndex = pCurrFunc->iIndex;
			while(GetNextToken() == TOKEN_TYPE_NEWLINE);
			break;
		}

		case TOKEN_TYPE_CLOSE_BRACE:
			{
				iIsFuncActive = false;
			if(strcmp(pCurrFunc->pstrName, MAIN_FUNC_NAME) == 0)
			{
				g_pInstrStream[g_iCurrInstrIndex].iOpCode = INSTR_EXIT;

				g_pInstrStream[g_iCurrInstrIndex].iOpCount = 1;

				g_pInstrStream[g_iCurrInstrIndex].pOpList = (Op *)malloc(sizeof(Op));

				g_pInstrStream[g_iCurrInstrIndex].pOpList[0].iType = OP_TYPE_INT;

				g_pInstrStream[g_iCurrInstrIndex].pOpList[0].iIntLiteral = 0;

			}
		else
		{
			g_pInstrStream[g_iCurrInstrIndex].iOpCode = INSTR_RET;
			g_pInstrStream[g_iCurrInstrIndex].iOpCount = 0;
			g_pInstrStream[g_iCurrInstrIndex].pOpList = NULL;
		}
		++g_iCurrInstrIndex;
		break;
			}
	case TOKEN_TYPE_PARAM:
		{
			if(GetNextToken() != TOKEN_TYPE_IDENT)
				ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
			char * pstrIdent = GetCurrLexeme();

			int iStackIndex = -(pCurrFunc->iLocalDataSize+2+(iCurrFuncParamCount+1));

			if(AddSymbol(pstrIdent,1,iStackIndex,iCurrFuncIndex) == -1)
				ExitOnCodeError(ERROR_MSSG_IDENT_REDEFINITION);

			++iCurrFuncParamCount;
			break;
		}
	case TOKEN_TYPE_INSTR:
		{
			GetInstrByMnemonic(GetCurrLexeme(), &CurrInstr);
			g_pInstrStream[g_iCurrInstrIndex].iOpCode = CurrInstr.iOpCode;
			g_pInstrStream[g_iCurrInstrIndex].iOpCount = CurrInstr.iOpCount;
			Op * pOpList = (Op*)malloc(CurrInstr.iOpCount * sizeof(Op));
			for(int iCurrOpIndex = 0; iCurrOpIndex < CurrInstr.iOpCount; ++iCurrOpIndex)
			{
				OpTypes CurrOpTypes = CurrInstr.OpList[iCurrOpIndex];
				Token InitOpToken = GetNextToken();
				switch(InitOpToken)
				{
				case TOKEN_TYPE_INT:
					if(CurrOpTypes & OP_FLAG_TYPE_INT)
					{
						pOpList[iCurrOpIndex].iType = OP_TYPE_INT;

						pOpList[iCurrOpIndex].iIntLiteral = atoi(GetCurrLexeme());
					}
					else
						ExitOnCodeError(ERROR_MSSG_INVALID_OP);
					break;
				case TOKEN_TYPE_FLOAT:
					if(CurrOpTypes & OP_FLAG_TYPE_FLOAT)
					{
						pOpList[iCurrOpIndex].iType = OP_TYPE_FLOAT;
						pOpList[iCurrOpIndex].fFloatLiteral = (float)atof(GetCurrLexeme());
					}
					else
						ExitOnCodeError(ERROR_MSSG_INVALID_OP);
					break;
				case TOKEN_TYPE_QUOTE:
					{
						if(CurrOpTypes & OP_FLAG_TYPE_STRING)
						{
							GetNextToken();
							
							switch(g_Lexer.CurrToken)
							{
							case TOKEN_TYPE_QUOTE:
								{
									pOpList[iCurrOpIndex].iType= OP_TYPE_INT;
									pOpList[iCurrOpIndex].iIntLiteral = 0;
									break;
								}
						case TOKEN_TYPE_STRING:
							{	char * pstrString = GetCurrLexeme();
							int iStringIndex = AddString(&g_RefStrings, pstrString);
							if(GetNextToken() != TOKEN_TYPE_QUOTE)
								ExitOnCharExpectedError('\\');
							pOpList[iCurrOpIndex].iType = OP_TYPE_STRING_INDEX;///set*****
							pOpList[iCurrOpIndex].iStringTableIndex = iStringIndex;
							break;
						}
				default:
					ExitOnCodeError(ERROR_MSSG_INVALID_STRING);
				}
			}
				else
						ExitOnCodeError(ERROR_MSSG_INVALID_OP);
					break;
				}
				case TOKEN_TYPE_REG_RETVAL:
					if(CurrOpTypes & OP_FLAG_TYPE_REG)
					{
						pOpList[iCurrOpIndex].iType = OP_TYPE_REG;
						pOpList[iCurrOpIndex].iReg = 0;
					}
					else
						ExitOnCodeError(ERROR_MSSG_INVALID_OP);
					break;
					

				case TOKEN_TYPE_IDENT:
					{
						if(CurrOpTypes & OP_FLAG_TYPE_MEM_REF)
						{
							char pstrIdent[MAX_IDENT_SIZE];
							strcpy_s(pstrIdent,GetCurrLexeme());

							if(!GetSymbolByIdent(pstrIdent,iCurrFuncIndex))
								ExitOnCodeError(ERROR_MSSG_UNDEFINED_IDENT);

							int iBaseIndex = GetStackIndexByIdent(pstrIdent,iCurrFuncIndex);
							if(GetLookAheadChar() != '[')
							{
								if(GetSizeByIdent(pstrIdent, iCurrFuncIndex)> 1)
									ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_NOT_INDEXED);
							pOpList[iCurrOpIndex].iType = OP_TYPE_ABS_STACK_INDEX;
							pOpList[iCurrOpIndex].iIntLiteral = iBaseIndex;
							}
							else
							{
							if(GetSizeByIdent(pstrIdent,iCurrFuncIndex) == 1)
								ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_SIZE);
							if(GetNextToken() != TOKEN_TYPE_OPEN_BRACKET)
								ExitOnCharExpectedError('[');
							Token IndexToken = GetNextToken();
							if(IndexToken == TOKEN_TYPE_INT)
							{
								int iOffsetIndex = atoi(GetCurrLexeme());
								pOpList[iCurrOpIndex].iType = OP_TYPE_ABS_STACK_INDEX;
								pOpList[iCurrOpIndex].iStackIndex =iBaseIndex + iOffsetIndex;
							}
							else if(IndexToken == TOKEN_TYPE_IDENT)
							{	char * pstrIndexIdent = GetCurrLexeme();
								if(!GetSymbolByIdent(pstrIndexIdent,iCurrFuncIndex))
									ExitOnCodeError(ERROR_MSSG_UNDEFINED_IDENT);
								if(GetSizeByIdent(pstrIndexIdent, iCurrFuncIndex) > 1)
									ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_INDEX);
								int iOffsetIndex = GetStackIndexByIdent(pstrIndexIdent,iCurrFuncIndex);
								pOpList[iCurrOpIndex].iType = OP_TYPE_REL_STACK_INDEX;
								pOpList[iCurrOpIndex].iStackIndex = iBaseIndex;
								pOpList[iCurrOpIndex].iOffsetIndex = iOffsetIndex;
							}
							else
							{
								ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_INDEX);
							}
							if(GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET)
								ExitOnCharExpectedError('[');
						}
					}

				if(CurrOpTypes & OP_FLAG_TYPE_LINE_LABEL)
				{
					char * pstrLabelIndent = GetCurrLexeme();
					LabelNode * pLabel = GetLabelByIdent(pstrLabelIndent,iCurrFuncIndex);
					if(!pLabel)
						ExitOnCodeError(ERROR_MSSG_UNDEFINED_LINE_LABEL);
					pOpList[iCurrOpIndex].iType = OP_TYPE_INSTR_INDEX;
					pOpList[iCurrOpIndex].iInstrIndex = pLabel->iTargetIndex;
				}
				if(CurrOpTypes & OP_FLAG_TYPE_FUNC_NAME)
				{
					char * pstrFuncName = GetCurrLexeme();

					FuncNode * pFunc = GetFuncByName(pstrFuncName);

					if(!pFunc)
						ExitOnCodeError(ERROR_MSSG_UNDEFINED_FUNC);

					pOpList[iCurrOpIndex].iType = OP_TYPE_FUNC_INDEX;
					pOpList[iCurrOpIndex].iFuncIndex = pFunc->iIndex;
				}
				if(CurrOpTypes & OP_FLAG_TYPE_HOST_API_CALL)
				{
					char* pstrHostAPICall = GetCurrLexeme();

					int iIndex = AddString(&g_RefHostInit,pstrHostAPICall);
					pOpList[iCurrOpIndex].iType = OP_TYPE_HOST_API_CALL_INDEX;
					pOpList[iCurrOpIndex].iHostAPICallIndex = iIndex;

				}
				break;
			}
			default:
				ExitOnCodeError(ERROR_MSSG_INVALID_OP);
				break;
			}
			if(iCurrOpIndex < CurrInstr.iOpCount-1)
				if(GetNextToken() != TOKEN_TYPE_COMMA)
					ExitOnCharExpectedError(',');
			}

			if(GetNextToken() != TOKEN_TYPE_NEWLINE)
				ExitOnCodeError(ERROR_MSSG_INVALID_INPUT);

			g_pInstrStream[g_iCurrInstrIndex].pOpList = pOpList;

			++g_iCurrInstrIndex;
			break;
			}
			}

			if(!SkipToNextLine())
				break;

}

}


    /******************************************************************************************
    *
    *   PrintAssmblStats ()
    *
    *   Prints miscellaneous assembly stats.
    */

    void PrintAssmblStats ()
    {
       
    }

/******************************************************************************************
    *
    *   BuildXSE ()
    *
    *   Dumps the assembled executable to an .XSE file.
    */

void BuildXSE()
{
	
	//char* buffer = 0;

	FILE* pExecFile;
	if(fopen_s(&pExecFile,g_ptr_ExeFilename, "wb"))
		ExitOnError("Could not open executable file for output");
	
	fwrite(XSE_ID_STRING, 4,1,pExecFile);

	char cVersionMajor = VERSION_MAJOR,
		cVersionMinor = VERSION_MINOR;
	fwrite(&cVersionMajor,1,1,pExecFile);
	fwrite(&cVersionMinor,1,1,pExecFile);

	fwrite(&g_ScriptHeader.iStackSize,4,1,pExecFile);
	fwrite(&g_ScriptHeader.iGlobalDataSize,4,1,pExecFile);

	char cIsMainPresent = 0;
	if(g_ScriptHeader.iIsMainFuncPresent)
		cIsMainPresent = 1;
	fwrite(&cIsMainPresent,1,1,pExecFile);
	fwrite(&g_ScriptHeader.iMainFuncIndex,4,1,pExecFile);

	char cPriorityType = g_ScriptHeader.iPriorityType;
	fwrite(&cPriorityType,1,1,pExecFile);
	fwrite(&g_ScriptHeader.iUserPriority,4,1,pExecFile);
	fwrite(&g_iInstrStreamSize,4,1,pExecFile);

	for(int iCurrInstrIndex = 0; iCurrInstrIndex < g_iInstrStreamSize;++iCurrInstrIndex)
	{
		short sOpcode = g_pInstrStream[iCurrInstrIndex].iOpCode;
		fwrite(&sOpcode,2,1,pExecFile);

		char iOpCount = g_pInstrStream[iCurrInstrIndex].iOpCount;
		fwrite(&iOpCount,1,1,pExecFile);
		for(int iCurrOpIndex = 0; iCurrOpIndex < iOpCount;++iCurrOpIndex)
		{
			Op CurrOp = g_pInstrStream[iCurrInstrIndex].pOpList[iCurrOpIndex];
			char cOpType = CurrOp.iType;
			fwrite(&cOpType,1,1,pExecFile);

			switch(CurrOp.iType)
			{
				case OP_TYPE_INT:
					fwrite(&CurrOp.iIntLiteral,sizeof(int), 1,pExecFile);
					break;
				case OP_TYPE_FLOAT:
					fwrite(&CurrOp.fFloatLiteral,sizeof(float),1,pExecFile);
					break;
				case OP_TYPE_STRING_INDEX:
					fwrite(&CurrOp.iStringTableIndex,sizeof(int),1,pExecFile);
					break;
				case OP_TYPE_INSTR_INDEX:
					fwrite(&CurrOp.iInstrIndex,sizeof(int),1,pExecFile);
					break;
				case OP_TYPE_ABS_STACK_INDEX:
					fwrite(&CurrOp.iStackIndex,sizeof(int),1,pExecFile);
					break;
				case OP_TYPE_REL_STACK_INDEX:
					fwrite(&CurrOp.iStackIndex,sizeof(int),1,pExecFile);
					fwrite(&CurrOp.iOffsetIndex,sizeof(int),1,pExecFile);
					break;
				case OP_TYPE_FUNC_INDEX:
					fwrite(&CurrOp.iFuncIndex,sizeof(int),1,pExecFile);
					break;
				case OP_TYPE_HOST_API_CALL_INDEX:
					fwrite(&CurrOp.iHostAPICallIndex,sizeof(int),1,pExecFile);
					break;
				case OP_TYPE_REG:
					fwrite(&CurrOp.iReg,sizeof(int),1,pExecFile);
					break;
			}
		}
	}


		char cParamCount;
		int buffer = 0;

buffer = g_RefStrings.getCount();
fwrite(&buffer,4,1,pExecFile);//start

DLItr<char*> ptr = g_RefStrings.GetIt();

for(ptr.Start();ptr.Valid();ptr.Forth())
{
	char* pstrCurrString = (char*)ptr.Item();
	int iCurrStringLength = strlen(pstrCurrString);

	fwrite(&iCurrStringLength,4,1,pExecFile);
	fwrite(pstrCurrString,strlen(pstrCurrString),1,pExecFile);
}

        


buffer = g_RefFunctions.getCount();
		fwrite (&buffer, 4, 1, pExecFile );

		

		DLItr<FuncNode*> fNode = g_RefFunctions.GetIt();

		

		for (fNode.Start();fNode.Valid();fNode.Forth())
		{
			

			FuncNode * pFunc = ( FuncNode * ) fNode.Item();

			

			fwrite ( & pFunc->iEntryPoint, sizeof ( int ), 1, pExecFile );

			

			cParamCount = pFunc->iParamCount;
			fwrite ( & cParamCount, 1, 1, pExecFile );

			

			fwrite ( & pFunc->iLocalDataSize, sizeof ( int ), 1, pExecFile );

            

            char cFuncNameLength = strlen ( pFunc->pstrName );
            fwrite ( & cFuncNameLength, 1, 1, pExecFile );

            

            fwrite ( & pFunc->pstrName, strlen ( pFunc->pstrName ), 1, pExecFile );

		
		}

        


buffer = g_RefHostInit.getCount();
		fwrite (&buffer, 4, 1, pExecFile );

		

		DLItr<char*> hNode = g_RefHostInit.GetIt();

		

		for (hNode.Start();hNode.Valid();hNode.Forth())
		{
			

			char * pstrCurrHostAPICall = ( char * ) hNode.Item();
			char cCurrHostAPICallLength = strlen ( pstrCurrHostAPICall );

			

			fwrite ( & cCurrHostAPICallLength, 1, 1, pExecFile );
			fwrite ( pstrCurrHostAPICall, strlen ( pstrCurrHostAPICall ), 1, pExecFile );

		}

        

        fclose ( pExecFile );
    }

    /******************************************************************************************
    *
    *   Exit ()
    *
    *   Exits the program.
    */

   void Exit()
{
	//Shutdown();
	exit(0);
}
