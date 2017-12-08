#!/usr/bin/env php
<?php

use Psr\Http\Message\ServerRequestInterface;
use React\EventLoop\Factory;
use React\Http\Response;
use React\Http\Server;

require __DIR__ . '/../vendor/autoload.php';

$loop = Factory::create();

$server = new Server(function (ServerRequestInterface $request) use ($argv) {
    return new Response(
        200,
        array(
            'Content-Type' => 'text/plain'
        ),
        "Hello world from {$argv[1]}\n"
    );
});

$socket = new \React\Socket\UnixServer(isset($argv[1]) ? $argv[1] : '/tmp/shepard-uds.sock', $loop);
$server->listen($socket);

echo 'Listening on ' . $socket->getAddress() . PHP_EOL;

$loop->run();
