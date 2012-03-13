--[[

Copyright 2012 The Luvit Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS-IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

--]]

local queue = require('./build/worker').queue

local Worker = require('core').Emitter:extend()

--[[
Creates a new instanse of a worker.

```lua
Worker:new(a_blocking_C_fn, 10)
  :on('end', p)
  :on('error', p)
```
]]
function Worker:initialize(cfunc, ...)

  -- enqueue execution of cfunc
  local status, err = pcall(
      queue,
      cfunc,
      -- emit 'end' on completion
      function (...) self:emit('end', ...) end,
      ...
    )

  -- emit 'error' on error
  if not status then
    self:emit('error', err)
  end

end

-- module
return {
  Worker = Worker,
}
