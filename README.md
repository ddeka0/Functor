# Functor
Functor library for C++ callable objects using C++11's variadic templates

Most of the source code of this header is taken from `C++ Templates: The Complete Guide`
book. The Handle Body Idion is used in this functor implementation and is introduced in
Scott Meyers's book `Modern C++ Design`. 

The member function handling is not included in the book `C++ Templates: The Complete Guide`,
I have included the member function handling feature from Scott Meyers's book.

Functor implemented in `Modern C++ Design` book does not include variadic templates, therefore the number of arguments is restricted in the case of their design.

`C++ Templates: The Complete Guide` book has shown the usage of variadic template although 
the underlying implementation idea (Handle Body Idiom) remains the same.

#### Compilation and Testing

    1. g++-10 -g -std=c++17 main.cpp
    2. ./a.out
    3. optional: for debug use  -DDEBUG during compilation
