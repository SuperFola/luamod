#pragma once

#include <luamod/lua.h>
#include <luamod/stack.h>
#include <luamod/returnproxy.h>
#include <luamod/function.h>

#include <memory>
#include <functional>

namespace lm {

	class LuaState {
	private:
		lua_State * m_state;
		std::unique_ptr<Stack> m_stack;

	public:
		LuaState();
		~LuaState();

		void RunFile(const char* filepath);
		
		inline Stack* GetStack() { return m_stack.get(); }
		inline lua_State* GetLuaState() { return m_state; }

		template <typename RetType, typename... Args>
		void Register(const char* funcName, std::function<RetType(Args...)> func) {
			new detail::Function<RetType, Args...>(m_state, funcName, func);
		}

		template <typename RetType, typename... Args>
		void Register(const char* funcName, RetType(*func)(Args...)) {
			new detail::Function<RetType, Args...>(m_state, funcName, std::function<RetType(Args...)>(func));
		}

		template <typename LambdaType>
		void Register(const char* funcName, LambdaType lambda) {
			Register(funcName, (typename detail::LambdaTraits<LambdaType>::Functor)(lambda));
		}

		template <typename T>
		T GetGlobal(const char* globalName) {
			lua_getglobal(m_state, globalName);
			T global = m_stack->Pop<T>();
			return global;
		}

		template <typename T>
		void SetGlobal(const char* globalName, const T& value) {
			m_stack->Push(value);
			lua_setglobal(m_state, globalName);
		}

		template <typename... RetType, typename... Args>
		typename detail::ReturnProxy<sizeof...(RetType), RetType...>::type Call(const char* funcName, const Args&... args) {
			lua_getglobal(m_state, funcName);
			m_stack->Push(args...);
			lua_pcall(m_state, sizeof...(args), sizeof...(RetType), 0);

			return detail::ReturnProxy<sizeof...(RetType), RetType...>::Pop(m_stack);
		}
	};
}