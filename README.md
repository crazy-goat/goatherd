# Goatherd
*Goatherd* is free, open source, languange independend, fast and lightweight load balancer with build-in process manager for any TCP/IP serivces. It's written using modern c++17 and well-known libraries like [Boost.Asio](http://www.boost.org/doc/libs/1_65_1/doc/html/boost_asio.html) for networking or [Boost.Process](http://www.boost.org/doc/libs/1_65_1/doc/html/process.html) for process monitoring.

## Features

- simple load ballancer using round-robin,
- TCP/IP or Unix sockets,
- process monitor
- Hot-code reload,

## Requirements	
To build *Goatherd* you will need:

- Modern c++ compiler (C++17 language support)
- Boost library (1.64+)
- cmake (tested on 3.5)
- wget - needed for downloading dependecies
- git - needed for downloading dependecies

## Building
Building *Goatherd* is very simple:
	
	git clone https://github.com/crazy-goat/goatherd.git
	cd goatherd
	mkdir build
	cd build
	cmake -DCMAKE_BUILD_TYPE=Release -DUSE_INTERNAL_BOOST=ON -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
	make
	make install
	
>If you have installed boost library, you can remove `-DUSE_INTERNAL_BOOST=ON` option

## Binaries
CrazyGoat provide binaries for Ubuntu/Debian and AlpineLinux. Binaries can be found in [releases](https://github.com/crazy-goat/goatherd/releases/latest).

Installing on Ubuntu/Debian

	wget https://github.com/crazy-goat/goatherd/releases/download/v0.3.1/goatherd-0.3.1.deb
	dpkg -i goatherd-0.3.1.deb

Installing on AlpineLinux

	wget https://github.com/crazy-goat/goatherd/releases/download/v0.3.1/goatherd-0.3.1-r0.apk
	apk add --allow-untrusted goatherd-0.3.1-r0.apk 

paramter `--allow-untrusted' is required as apk is self-signed

## Running
*Goatherd* requires only one parameter to run - path to config file

	goatherd my_service.json

## Configure
Look at this minimal config file.

	{
	  "listenSocket": "tcp://0.0.0.0:1234",
	  "workerCount" : 5,
	  "watchDir" : "src",
	  "worker" : {
	    "command" : "src/app-tcp.php",
	    "params": "%%socket%%",
	    "workerSocket" : "tcp://127.0.0.1:6000"
	  }
	}

It will start goatherd on TCP port 1234. It will spawn 5 workers listening on ports 6000-6004.
If dir `src` change, goatherd will restart all workers.

**listenSocket** - can be TCP `tcp://0.0.0.0:1234`or Unix socket `unix:///tmp/goatherd.sock`

**workerCount** - number of spawned workers

**watchDir** - path to direcotry, if this content changes all workers will be restarted. This can be relative path from working dir.

**worker.command** - path to worker executable. Can be relative path.

**worker.params** - worker optional params. The `%%socket%%` variable is used to pass port (TCP) or socket (Unix socket) number.

**worker.workerSocket** - can be TCP `tcp://127.0.0.1:1234`or Unix socket `unix:///tmp/goatherd.sock`

More config examples can be found in [examples dir](https://github.com/crazy-goat/goatherd/tree/master/examples).
