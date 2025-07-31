#include <Windows.h>
#include <iostream>
using namespace std;

HHOOK keyboardHook = NULL;

void ForceF4() {
	HWND hwnd = GetForegroundWindow();

	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);

	HANDLE proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, pid);

	DWORD pathSize = MAX_PATH;
	WCHAR path[MAX_PATH];
	QueryFullProcessImageName(proc, 0, path, &pathSize);

	wcout << L"Terminating process \"" << path << L"\"..." << endl;
	TerminateProcess(proc, 1);
	wcout << L"Terminated process \"" << path << L"\"." << endl;

	CloseHandle(proc);
}

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
		if ((pKeyboard->vkCode == VK_F4) && (GetAsyncKeyState(VK_MENU) & 0x8000))
		{
			if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
			{
				cout << "Terminating process of currently in focus window..." << endl;
				ForceF4();
				return 1;
			}
		}
	}

	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main()
{
	cout << "Registering Alt + F4 low level keyboard hook..." << endl;
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, NULL);
	cout << "Registered Alt + F4 low level keyboard hook." << endl;

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	cout << "Unregistering Alt + F4 low level keyboard hook..." << endl;
	UnhookWindowsHookEx(keyboardHook);
	cout << "Unregistered Alt + F4 low level keyboard hook." << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
