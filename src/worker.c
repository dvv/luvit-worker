/*
 *  Copyright 2012 The Luvit Authors. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "uv.h"

#include "lua.h"
#include "lauxlib.h"

/* ------------------------------------------------------ */

/*
 * Worker context
 */
typedef struct {
  lua_State* L;
  lua_State* X;
  uv_work_t work_req;
  int cb;
} luv_work_t;

/*
 * Generic worker function.
 * Just runs a function inside a separate state
 */
static void worker(uv_work_t* req) {
  /*luv_work_t* ref = container_of(req, luv_work_t, data);*/
  luv_work_t* ref = req->data;
  if (lua_isfunction(ref->X, 1)) {
    lua_call(ref->X, lua_gettop(ref->X) - 1, LUA_MULTRET);
  } /*else {
    ref->fn = luaL_checkint(ref->X, 1);
    int rc = ref->fn(luaL_checkint(ref->X, 2));
  }*/
}

/*
 * Called after a worker is done.
 * Moves results from separate state where worker function has run
 *   to the current state, and calls stored Lua function passed
 *   to luv_queue_work (effectively returning results of run worker)
 */
static void after_work(uv_work_t* req) {
  /*luv_work_t* ref = container_of(req, luv_work_t, data);*/
  luv_work_t* ref = req->data;
  lua_State *L = ref->L;

  int before = lua_gettop(L);

  /* load the callback */
  int argc = lua_gettop(ref->X);
  lua_rawgeti(L, LUA_REGISTRYINDEX, ref->cb);
  luaL_unref(L, LUA_REGISTRYINDEX, ref->cb);

  /* load error. N.B. trivially nil so far */
  lua_pushnil(L);

  /* move results */
  lua_xmove(ref->X, L, argc);

  /* ensure separate state is cleared */
  assert(lua_gettop(ref->X) == 0);

  /* call back */
  luv_acall(L, argc + 1, 0, "after_work");

  assert(lua_gettop(L) == before);

  /*cleanup */
  lua_close(ref->X);
  free(ref);
}

/*
 * Queue a worker.
 * Takes two functions, runs the first in separate state,
 *   returns results by calling the second function.
 */
int luv_queue_work(lua_State* L) {

  int argc = lua_gettop(L);

  /* check arguments */
  /* FIXME: allow cdata, mind FFI.C functions, e.g. */
  luaL_checktype(L, 1, LUA_TFUNCTION);
  luaL_checktype(L, argc, LUA_TFUNCTION);

  /* allocate worker object */
  luv_work_t* ref = malloc(sizeof(luv_work_t));
  ref->work_req.data = ref;
  ref->L = L;

  /* allocate new state */
  /* N.B. http://pgl.yoyo.org/luai/i/lua_State
   * The Lua library is fully reentrant: it has no global variables.
   * All information about a state is kept in this structure.
   */
  ref->X = luaL_newstate();
  if (ref->X == NULL) {
    return luaL_error(L, "queue: can not allocate new state");
  }

  /* clone environment */
  /* FIXME: doesn't seem to improve :( */
  /*
  lua_getfenv(L, argc);
  lua_setfenv(L, 1);
  */

  /* pop and store callback */
  ref->cb = luaL_ref(L, LUA_REGISTRYINDEX);

  /* move arguments */
  //printf("STACK0 L=%d, X=%d\n", lua_gettop(L), lua_gettop(ref->X));
  lua_xmove(L, ref->X, argc - 1);

  /* shedule the worker */
  if (uv_queue_work(luv_get_loop(L), &ref->work_req, worker, after_work)) {
    uv_err_t err = uv_last_error(luv_get_loop(L));
    return luaL_error(L, "queue: %s", uv_strerror(err));
    /*luv_push_async_error(L, err, "queue");
    return lua_error(L);*/
  }

  /* cleanup */
  assert(lua_gettop(L) == 0);

  return 0;
}

/* ------------------------------------------------------ */

static const luaL_reg exports[] = {
  { "queue", luv_queue_work },
  { NULL, NULL }
};

LUALIB_API int luaopen_worker(lua_State *L) {

  lua_newtable(L);
  luaL_register(L, NULL, exports);

  return 1;
}
