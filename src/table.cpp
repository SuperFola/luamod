#include <luamod/table.h>

#include <assert.h>

using namespace lm;

Table::Table(lua_State* state) 
	: m_state(state) {
}

void Table::FromStack(int index) {
	m_ref = LuaRefProxy(m_state);
	m_ref.CreateFromStack(index);
}

int Array::Count() {
	m_ref.Push();
	lua_len(m_state, -1);
	
	int count = static_cast<int>(lua_tonumber(m_state, -1));

	lua_pop(m_state, 1);
	return count;
}

void Array::FromStack(int i) {
	m_ref = LuaRefProxy(m_state);
	m_ref.CreateFromStack(i);
}