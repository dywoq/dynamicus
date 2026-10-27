# How to build

Dynamicus uses Makefile to handle cross-compilation and packing ZIP archives for library 
distribution.

# Targets

Dynamicus can compile to Windows/Linux AMD64. The environment variables OS and ARCH are used to
choose a target:

- `OS`: `windows`, `linux`
- `ARCH`: `amd64`

Example:

```
OS=windows ARCH=amd64 make
OS=linux ARCH=amd64 make
```

## GNU Toolchains

Depending on the `OS`-`ARCH` pair, a specific GNU toolchain is required:

- `windows`-`amd64`: `x86_64-w64-mingw32-gnu`
- `linux`-`amd64`: `x86_64-linux-gnu`

# Build mode

You can choose the build mode by setting the environment variable `BUILD_MODE`:

```bash
BUILD_MODE=debug make # Appends the -g and -DDEBUG flags to the C++ compiler
BUILD_MODE=release make # Appends -O2 and -DRELEASE flags
```

# Make recipes

## `all`

**Description**: Compile and archive the library. The output is `$(BUILD_DIR)/libDynamicus.a`.
**Example**:
```
make all
```

## `clean`

**Description**: Deletes the build and objects directory.
**Example**:
```
make clean
```

## `pack_zip`

**Description**: Packs the library's headers, archived library, README.md file and `docs/` folder in the ZIP 
file. Theoutput is `dynamicus-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD_MODE)`. Before packing it, the recipe 
compiles and archives the library. This recipe is needed for distribution.

**Example**:
```
make pack_zip
```

