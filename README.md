# Fibonacci

Implement a simple fibonacci client / server.

## Building

You need to have Boost 1.55.0 as well as a C++11 compiler installed on your system.

```
% cmake .
% gmake
```
## Usage
The server is invoked by executing the `fibonacci` binary with a single
argument, the `port` number to listen on. Example:

```
% ./fibonacci 1234
```

The client is invoked by executing the `fibonacci` binary with two arguments,
the `host` and `port` to connect to. Input is read from stdin, example:

```
% echo 10 | ./fibonacci 127.0.0.1 1234
```
