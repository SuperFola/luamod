#include <Windows.h>

#include <luamod/luastate.h>

namespace test_lib {
	void ShowMessageBox(const char* title, const char* text, int buttons) {
		::MessageBox(0, text, title, buttons);
	}
}

int main() {
	lm::LuaState lua;
	
	lua.SetGlobal("MessageBoxButtons_OK", MB_OK);
	lua.SetGlobal("MessageBoxButtons_OKCancel", MB_OKCANCEL);

	lua.Register("ShowMessageBox", &test_lib::ShowMessageBox);
	lua.Register("GetRandomNumber", []() {
		// https://xkcd.com/221/
		return 4; // chosen by fair dice roll.
		          // guaranteed to be random.
	});

	lua.RunFile("scripts/index.lua");
	
	system("pause");
	return 0;
}