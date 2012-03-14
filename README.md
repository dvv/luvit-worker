Worker API for Luvit
=====

[![Build Status](https://secure.travis-ci.org/dvv/luvit-worker.png)](http://travis-ci.org/dvv/luvit-worker)

"Unblock" blocking API.

Usage
-----

```lua
local Worker = require('worker').Worker

--
-- call blocking function in separate state
--

Worker:new()
  :on('end', function (...)
    -- function returned
    p('RESULTS', ...)
  end)
  :on('error', function (err)
    p('ERROR', err)
  end)
  :run(a_blocking_C_fn, ... --[[ optional args to the function]]-)

--
-- execution continues as if function were non-blocking
--

print('This is printed before RESULTS or ERROR')
```

License
-------

[MIT](luvit-worker/license.txt)
