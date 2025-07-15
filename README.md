# C++20 Format String Utility

Header-only library for formatting to C++ by letting you embed numbered placeholders such as {0} and {1} directly in a template string and replace them at runtime with any types streamable into std::ostream. It enforces strict syntax validation, throwing a FormatError immediately if braces are misused, non-digit characters appear inside placeholders, or an index goes out of range, while leaving any text outside the placeholders untouched.

## Example

```cpp
#include <iostream>
#include "format.hpp"

int main() {
    try {
        auto s = format("[{1}] {0} + {0} = {2}", "sum", 42, 84);
        std::cout << s << "\n"; // prints: [42] sum + sum = 84
    }
    catch (const FormatError& e) {
        std::cerr << "Formatting failed: " << e.what() << '\n';
    }
}
```

## Error Handling

Any misuse of braces, non‑digit characters inside `{}` or references to non‑existent argument indices will immediately throw `FormatError`.

## Build and Test

A simple `Makefile` compiles everything under C++20 and has Google Test for unit tests.

```bash
make        # compiles the library and tests
make test   # runs the full test suite
make clean  
```