#pragma once

#include <luamod/lua.h>
#include <luamod/luarefproxy.h>

namespace lm {
	class Stack;

	class Table {
	public:
		Table(lua_State* state);
		
		void FromStackTop();
		
		template <typename T>
		T GetProperty(const char* propertyName) {
			m_ref.Push();
			lua_getfield(m_state, -1, propertyName);
			
			Stack localStack(m_state);
			T prop = localStack.Pop<T>();
			
			lua_pop(m_state, 1);
			return prop;
		}

	private:
		LuaRefProxy m_ref;
		lua_State* m_state;
	};
}
