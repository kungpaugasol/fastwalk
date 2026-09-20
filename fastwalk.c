#include <math.h>
#include <lua.h>
#include <lauxlib.h>

#define DEF_FOLD(name, T, init, op)                          \
static int l_##name(lua_State *L) {                          \
    luaL_checktype(L, 1, LUA_TTABLE);                        \
    T acc = init;                                            \
    lua_pushnil(L);                                          \
    while (lua_next(L, 1)) {                                 \
        T v = (T)lua_tonumber(L, -1);                        \
        op;                                                  \
        lua_pop(L, 1);                                       \
    }                                                        \
    lua_pushnumber(L, (lua_Number)acc);                      \
    return 1;                                                \
}

DEF_FOLD(sum_values, lua_Number, 0, acc += v)
DEF_FOLD(count,      long,       0, acc += 1)
DEF_FOLD(max_value,  lua_Number, -HUGE_VAL, if (v > acc) acc = v)
DEF_FOLD(min_value,  lua_Number,  HUGE_VAL, if (v < acc) acc = v)

static const luaL_Reg R[] = {
    {"sum_values", l_sum_values},
    {"count",      l_count},
    {"max_value",  l_max_value},
    {"min_value",  l_min_value},
    {NULL, NULL}
};

int luaopen_fastwalk(lua_State *L) {
    luaL_newlib(L, R);
    return 1;
}