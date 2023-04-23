# Jaguar Server

The Jaguar Server project aims at providing an implementation of a
process manager, forking `N` long-living workers. Each of the worker
implement the FastCGI protocol and make use of a NIO event loop to
manage connections and requests.

The is inspired by GUnicorn and PHP-FPM. The goal is to provide
a full transparency on the process resources, in such a way that it would
become possible to gain full control over database connections and persistent
objects.

- https://www.mit.edu/~yandros/doc/specs/fcgi-spec.html#S1

