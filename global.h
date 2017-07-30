///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright 2015 David Martin. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LinkedList.h"
//**********Header Global********************************************
#define VERSION_MAJOR	0
#define VERSION_MINOR	1
#define MAX_FILENAME_SIZE 2048
#define SOURCE_FILE_EXT		".VASM"
#define EXEC_FILE_EXT		".VSE"
#define MAX_INSTRUCTION_LOOKUP_COUNT 256
#define MAX_INSTRUCTION_TOKEN_SIZE 16
#define MAX_SOURCE_LINE_SIZE	4096
#define MAX_IDENT_SIZE      256
#define MAX_INSTR_MNEMONIC_SIZE 16
#define MAX_LEXEME_SIZE     256
#define PRIORITY_USER       0
#define TOKEN_TYPE_IDENT    4
#define PRIORITY_LOW_KEYWORD "Low"
#define PRIORITY_MED_KEYWORD "Med"
#define PRIORITY_HIGH_KEYWORD "High"
#define TOKEN_TYPE_VAR      15
#define MAIN_FUNC_NAME "_MAIN"
#define TOKEN_TYPE_OPEN_BRACE 9
#define TOKEN_TYPE_CLOSE_BRACE 10
#define TOKEN_TYPE_NEWLINE 11
#define XSE_ID_STRING "XSE0"





#define PRIORITY_LOW		1
#define PRIORITY_MED        2
#define PRIORITY_HIGH		3
#define INSTR_EXIT 32
#define OP_TYPE_INT 0
#define OP_TYPE_STRING_INDEX 2
#define OP_TYPE_REG 8





#define TOKEN_TYPE_INVALID  19
#define TOKEN_TYPE_INT      0
#define LEX_STATE_NO_STRING   0
#define END_OF_TOKEN_STREAM   20
#define TOKEN_TYPE_SETSTACKSIZE 13
#define TOKEN_TYPE_SETPRIORITY 14
#define TOKEN_TYPE_OPEN_BRACKET 6
#define TOKEN_TYPE_CLOSE_BRACKET 7
#define TOKEN_TYPE_FUNC 16
#define TOKEN_TYPE_PARAM 17
#define TOKEN_TYPE_INSTR 12
#define TOKEN_TYPE_FLOAT 1
#define TOKEN_TYPE_QUOTE 3
#define TOKEN_TYPE_STRING 2
#define TOKEN_TYPE_REG_RETVAL 18
#define TOKEN_TYPE_COMMA 8
#define TOKEN_TYPE_COLON 5
#define TOKEN_TYPE_SETPRIORITY 14

#define ERROR_MSSG_GLOBAL_INSTR "Instructions can only appear inside functions"


#define ERROR_MSSG_LOCAL_SETSTACKSIZE "SetStackSize can only appear in the global scope"
#define ERROR_MSSG_MULTIPLE_SETSTACKSIZES "Multiple instances of SetStackSize illegal"
#define ERROR_MSSG_INVALID_STACK_SIZE     "Invalid stack size"
#define ERROR_MSSG_LOCAL_SETPRIORITY	"SetPriority canonly appear in the global scope"
#define ERROR_MSSG_MULTIPLE_SETPRIORITIES "Multiple instances of SetPriority illegal"
#define ERROR_MSSG_INVALID_PRIORITY "Invalid priority"
#define ERROR_MSSG_IDENT_EXPECTED "Identifier expected"
#define ERROR_MSSG_INVALID_ARRAY_SIZE "Invalid array size"
#define ERROR_MSSG_IDENT_REDEFINITION "Identifier redefinition"
#define ERROR_MSSG_NESTED_FUNC "Nested functions illegal"
#define ERROR_MSSG_FUNC_REDEFINITION "Function redefinition"
#define ERROR_MSSG_GLOBAL_PARAM "Parameters can only appear inside functions"
#define ERROR_MSSG_MAIN_PARAM "_Main () function cannot accept parameters"
#define ERROR_MSSG_INVALID_INSTR "Invalid instruction"
#define ERROR_MSSG_GLOBAL_LINE_LABEL "Line label can only appear inside functions"
#define ERROR_MSSG_LINE_LABEL_REDEFINITION "Line label redefinition"
#define ERROR_MSSG_INVALID_INPUT "Invalid input"
#define ERROR_MSSG_INVALID_OP "Invalid operand"
#define ERROR_MSSG_INVALID_STRING "Invalid string"
#define ERROR_MSSG_UNDEFINED_IDENT "Undefined identifier"
#define ERROR_MSSG_INVALID_ARRAY_NOT_INDEXED "Arrays must be indexed"
#define ERROR_MSSG_INVALID_ARRAY_SIZE "Invalid array size"
#define ERROR_MSSG_UNIDEFINED_IDENT "Undefined identifier"
#define ERROR_MSSG_INVALID_ARRAY_INDEX "Invalid array index"
#define ERROR_MSSG_UNDEFINED_LINE_LABEL "Undefined line label"
#define ERROR_MSSG_UNDEFINED_FUNC "Undefined function"
#define ERROR_MSSG_INVALID_OP "Invalid operand"
#define LEX_STATE_END_STRING 2
#define LEX_STATE_IN_STRING 1


//instruction Opcodes
#define INSTR_MOV	0
#define INSTR_ADD	1
#define INSTR_SUB	2
#define INSTR_MUL	3
#define INSTR_DIV	4
#define INSTR_MOD	5
#define INSTR_EXP	6
#define INSTR_NEG	7
#define INSTR_INC	8
#define INSTR_DEC	9
#define	INSTR_AND	10
#define INSTR_OR	11
#define INSTR_XOR	12
#define INSTR_NOT	13
#define INSTR_SHL	14
#define INSTR_SHR	15
#define INSTR_CBW   16
#define INSTR_CDQ	17
#define INSTR_SET	18
#define INSTR_JMP	19
#define INSTR_JE	20
#define INSTR_JNE	21
#define INSTR_JG	22
#define INSTR_JL	23
#define INSTR_JGE	24
#define INSTR_JLE	25
#define INSTR_PUSH	26
#define INSTR_POP	27
#define INSTR_CALL	28
#define INSTR_RET	29
#define INSTR_INT	30
#define INSTR_WAIT	31
#define INSTR_END	32


//**********************************
//operand datatype
//**********************************
#define OP_FLAG_TYPE_MEM_INT	1
#define OP_FLAG_TYPE_FLOAT	2
#define OP_FLAG_TYPE_MEM_STRING	4
#define OP_FLAG_TYPE_MEM_REF 8
#define OP_FLAG_TYPE_LINE_LABEL	16
#define OP_FLAG_TYPE_FUNC_NAME	32
#define OP_FLAG_TYPE_HOST_API_CALL	64
#define OP_FLAG_TYPE_REG	128
#define OP_FLAG_TYPE_INT 1
#define OP_TYPE_ABS_STACK_INDEX 3
#define OP_TYPE_REL_STACK_INDEX 4 
#define OP_TYPE_INSTR_INDEX 5
#define OP_FLAG_TYPE_FUNC_NAME 32
#define OP_TYPE_FUNC_INDEX 6
#define OP_TYPE_HOST_API_CALL_INDEX 7
#define OP_TYPE_FLOAT 1
#define OP_FLAG_TYPE_STRING 4








//******************************************************************
//**********************Struct Globals******************************
typedef int Token;
typedef int OpTypes;

typedef struct _instructRef
{
	char pstrInstr[MAX_INSTRUCTION_TOKEN_SIZE];
	int iOpcode;
	int iOpcount;
	OpTypes* OpList;
}
instructRef;

typedef struct _ScriptHeader
{
	int iStackSize;
	int iGlobalDataSize;
	int iIsMainFuncPresent;
	int iMainFuncIndex;
	int iPriorityType;
	int iUserPriority;
}
ScriptHeader;

ScriptHeader g_ScriptHeader;


typedef struct _FuncNode
{
	int iIndex;
	char pstrName[MAX_IDENT_SIZE];
	int iEntryPoint;
	int iParamCount;
	int iLocalDataSize;
}
FuncNode;

typedef struct _InstrLookup
{
	char pstrMnemonic[MAX_INSTR_MNEMONIC_SIZE];
	int iOpCode;
	int iOpCount;
	OpTypes * OpList;
}
InstrLookup;

typedef struct _Lexer
{
	int iCurrSourceLine;

	unsigned int iIndex0,iIndex1;

	Token CurrToken;
	char pstrCurrLexeme[MAX_LEXEME_SIZE];
	int iCurrLexState;
}
Lexer;

Lexer g_Lexer;

typedef struct _Op
{
	int iType;
	union
	{
		int iIntLiteral;
		float fFloatLiteral;
		int iStringTableIndex;
		int iStackIndex;
		int iInstrIndex;
		int iFuncIndex;
		int iHostAPICallIndex;
		int iReg;
	};
	int iOffsetIndex;
}Op;


typedef struct _Instr
{
	int iOpCode;
	int iOpCount;
	Op* pOpList;
}
Instr;


typedef struct _SymbolNode
{
	int iIndex;
	char pstrIdent[MAX_IDENT_SIZE];
	int iSize;
	int iStackIndex;
	int iFuncIndex;
}
SymbolNode;


typedef struct _LabelNode
{
	int iIndex;
	char pstrIdent[MAX_IDENT_SIZE];
	int iTargetIndex;
	int iFuncIndex;
}
LabelNode;



//********************************************************************

int AddinstructionRef( char * pstrInstr, int iOpcode, int iOpCount);
void SetOpType(int index, int iOpIndex, OpTypes iOptype);
int IsCharWhitespace(char cChar);
void Exit();
SymbolNode* GetSymbolByIdent(char* pstrIdent, int iFuncIndex);
void ExitOnCodeError(char* pstrErrorMssg);

//*******************************************************************
//**********************Source Globals******************************
char g_ptrSourceFilename[MAX_FILENAME_SIZE], g_ptr_ExeFilename[MAX_FILENAME_SIZE];
char* s_tempFileName = 0;
FILE *g_pSourceFile = 0;
int g_iSourceCodeSize = 0;
char** g_ppstrSourceCode = 0;
int g_iInstrStreamSize;
int g_iCurrInstrIndex;
int g_iIsSetStackSizeFound;
int g_iIsSetPriorityFound;
int iCurrFuncLocalDtaSize = 0;
Instr* g_pInstrStream = NULL;

//********************************************************************
DList<FuncNode *> g_RefFunctions;
DList<LabelNode*> g_RefLabels;
DList<SymbolNode*> g_RefSymbols;
DList<char*> g_RefStrings;
DList<char*> g_RefHostInit;

instructRef g_InstructionTable[MAX_INSTRUCTION_LOOKUP_COUNT];
