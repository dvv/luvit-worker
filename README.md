Worker API for Luvit
=====

"Unblock" blocking API.

Usage
-----

```lua
local Worker = require('worker').Worker

local blocking_fn = some-blocking-C-function-say-sleep

Worker:new(blocking_fn, 10)
  :on('end', p)
  :on('error', p)
```

License
-------

[MIT](luvit-worker/license.txt)
