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

## Licensing

I do not allow reproduction nor usage without explicit written agreement
from me directly.

## Develop

In order to develop, we recommend spawning the Docker image in
interactive mode:

```
make devenv
```

You can then benefit from the local repository inside the Docker image,
in an environment that contains C++ compiler. For example, to run a
light-build of the file you are currently working on:

```
make lbuild
```

We do not believe warnings should be ignored, so we apply a strict
warning=error policy. If you think it is a mistake, rethink. If you
still think it is a mistake, well... rethink again.

## Tooling

We believe documentation and tests are of primary importance.
The documentation is currently handled as a Sphinx process. This
requires a Python environment.

From tooling, build the Docker image:
```
docker build -t jaguar-tooling .
```

You activate the tooling environment by running
```
make toolenv
```

### Generate documentation

Generate the documentation from the source (once the tooling environment
activated, see above):

```
make doc
```

## Develop 

In order to develop, first create a Docker image for development purpose.
From within the `/develop` folder, run:

```
docker build -t jaguar-develop .
```

You activate the development environment by running:

```
make devenv
```