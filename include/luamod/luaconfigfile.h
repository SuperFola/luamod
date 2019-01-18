#pragma once

#include <luamod/luastate.h>

namespace lm {
	class LuaConfigFile {
	private:
		lm::LuaState m_state;

	public:
		LuaConfigFile(const char* filepath) {
			m_state.RunFile(filepath);
		}
	};
}