#include <luamod/luastate.h>

using namespace lm;

LuaState::LuaState() {
	m_state = lua_open();
	luaL_openlibs(m_state);

	m_stack = std::unique_ptr<Stack>(new Stack(m_state));
}

LuaState::~LuaState() {
	lua_close(m_state);
}

void LuaState::RunFile(const char* filepath) {
	luaL_loadfile(m_state, filepath);
	lua_pcall(m_state, 0, LUA_MULTRET, 0);
}