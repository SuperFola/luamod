#pragma once

#include <luamod/lua.h>
#include <luamod/luarefproxy.h>
#include <vector>

namespace lm {
	class Stack;

	class Table {
	public:
		Table(lua_State* state);
		Table() {}

		void FromStack(int index);
		void Push() { m_ref.Push(); }

		template <typename T>
		T GetProperty(const char* propertyName, T defaultValue = T()) {
			m_ref.Push();
			lua_getfield(m_state, -1, propertyName);
			
			Stack localStack(m_state);

			if (localStack.TopIsNil())
			{
				lua_pop(m_state, 2);
				return defaultValue;
			}

			T prop = localStack.Pop<T>();
			
			lua_pop(m_state, 1);
			return prop;
		}

	private:
		LuaRefProxy m_ref;
		lua_State* m_state;
	};

	class Array {
	private:
		lua_State* m_state;
		LuaRefProxy m_ref;
		
	public:
		Array(lua_State* state): m_state(state) {}
		Array() {}

		void FromStack(int i);

		void Push() { m_ref.Push(); }

		template <typename T>
		std::vector<T> ToVector() {
			int count = Count();
			std::vector<T> outVector;
			for (int i = 1; i <= count; i++) {
				outVector.push_back(At<T>(i));
			}
			return outVector;
		}

		template <typename T>
		T At(int index) {
			assert(index > 0);
			assert(index <= Count());
			
			m_ref.Push();
			lua_pushnumber(m_state, index);
			lua_gettable(m_state, -2);

			Stack localStack(m_state);
			
			if (localStack.TopIsNil()) {
				lua_pop(m_state, 2);
				return T();
			}

			T out = localStack.Pop<T>();
			lua_pop(m_state, 1);
			return out;
		}

		int Count();
	};
}
