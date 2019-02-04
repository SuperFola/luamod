#pragma once

#include <luamod/lua.h>
#include <luamod/table.h>

#include <string>
#include <vector>

namespace lm {
	class Stack;

	class LuaFunction;

	class Stack {
	private:
		lua_State * m_l;
	public:
		Stack(lua_State* state);

		void Push(int n);
		void Push(bool b);
		
		void Push(const char* c);
		void Push(const std::string& str);

		void Push(long l);

		template <typename T, typename... Args>
		void Push(T t, const Args&... args) {
			Push(t);
			Push(args...);
		}

		void Push() {}

		template <typename T>
		T Read(int index=-1);

		template <>
		int Read<int>(int index) {
			int i = lua_tointeger(m_l, index);
			return i;
		}

		template <>
		lua_Number Read<lua_Number>(int index) {
			lua_Number d = lua_tonumber(m_l, index);
			return d;
		}

		template <>
		bool Read<bool>(int index) {
			bool b = lua_toboolean(m_l, index);
			return b;
		}

		template <>
		const char* Read<const char*>(int index) {
			const char* c = lua_tostring(m_l, index);
			return c;
		}

		template <>
		std::string Read<std::string>(int index) {
			const char* c = lua_tostring(m_l, index);
			return std::string(c);
		}

		template <>
		Array Read(int index) {
			Array array(m_l);
			array.FromStack(index);
			array.Push();
			return array;
		}

		template <>
		Table Read(int index) {
			Table table(m_l);
			table.FromStack(index);
			table.Push();
			return table;
		}

		lua_State* GetState() {
			return m_l;
		}

		template <typename T>
		T Pop() {
			const char* t32 = typeid(T).name();
			const char* ttype = TypeOfTop();
			
			T top = Read<T>(-1);
			lua_pop(m_l, 1);
			return top;
		}

		const char* TypeOfTop() {
			int t = lua_type(m_l, -1);
			const char* t2 = lua_typename(m_l, t);
			return t2;
		}

		bool TopIsNil() {
			return lua_isnil(m_l, -1);
		}
	};
}
