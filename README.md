[![Actions Status](https://github.com/cxkoda/nami/workflows/MacOS/badge.svg)](https://github.com/cxkoda/nami/actions)
[![Actions Status](https://github.com/cxkoda/nami/workflows/Windows/badge.svg)](https://github.com/cxkoda/nami/actions)
[![Actions Status](https://github.com/cxkoda/nami/workflows/Ubuntu/badge.svg)](https://github.com/cxkoda/nami/actions)
[![Actions Status](https://github.com/cxkoda/nami/workflows/Style/badge.svg)](https://github.com/cxkoda/nami/actions)
[![Actions Status](https://github.com/cxkoda/nami/workflows/Install/badge.svg)](https://github.com/cxkoda/nami/actions)
[![codecov](https://codecov.io/gh/cxkoda/nami/branch/master/graph/badge.svg)](https://codecov.io/gh/cxkoda/nami)

# Nami

Finite volume framework for hyperbolic systems written in modern C++.

## Dev - Usage

### Build and run test suite

Use the following commands from the project's root directory to run the test suite.

```bash
cmake -Htest -Bbuild/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# or simply call the executable: 
./build/test/GreeterTests
```

To collect code coverage information, run CMake with the `-DENABLE_TEST_COVERAGE=1` option.

### Run clang-format

Use the following commands from the project's root directory to check and fix C++ and CMake source style.
This requires _clang-format_, _cmake-format_ and _pyyaml_ to be installed on the current system.

```bash
cmake -Htest -Bbuild/test

# view changes
cmake --build build/test --target format

# apply changes
cmake --build build/test --target fix-format
```

See [Format.cmake](https://github.com/TheLartians/Format.cmake) for details.

### Build the documentation

The documentation is automatically built and [published](https://thelartians.github.io/ModernCppStarter) whenever a [GitHub Release](https://help.github.com/en/github/administering-a-repository/managing-releases-in-a-repository) is created.
To manually build documentation, call the following command.

```bash
cmake -Hdocumentation -Bbuild/doc
cmake --build build/doc --target GenerateDocs
# view the docs
open build/doc/doxygen/html/index.html
```

To build the documentation locally, you will need Doxygen, jinja2 and Pygments on installed your system.

### Build and run the standalone target

Use the following command to build and run the executable target.

```bash
cmake -Hstandalone -Bbuild/standalone
cmake --build build/standalone
./build/standalone/Greeter --help
```

### Build everything at once

The project also includes an `all` directory that allows building all targets at the same time.
This is useful during development, as it exposes all subprojects to your IDE and avoids redundant builds of the library.

```bash
cmake -Hall -Bbuild
cmake --build build

# run tests
./build/test/GreeterTests
# format code
cmake --build build --target fix-format
# run standalone
./build/standalone/Greeter --help
# build docs
cmake --build build --target GenerateDocs
```

### Additional tools

The test and standalone subprojects include the [tools.cmake](cmake/tools.cmake) file which is used to import additional tools on-demand through CMake configuration arguments.
The following are currently supported.

#### Sanitizers

Sanitizers can be enabled by configuring CMake with `-DUSE_SANITIZER=<Address | Memory | MemoryWithOrigins | Undefined | Thread | Leak | 'Address;Undefined'>`.

#### Static Analyzers

Static Analyzers can be enabled by setting `-DUSE_STATIC_ANALYZER=<clang-tidy | iwyu | cppcheck>`, or a combination of those in quotation marks, separated by semicolons.
By default, analyzers will automatically find configuration files such as `.clang-format`.
Additional arguments can be passed to the analyzers by setting the `CLANG_TIDY_ARGS`, `IWYU_ARGS` or `CPPCHECK_ARGS` variables.

#### Ccache

Ccache can be enabled by configuring with `-DUSE_CCACHE=<ON | OFF>`.
