Worker API for Luvit
=====

[![Build Status](https://secure.travis-ci.org/dvv/luvit-worker.png)](http://travis-ci.org/dvv/luvit-worker)

"Unblock" blocking API.

Usage
-----

```lua
local Worker = require('worker').Worker

--
-- call [lua_CFunction](http://pgl.yoyo.org/luai/i/lua_CFunction) in separate state
--

Worker:new()
  :on('end', function (...)
    -- function returned
    p('RESULTS', ...)
  end)
  :on('error', function (err)
    p('ERROR', err)
  end)
  :run(func, ... --[[ optional args to the function]]-)

--
-- execution continues nomatter whether function were blocking or not
--

print('This is printed before RESULTS or ERROR')
```

License
-------

[MIT](luvit-worker/license.txt)
