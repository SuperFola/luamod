#include <iostream>

#include <luamod/luastate.h>

int main() {
	lm::LuaState lua;
	
	lua.RunFile("scripts/index.lua");
	
	system("pause");
	return 0;
}