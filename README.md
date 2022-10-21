# Select Tools

The _Select Tools_ is a simple template library for selecting elements from containers.
It is mostly an theoretical experiment and does not claim in any way correctness or completeness.

## Selecting

Selecting an element from a container is similar to finding it.
However, where the usual STL _find_ functions return an iterator to `T`, defaulting to the end iterator, _select_ returns a pointer to `T`, defaulting to the `nullptr`.

Some functions use other defaults to be able to return `T` by reference or by value, instead of pointer.

## Documentation

Documentation is provided as inline comments.
Use Doxygen to extract and build a standalone documenation.

## License

Copyright David Lichti 2022.
Distributed under the Boost Software License, Version 1.0.
See accompanying file [LICENSE.txt](LICENSE.txt) or copy at [https://www.boost.org/LICENSE_1_0.txt](https://www.boost.org/LICENSE_1_0.txt)
