Worker API for Luvit
=====

"Unblock" blocking API.

Usage
-----

```lua
local Worker = require('worker').Worker

--
-- call blocking function in separate state
--

Worker:new(a_blocking_C_fn, ... --[[ optional args to the function]]-)
  :on('end', function (...)
    -- function returned
    p('RESULTS', ...)
  end)
  :on('error', function (err)
    p('ERROR', err)
  end)

--
-- execution continues as if function were non-blocking
--

print('This is printed before RESULTS or ERROR')
```

License
-------

[MIT](luvit-worker/license.txt)
