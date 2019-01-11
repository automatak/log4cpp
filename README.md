# log4cpp
Lightweight C++ logging framework with extensible log levels to use with [ser4cpp](https://github.com/automatak/ser4cpp).

## Usage
This project is a header-only library. You can safely copy the `src` folder and add it in the include directories
to be up and running. You will also need [ser4cpp](https://github.com/automatak/ser4cpp)

The project also provides a CMake configuration. You will need a recent version of CMake (3.8 and higher).

* Copy/clone this repository in your project (or use [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules))
* `add_subdirectory` the repository
* For every target that needs this library, `target_link_libraries(foo PUBLIC|PRIVATE|INTERFACE log4cpp)`

Note that a target named `ser4cpp` must be available.

## License
This project is licensed under the terms of the BSD v3 license. See `LICENSE` for more details.
