# Express

A symbolic maths library for computational scientists written in modern C++.

## Features

Currently expressible expressions include:

* Strings
* Arbitrary precision (fixed and floating) numerics
* Sum types
* Product types
* Lists

Express performs semantic reduction and simplification of expressions.

Expression visualisation (graphing) is planned.

> Express does not pose restrictions on what is a "valid" expression, but it does only know how to reduce and simplify certain patterns of expression.
>
> E.g. String multiplication is not well-defined in express, so `2 * "hello"` would simply reduce to `2 * "hello"`.

## Who is this for?

Express is designed for computer scientists, mathematicians and anybody with a love for computation.

Some possible use cases might be:

* Programming language implementations
* Workbook-style applications (e.g. mathematica)
* Computational analysis/science
* Messing around in your spare time

## C++ Example

Here is an example of expression reduction:

```c++
#include <iostream>

#include "expr/Expr.hpp"

int main(int, char**){
    using namespace expr::literals;
    using namespace expr::vars;
    
    auto e0 = ((16 * x) / 4) + (1_e / 2_e);
    std::cout << e0 << '\n'; // should print 4x + 1/2
}
```

