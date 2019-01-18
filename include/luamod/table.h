#pragma once

#include <luamod/lua.h>
#include <luamod/luarefproxy.h>

namespace lm {
	class Stack;

	class Table {
	public:
		Table(lua_State* state) { m_state = state; }
		
		void LoadFromStackTop() {
			m_ref = LuaRefProxy(m_state);
			m_ref.CreateFromStackTop();
		}

		template <typename T>
		T GetProperty(const char* propertyName) {
			Stack localStack(m_state);
			m_ref.PushOntoStack();
			lua_getfield(m_state, -1, propertyName);
			T prop = localStack.Pop<T>();
			lua_pop(m_state, 1);
			return prop;
		}

		template <typename T>
		T operator[](const char* propertyName) {
			return GetProperty(propertyName);
		}
		
		// #todo (need a cleaner way of managing references)
		void Free() {
			assert(m_ref.IsInUse());
			m_ref.Free();
		}

	private:
		LuaRefProxy m_ref;
		lua_State* m_state;
	};
}