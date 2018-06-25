#include "VM.h"
#include "sys.h"

VM::VM()
	: m_dllHandle(nullptr)
	, m_entryPoint(nullptr)
	, m_systemCall(nullptr)
	, m_dllSyscallWrap(std::bind(&VM::DllSyscall, this, std::placeholders::_1))
{}

std::unique_ptr<VM> VM::Create(const char *module, std::function<int(int*)> systemCall) {
	int (QDECL *entryPoint)(int, ...);
	auto vm = std::make_unique<VM>();
	// std::function does not allow ellipses
	// we cast it back here
	auto target = (int (*)(int, ...)) vm->m_dllSyscallWrap.target<int(*)(int)>();
	auto dllHandle = Sys_LoadDll(module, &entryPoint, target);
	if (!dllHandle) {
		return {};
	}
	vm->m_dllHandle = dllHandle;
	vm->m_entryPoint = entryPoint;
	vm->m_systemCall = std::move(systemCall);
	return vm;
}

int VM::DllSyscall(int arg, ...) {
	return m_systemCall(&arg);
}

VM::~VM() {
	if (m_dllHandle) {
		Sys_UnloadDll(m_dllHandle);
	}
}

void VM::Init(CvarSystem& cvars, CommandSystem& cmd) {
	cvars.Get( "vm_cgame", "0", CVAR_SYSTEMINFO|CVAR_ARCHIVE );	// default to DLLs now instead. Our VMs are getting too HUGE.
	cvars.Get( "vm_game", "0", CVAR_SYSTEMINFO|CVAR_ARCHIVE );	// 
	cvars.Get( "vm_ui", "0", CVAR_SYSTEMINFO|CVAR_ARCHIVE );		// 
	//client wants to know if the server is using vm's for certain modules,
	//so if pure we can force the same method (be it vm or dll) -rww
	// cmd.AddCommand ("vmprofile", VM_VmProfile_f );
	// cmd.AddCommand ("vminfo", VM_VmInfo_f );
}