p = debug

local Worker = require('../').Worker

-- C exposed to Lua
local Sleep = require('../build/sleep')

p('N.B. While we are blocking in sleep(), REPL should still work')
local w1 = Worker:new()
  w1:on('end', function (...)
    p('SLEPT 5 sec', ...)
    p('EXITING NORMALLY')
    process.exit(0)
  end)
  w1:on('error', function (err)
    p('ERROR SLEEPING 5 sec', err)
  end)
  w1:run(Sleep.sleep, 5)

local w2 = Worker:new()
  w2:on('end', function (...)
    p('SLEPT 2 sec', ...)
  end)
  w2:on('error', function (err)
    p('ERROR SLEEPING 2 sec', err)
  end)
  w2:run(Sleep.sleep, 2)

local w3 = Worker:new()
  w3:on('end', function (...)
    p('SLEPT 3 sec', ...)
  end)
  w3:on('error', function (err)
    p('ERROR SLEEPING 3 sec', err)
  end)
  w3:run(Sleep.sleep, 3)

-- FFI style
local FFI = require('ffi')
FFI.cdef[[
int sleep(uint32_t sec);
]]

-- TODO: fixup
--[[
Worker:new()
  :on('end', function (...)
    p('FFI: SLEPT 4 sec', ...)
  end)
  :on('error', function (err)
    p('FFI: ERROR SLEEPING 4 sec', err)
  end)
  :run(FFI.C.sleep, 4)
]]--
