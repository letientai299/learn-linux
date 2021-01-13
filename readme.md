# Learn Linux

This repo contains C code and related for many purpose:

- Learn Linux development and performance monitoring/troubleshooting.
- Learn C properly
- Learn to use CMake and related tools for developing a non-trivival C/C++ project.
- Possibly use this project as a template for my other projects with C/C++

## Build

```sh
cmake . && make
```

## Test

Run the compiled test executable.

```sh
make check
```

Rebuilt and run the test executable

```
make check
```

## Note

I'm mainly using CLion during this learning, and currently, it only support a
few [C++ test frameworks](https://www.jetbrains.com/help/clion/unit-testing-tutorial.html).
I choose [doctest](https://github.com/onqtam/doctest) due to its fast
compilation. At first, I want this repo to be pure C, but it's very hard to
find and setting up a convenient workflow with combine those C frameworks and
Clion.

Main code will be in C, but test code is C++.
