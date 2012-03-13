p = debug

local Worker = require('../').Worker

-- C exposed to Lua
local Sleep = require('../build/sleep')

p('N.B. While we are blocking in sleep(), REPL should still work')
Worker:new(Sleep.sleep, 5)
  :on('end', function (...)
    p('SLEPT 5 sec', ...)
    p('EXITING NORMALLY')
    process.exit(0)
  end)
  :on('error', function (err)
    p('ERROR SLEEPING 5 sec', err)
  end)

Worker:new(Sleep.sleep, 2)
  :on('end', function (...)
    p('SLEPT 2 sec', ...)
  end)
  :on('error', function (err)
    p('ERROR SLEEPING 2 sec', err)
  end)

Worker:new(Sleep.sleep, 3)
  :on('end', function (...)
    p('SLEPT 3 sec', ...)
  end)
  :on('error', function (err)
    p('ERROR SLEEPING 3 sec', err)
  end)

-- FFI style
local FFI = require('ffi')
FFI.cdef[[
int sleep(uint32_t sec);
]]

-- TODO: fixup
--[[
Worker:new(FFI.C.sleep, 4)
  :on('end', function (...)
    p('FFI: SLEPT 4 sec', ...)
  end)
  :on('error', function (err)
    p('FFI: ERROR SLEEPING 4 sec', err)
  end)
]]--
