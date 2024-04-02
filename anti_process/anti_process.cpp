
#include <Windows.h>
#include <iostream>
#include "detours.h"
#include <Winternl.h>


struct defines {

    typedef BOOL(WINAPI* LPFN_CREATEPROCESSW)(
        LPCWSTR lpApplicationName,
        LPWSTR lpCommandLine,
        LPSECURITY_ATTRIBUTES lpProcessAttributes,
        LPSECURITY_ATTRIBUTES lpThreadAttributes,
        BOOL bInheritHandles,
        DWORD dwCreationFlags,
        LPVOID lpEnvironment,
        LPCWSTR lpCurrentDirectory,
        LPSTARTUPINFOW lpStartupInfo,
        LPPROCESS_INFORMATION lpProcessInformation
        );

	typedef NTSTATUS(NTAPI* LPFN_NTQUERYSYSTEMINFORMATION)(
		SYSTEM_INFORMATION_CLASS SystemInformationClass,
		PVOID SystemInformation,
		ULONG SystemInformationLength,
		PULONG ReturnLength
		);

}; extern defines* define;

struct pointers {

    HMODULE ntdll_module = GetModuleHandleA("ntdll.dll");
    defines::LPFN_CREATEPROCESSW CreateProcessW_pointer = CreateProcessW;
    defines::LPFN_NTQUERYSYSTEMINFORMATION NtQuerySystemInformation_pointer = (defines::LPFN_NTQUERYSYSTEMINFORMATION)GetProcAddress(ntdll_module, "NtQuerySystemInformation");;

}; pointers* ptr = new pointers();

struct detoured {

   static BOOL WINAPI CreateProcessW_Hook(
        LPCWSTR lpApplicationName,
        LPWSTR lpCommandLine,
        LPSECURITY_ATTRIBUTES lpProcessAttributes,
        LPSECURITY_ATTRIBUTES lpThreadAttributes,
        BOOL bInheritHandles,
        DWORD dwCreationFlags,
        LPVOID lpEnvironment,
        LPCWSTR lpCurrentDirectory,
        LPSTARTUPINFOW lpStartupInfo,
        LPPROCESS_INFORMATION lpProcessInformation
    )
    {
        return FALSE;
    }

  static NTSTATUS NTAPI NtQuerySystemInformation_Hook(
       SYSTEM_INFORMATION_CLASS SystemInformationClass,
       PVOID SystemInformation,
       ULONG SystemInformationLength,
       PULONG ReturnLength
   ) 
   {

      return FALSE;
   }
}; extern detoured* detours;

class anti_process
{

public:

    void anti_file() {
        HMODULE module = GetModuleHandleA("kernel32.dll");
        if (module) {

			ptr->CreateProcessW_pointer = (defines::LPFN_CREATEPROCESSW)GetProcAddress(module, "CreateProcessW");
            HHOOK hook = SetWindowsHookEx(WH_CBT, (HOOKPROC)detours->CreateProcessW_Hook, module, 0);
            printf("success");
            getchar();
        }
    }

    void anti_NTQUERY() {

        HMODULE module = GetModuleHandleA("ntdll.dll");
        if (module)
        {
            HHOOK hook = SetWindowsHookEx(WH_CBT, (HOOKPROC)detours->NtQuerySystemInformation_Hook, module, 0);
            printf("success");
            getchar();
        }
    }

	void hook_process() {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)ptr->CreateProcessW_pointer, detours->CreateProcessW_Hook);
		DetourTransactionCommit();
	} 
}; static anti_process* process = new anti_process();


int main()
{
    process->anti_NTQUERY(); // works best.
}
