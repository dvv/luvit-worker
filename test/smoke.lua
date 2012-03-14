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

local function sleep0(...) print('CALLED!', ...) end
local function sleep4() return FFI.C.sleep(4) end
local sleep_p = tonumber(tostring(FFI.C.sleep):gsub('^.*: ', ''), 16)
--p(FFI.C.sleep, sleep_p)

local f1 = Worker:new()
  f1:on('end', function (...)
    p('FFI: SLEPT 4 sec', ...)
  end)
  f1:on('error', function (err)
    p('FFI: ERROR SLEEPING 4 sec', err)
  end)
  --f1:run(sleep0)
  --f1:run(sleep_p, 4)
