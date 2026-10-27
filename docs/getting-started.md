# Getting Started

This document gives an overview of how to load dynamic libraries in Dynamicus. In the following code 
snippet, we will focus on the class `dynamicus::Library` that represents a dynamic library we load.

## Basics

During construction, if the dynamic library path was specified, the class `dynamicus::Library`
automatically performs operations to load a library (invoking the method `load`). During destruction, the 
class `dynamicus::Library` also automatically frees resources (invoking the method `destroy`).

Example:

```cxx
#include <Dynamicus.hxx>
#include <gnu/lib-names.h>

dynamicus::Library lib(LIBM_SO);
// ...
```

## Loading library

The method `load` sets the library's condition to `Condition::FAILED` if it failed to load a dynamic
library. To check the current condition, use the method `getCondition`:

```cxx
dynamicus::Library lib;
lib.load("libnot-found.a");
if (lib.getCondition() == dynamicus::Library::Condition::FAILED) {
    // Do something
}
```

## Destroying library

Your code can invoke the method `destroy` to manually free the library without waiting for the destructor.

```cxx
dynamicus::Library lib;
lib.load(LIBM_SO);
// ...
lib.destroy();
```

## Getting a symbol address

The class `dynamicus::Library` provides a way to get address of a symbol from the dynamic library. Ensure
the dynamic library is not destroyed and successfully initialized to successfully get the symbol address:

```cxx
#include <Dynamicus.hxx>
#include <gnu/lib-names.h>
#include <iostream>

typedef double (*CosFn)(double);

int main() {
  dynamicus::Library lib(LIBM_SO);
  if (lib.getCondition() == dynamicus::Library::Condition::FAILED) {
    std::cout << "Loading the library " << LIBM_SO << " failed" << std::endl;
    return -1;
  }

  CosFn cos =
      reinterpret_cast<CosFn>(*lib.getSymbolAddress("cos").getValueUnsafe());
  if (!cos) {
    std::cout << "Failed to get \"cos\" symbol" << std::endl;
    return 0;
  }

  std::cout << "cos(25) = " << cos(25) << std::endl;

  return 0;
}
```
