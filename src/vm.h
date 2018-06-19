#pragma once
/*
==============================================================

VIRTUAL MACHINE

==============================================================
*/
typedef struct vmSymbol_s {
	struct vmSymbol_s	*next;
	int		symValue;
	int		profileCount;
	char	symName[1];		// variable sized
} vmSymbol_t;

typedef struct vm_s {
    // DO NOT MOVE OR CHANGE THESE WITHOUT CHANGING THE VM_OFFSET_* DEFINES
    // USED BY THE ASM CODE
    int			programStack;		// the vm may be recursively entered
    int			(*systemCall)( int *parms );

	//------------------------------------
   
    char		name[MAX_QPATH];

	// for dynamic linked modules
	void		*dllHandle;
	int			(QDECL *entryPoint)( int callNum, ... );

	// for interpreted modules
	qboolean	currentlyInterpreting;

	qboolean	compiled;
	byte		*codeBase;
	int			codeLength;

	int			*instructionPointers;
	int			instructionPointersLength;

	byte		*dataBase;
	int			dataMask;

	int			stackBottom;		// if programStack < stackBottom, error

	int			numSymbols;
	struct vmSymbol_s	*symbols;

	int			callLevel;			// for debug indenting
	int			breakFunction;		// increment breakCount on function entry to this
	int			breakCount;
} vm_t;

typedef enum {
	VMI_NATIVE,
	VMI_BYTECODE,
	VMI_COMPILED
} vmInterpret_t;

typedef enum {
	TRAP_MEMSET = 100,
	TRAP_MEMCPY,
	TRAP_STRNCPY,
	TRAP_SIN,
	TRAP_COS,
	TRAP_ATAN2,
	TRAP_SQRT,
	TRAP_MATRIXMULTIPLY,
	TRAP_ANGLEVECTORS,
	TRAP_PERPENDICULARVECTOR,
	TRAP_FLOOR,
	TRAP_CEIL,

	TRAP_TESTPRINTINT,
	TRAP_TESTPRINTFLOAT,

	TRAP_ACOS,
	TRAP_ASIN
} sharedTraps_t;

void	VM_Init( void );
vm_t	*VM_Create( const char *module, int (*systemCalls)(int *), 
				   vmInterpret_t interpret );
// module should be bare: "cgame", not "cgame.dll" or "vm/cgame.qvm"

void	VM_Free( vm_t *vm );
void	VM_Clear(void);
vm_t	*VM_Restart( vm_t *vm );

int		QDECL VM_Call( vm_t *vm, int callNum, ... );

void	VM_Debug( int level );

void	VM_Shifted_Alloc(void **ptr, int size);
void	VM_Shifted_Free(void **ptr);

void	*VM_ArgPtr( int intValue );
void	*VM_ExplicitArgPtr( vm_t *vm, int intValue );