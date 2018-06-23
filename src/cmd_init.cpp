#include "CommandSystem.h"
#include "com.h"

void Cmd_List_f(CommandSystem& cmd, CommandArgs& args) {
	const char *match;
	if (args.Argc() > 1) {
		match = args.Argv(1);
	} else {
		match = NULL;
	}
	int i = 0;
	cmd.Iter([&](auto name) {
		if (match && !Com_Filter(match, name, qfalse)) return;
		Com_Printf("%s\n", name);
		i++;
	});
	Com_Printf("%i commands\n", i);
}

void Cmd_Init(CommandSystem& cmd) {
	cmd.AddCommand("cmdlist", [&](auto& args) {
		Cmd_List_f(cmd, args);
	});
	#if 0
	cmd.AddCommand("exec",Cmd_Exec_f);
	cmd.AddCommand("vstr",Cmd_Vstr_f);
	cmd.AddCommand("echo",Cmd_Echo_f);
	cmd.AddCommand("wait", Cmd_Wait_f);
	#endif
}