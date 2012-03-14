p = debug

local Worker = require('../')

-- C exposed to Lua
local Sleep = require('../build/sleep')

p('N.B. While we are blocking in sleep(), REPL should still work')

Worker.run(Sleep.sleep, 5, function (err, ...)
  if err then
    p('ERROR SLEEPING 5 sec', err)
  else
    p('SLEPT 5 sec', ...)
    p('EXITING NORMALLY')
    process.exit(0)
  end
end)

Worker.run(Sleep.sleep, 3, function (err, ...)
  if err then
    p('ERROR SLEEPING 3 sec', err)
  else
    p('SLEPT 3 sec', ...)
  end
end)

Worker.run(Sleep.sleep, 2, function (err, ...)
  if err then
    p('ERROR SLEEPING 2 sec', err)
  else
    p('SLEPT 2 sec', ...)
  end
end)
