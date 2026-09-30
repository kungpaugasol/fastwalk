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

/* sum_field(t, key) -> sum of t[i][key] over all i */
static int l_sum_field(lua_State *L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    const char *key = luaL_checkstring(L, 2);

    lua_Number s = 0;
    lua_pushnil(L);
    while (lua_next(L, 1)) {
        if (lua_istable(L, -1)) {
            lua_getfield(L, -1, key);
            s += lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }
    lua_pushnumber(L, s);
    return 1;
}

/* any(t, pred) -> boolean, short-circuits on first truthy */
static int l_any(lua_State *L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    lua_pushnil(L);
    while (lua_next(L, 1)) {
        lua_pushvalue(L, 2);
        lua_pushvalue(L, -3);
        lua_pushvalue(L, -3);
        lua_call(L, 2, 1);
        int truthy = lua_toboolean(L, -1);
        lua_pop(L, 2);
        if (truthy) { lua_pushboolean(L, 1); return 1; }
    }
    lua_pushboolean(L, 0);
    return 1;
}

/* all(t, pred) -> boolean, short-circuits on first falsy */
static int l_all(lua_State *L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    lua_pushnil(L);
    while (lua_next(L, 1)) {
        lua_pushvalue(L, 2);
        lua_pushvalue(L, -3);
        lua_pushvalue(L, -3);
        lua_call(L, 2, 1);
        int truthy = lua_toboolean(L, -1);
        lua_pop(L, 2);
        if (!truthy) { lua_pushboolean(L, 0); return 1; }
    }
    lua_pushboolean(L, 1);
    return 1;
}

static const luaL_Reg R[] = {
    {"sum_values", l_sum_values},
    {"count",      l_count},
    {"max_value",  l_max_value},
    {"min_value",  l_min_value},
    {"sum_field",  l_sum_field},
    {"any",        l_any},
    {"all",        l_all},
    {NULL, NULL}
};

int luaopen_fastwalk(lua_State *L) {
    luaL_newlib(L, R);
    return 1;
}
