#pragma once

#include <luamod/lua.h>
#include <luamod/stack.h>
#include <luamod/argsproxy.h>

#include <functional>
#include <memory>

namespace lm {
	namespace detail {
		class BaseFunction {
		public:
			virtual int Call(lua_State* state) = 0;
		};

		int LuaFunctorDispatch(lua_State* state);

		template <typename T>
		struct LambdaTraits : public LambdaTraits<decltype(&T::operator())> {};

		template <typename T, typename RetType, typename... Args>
		struct LambdaTraits<RetType(T::*)(Args...) const> {
			typedef std::function<RetType(Args...)> Functor;
		};

		template <typename RetType, typename... Args>
		class Function : public BaseFunction {
		private:
			typedef std::function<RetType(Args...)> FuncSig;
			FuncSig m_func;

		public:
			Function(lua_State* l, const char* name, FuncSig func)
				: m_func(func) {
				lua_pushlightuserdata(l, (void*)static_cast<BaseFunction*>(this));
				lua_pushcclosure(l, &LuaFunctorDispatch, 1);
				lua_setglobal(l, name);
			}

			virtual int Call(lua_State* state) {
				Stack closureStack(state);
				
				std::tuple<Args...> args = std::make_tuple(closureStack.Pop<Args>()...);
				RetType t = ArgumentsProxy::Lift(m_func, args);

				closureStack.Push(t);
				return 1;
			}
		};

		template <typename... Args>
		class Function<void, Args...> : public BaseFunction {
		private:
			typedef std::function<void(Args...)> FuncSig;
			FuncSig m_func;

		public:
			Function(lua_State* l, const char* name, FuncSig func)
				: m_func(func) {
				lua_pushlightuserdata(l, (void*)static_cast<BaseFunction*>(this));
				lua_pushcclosure(l, &LuaFunctorDispatch, 1);
				lua_setglobal(l, name);
			}

			virtual int Call(lua_State* state) {
				Stack closureStack(state);
				std::tuple<Args...> args = std::make_tuple(closureStack.Pop<Args>()...);
				ArgumentsProxy::Lift(m_func, args);
				return 0;
			}
		};
	}
}