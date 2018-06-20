#include "cvar_init.h"
#include "com.h"

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

void Cvar_Init(CvarSystem& cvars, CommandSystem &cmd) {
	(void) cvars.Get("sv_cheats", "0", CVAR_ROM | CVAR_SYSTEMINFO);
	cmd.AddCommand("toggle", [&](auto args) { Cvar_Toggle_f(cvars, args); });
	#if 0
	cmd.AddCommand("set", Cvar_Set_f);
	cmd.AddCommand("sets", Cvar_SetS_f);
	cmd.AddCommand("setu", Cvar_SetU_f);
	cmd.AddCommand("seta", Cvar_SetA_f);
	cmd.AddCommand("reset", Cvar_Reset_f);
	cmd.AddCommand("cvarlist", Cvar_List_f);
	cmd.AddCommand("cvar_restart", Cvar_Restart_f);
	#endif
}