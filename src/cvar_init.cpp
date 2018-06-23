#include "cvar_init.h"
#include "com.h"
#include "CommandSystem.h"

/*
============
Cvar_Toggle_f

Toggles a cvar for easy single key binding
============
*/
void Cvar_Toggle_f(CvarSystem& cvars, CommandArgs& args) {
	if (args.Argc() != 2) {
		Com_Printf("usage: toggle <variable>\n");
		return;
	}
	auto v = cvars.VariableValue(args.Argv(1));
	v = !v;
	cvars.Set2(args.Argv(1), va("%i", v), false);
}
/*
============
Cvar_Set_f

Allows setting and defining of arbitrary cvars from console, even if they
weren't declared in C code.
============
*/
void Cvar_Set_f(CvarSystem& cvars, CommandArgs& args) {
	char	combined[MAX_STRING_TOKENS];
	if (args.Argc() < 3) {
		Com_Printf ("usage: set <variable> <value>\n");
		return;
	}
	combined[0] = 0;
	int l = 0;
	for (int i = 2 ; i < args.Argc(); i++) {
		auto len = strlen(args.Argv(i) + 1);
		if (l + len >= MAX_STRING_TOKENS - 2) {
			break;
		}
		strcat(combined, args.Argv(i));
		if (i != args.Argc()-1) {
			strcat(combined, " ");
		}
		l += len;
	}
	cvars.Set2(args.Argv(1), combined, false);
}
/*
============
Cvar_SetS_f

As Cvar_Set, but also flags it as userinfo
============
*/
void Cvar_SetS_f(CvarSystem& cvars, CommandArgs& args) {
	if (args.Argc() != 3) {
		Com_Printf("usage: sets <variable> <value>\n");
		return;
	}
	Cvar_Set_f(cvars, args);
	auto var = cvars.FindVar(args.Argv(1));
	if (!var) {
		return;
	}
	var->get().add_flag(CVAR_SERVERINFO);
}
/*
============
Cvar_SetU_f

As Cvar_Set, but also flags it as userinfo
============
*/
void Cvar_SetU_f(CvarSystem& cvars, CommandArgs& args) {
	if (args.Argc() != 3) {
		Com_Printf("usage: setu <variable> <value>\n");
		return;
	}
	Cvar_Set_f(cvars, args);
	auto v = cvars.FindVar(args.Argv(1));
	if (!v) {
		return;
	}
	v->get().add_flag(CVAR_USERINFO);
}
/*
============
Cvar_SetA_f

As Cvar_Set, but also flags it as archived
============
*/
void Cvar_SetA_f(CvarSystem& cvars, CommandArgs& args) {
	if (args.Argc() != 3) {
		Com_Printf("usage: seta <variable> <value>\n");
		return;
	}
	Cvar_Set_f(cvars, args);
	auto v = cvars.FindVar(args.Argv(1));
	if (!v) {
		return;
	}
	v->get().add_flag(CVAR_ARCHIVE);
}
/*
============
Cvar_Reset_f
============
*/
void Cvar_Reset_f(CvarSystem& cvars, CommandArgs& args) {
	if (args.Argc() != 2) {
		Com_Printf("usage: reset <variable>\n");
		return;
	}
	cvars.Reset(args.Argv(1));
}
/*
============
Cvar_List_f
============
*/
void Cvar_List_f(CvarSystem& cvars, CommandArgs& args) {
	const char *match;
	if (args.Argc() > 1 ) {
		match = args.Argv(1);
	} else {
		match = NULL;
	}
	cvars.CvarIter([&](auto& var) {
		if (match && !Com_Filter(match, var.name(), qfalse)) return;

		if (var.has_flag(CVAR_SERVERINFO)) {
			Com_Printf("S");
		} else {
			Com_Printf(" ");
		}
		if (var.has_flag(CVAR_USERINFO)) {
			Com_Printf("U");
		} else {
			Com_Printf(" ");
		}
		if (var.has_flag(CVAR_ROM)) {
			Com_Printf("R");
		} else {
			Com_Printf(" ");
		}
		if (var.has_flag(CVAR_INIT)) {
			Com_Printf("I");
		} else {
			Com_Printf(" ");
		}
		if (var.has_flag(CVAR_ARCHIVE)) {
			Com_Printf("A");
		} else {
			Com_Printf(" ");
		}
		if (var.has_flag(CVAR_LATCH)) {
			Com_Printf("L");
		} else {
			Com_Printf(" ");
		}
		if (var.has_flag(CVAR_CHEAT)) {
			Com_Printf("C");
		} else {
			Com_Printf(" ");
		}

		Com_Printf(" %s \"%s\"\n", var.name(), var.string());
	});

	Com_Printf("\n%i total cvars\n", cvars.size());
}

void Cvar_Init(CvarSystem& cvars, CommandSystem& cmd) {
	(void) cvars.Get("sv_cheats", "0", CVAR_ROM | CVAR_SYSTEMINFO);
	cmd.AddCommand("toggle", [&](auto& args) { 
		Cvar_Toggle_f(cvars, args); 
	});
	cmd.AddCommand("set", [&](auto& args) { 
		Cvar_Set_f(cvars, args); 
	});
	cmd.AddCommand("sets", [&](auto& args) {
		Cvar_SetS_f(cvars, args);
	});
	cmd.AddCommand("setu", [&](auto& args) {
		Cvar_SetU_f(cvars, args);
	});
	cmd.AddCommand("seta", [&](auto& args) {
		Cvar_SetA_f(cvars, args);
	});
	cmd.AddCommand("reset", [&](auto& args) {
		Cvar_Reset_f(cvars, args);
	});
	cmd.AddCommand("cvarlist", [&](auto& args) {
		Cvar_List_f(cvars, args);
	});
	cmd.AddCommand("cvar_restart", [&](auto& args) {
		cvars.Restart();
	});
}