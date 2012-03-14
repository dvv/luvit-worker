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
  lua_CFunction fn;
  int cb;
} luv_work_t;

/*
 * Generic worker function.
 * Just runs a C function inside a separate state
 */
static void worker(uv_work_t* req) {
  /*luv_work_t* ref = container_of(req, luv_work_t, data);*/
  luv_work_t* ref = req->data;
  ref->fn(ref->X);
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

  /* load the callback and arguments */
  int argc = lua_gettop(ref->X);
  lua_rawgeti(L, LUA_REGISTRYINDEX, ref->cb);
  luaL_unref(L, LUA_REGISTRYINDEX, ref->cb);
  lua_xmove(ref->X, L, argc);
  /* ensure separate state is cleared */
  assert(lua_gettop(ref->X) == 0);

  /* call back */
  luv_acall(L, argc, 0, "after_work");

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

  int nargs = lua_gettop(L);

  /* check arguments */
  /* FIXME: allow cdata, mind FFI.C functions, e.g. */
  luaL_checktype(L, 1, LUA_TFUNCTION);
  luaL_checktype(L, 2, LUA_TFUNCTION);

  /* allocate worker object */
  luv_work_t* ref = malloc(sizeof(luv_work_t));
  ref->work_req.data = ref;
  ref->L = L;

  /* store worker function */
  /* FIXME: allow cdata, mind FFI.C functions, e.g. */
  ref->fn = lua_tocfunction(L, 1);
  if (ref->fn == NULL) {
    return luaL_error(L, "queue: can not cast worker to C function");
  }

  /* allocate new state */
  ref->X = luaL_newstate();
  if (ref->X == NULL) {
    return luaL_error(L, "queue: can not allocate new state");
  }

  /* store callback */
  lua_pushvalue(L, 2);
  ref->cb = luaL_ref(L, LUA_REGISTRYINDEX);

  /* transfer initial arguments */
  lua_xmove(L, ref->X, nargs - 2);

  /* shedule the worker */
  if (uv_queue_work(luv_get_loop(L), &ref->work_req, worker, after_work)) {
    uv_err_t err = uv_last_error(luv_get_loop(L));
    return luaL_error(L, "queue: %s", uv_strerror(err));
    /*luv_push_async_error(L, err, "queue");
    return lua_error(L);*/
  }

  /* cleanup */
  lua_pop(L, 2);
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
