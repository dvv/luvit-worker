PATH := .:$(PATH)

GET := $(shell which curl && echo ' -L --progress-bar')
ifeq ($(GET),)
GET := $(shell which wget && echo ' -q --progress=bar --no-check-certificate -O -')
endif
ifeq ($(GET),)
GET := curl-or-wget-is-missing
endif

all: deps build/worker.luvit build/sleep.luvit
	-luvit -i test/smoke.lua

deps: build/uv

build/%.luvit: src/%.c
	mkdir -p build
	$(CC) -g -pipe -shared -fPIC -Isrc/ -Ibuild/uv/include -o $@ $^

build/uv:
	mkdir -p build
	$(GET) https://github.com/joyent/libuv/tarball/master | tar -xzpf - -C build
	mv build/joyent-* build/uv

clean:
	rm -fr build

test:
	-luvit -e '' || wget -qct3 http://luvit.io/dist/latest/ubuntu-latest/$(shell uname -m)/luvit-bundled/luvit
	-chmod a+x luvit 2>/dev/null
	luvit -i test/smoke.lua

.PHONY: all deps test clean
.SILENT:
