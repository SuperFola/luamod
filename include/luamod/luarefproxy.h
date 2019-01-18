#pragma once

#include <luamod/lua.h>
#include <assert.h>

namespace lm {
	class LuaRefProxy {
	public:
		typedef int Id;

		LuaRefProxy(lua_State* state) : m_state(state), m_inUse(false) {
		}

		LuaRefProxy() {}

		inline bool IsInUse() const { return m_inUse; }

		void CreateFromStackTop() {
			assert(!m_inUse);
			m_refindex = lua_ref(m_state, true);
			m_inUse = true;
		}

		void PushOntoStack() {
			int b1 = lua_istable(m_state, -1);
			lua_getref(m_state, m_refindex);
			int b2 = lua_istable(m_state, -1);
			const char* c = lua_typename(m_state, lua_type(m_state, -1));

		}

		void Free() {
			assert(m_inUse);
			
			lua_unref(m_state, m_refindex);
		}

		Id GetRef() {
			return m_refindex;
		}

	private:
		Id m_refindex;
		lua_State* m_state;
		bool m_inUse;
	};
}