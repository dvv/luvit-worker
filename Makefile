LUADIR := ../luvit/deps/luajit/src
UVDIR  := ../luvit/deps/uv

all: build/worker.luvit build/sleep.luvit
	luvit -i test/smoke.lua

build/%.luvit: src/%.c
	mkdir -p build
	$(CC) -g -pipe -shared -fPIC -I$(LUADIR) -I$(UVDIR)/include -o $@ $^

clean:
	rm -fr build

test: all
	#luvit -i test/smoke.lua

.PHONY: all test clean
.SILENT:
