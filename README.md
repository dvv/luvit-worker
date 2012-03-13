Worker API for Luvit
=====

[![Build Status](https://secure.travis-ci.org/dvv/luvit-worker.png)](http://travis-ci.org/dvv/luvit-worker)

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
