# interstate75-projects

Very WIP repo based off [i75-boilerplate](https://github.com/pimoroni/i75-boilerplate).

- `src/main.cpp` / `rain.cpp`: A Matrix-type digital rain display
- `conway.cpp`: A 'Game of Life' display

Replace the content of `src/main.cpp` to build the UF2 file.
## Setting Up

We've included a `cpp.sh` bash script that will help get you started on
Linux-like systems. This works on macOS and WSL too.

First, clone and enter this repository:

```
git clone https://github.com/pimoroni/i75-boilerplate
cd i75-boilerplate
```

Then create a build directory and grab the dependencies:

```
mkdir build
cd build
source ../ci/cpp.sh
ci_prepare_all
```

This will grab the versions of Pico SDK and Pimoroni Pico that work with this
boilerplate into the `build` directory you just created.

To configure and build just:

```
ci_cmake_configure
ci_cmake_build
```