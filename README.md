# Functor
Functor library for C++ callable objects using C++11's variadic templates

Most of the source code of this header is taken from `C++ Templates: The Complete Guide`
book. The Handle Body Idion is used in this functor implementation and is introduced in
Scott Meyers's book `Modern C++ Design`. 

The member function handling is not included in the book `C++ Templates: The Complete Guide`,
I have included the member function handling feature from Scott Meyers's book.

Functor implemented in `Modern C++ Design` book does not include variadic templates, therefore the number of arguments to the functor is restricted in the case of their design.

`C++ Templates: The Complete Guide` book has shown the usage of variadic template although 
the underlying implementation idea (Handle Body Idiom) remains the same.

#### Here is an overall idea of what functors are capable of storing and handling:

![functors](https://github.com/ddeka0/Functor/blob/master/functor.gif)

#### Compilation and Testing

    1. g++-10 -g -std=c++17 main.cpp
    2. ./a.out
    3. optional: for debug use  -DDEBUG during compilation
    
#### Examples:
    // non member function
    void func1(int x) {
        x++;
        std::cout << x << std::endl;
    }

    class A {
    public:
        // member function
        int func(int x) {
            x++;
            return x + 1;
        }
        int func1(int x,int y) {
            x++;y++;
            return x + 1 + y;
        }
    };
    
    // usage:
    FunctionPtr<void(int)> f2 = func1;
    f2(1);
    
    // usage:
    using mFunc = FunctionPtr<int(int,int)>;
    mFunc f(&a,&A::func1);
    mFunc g;
    g = f;
    std::cout << g(1,1) << std::endl;
    std::cout << f(1,2) << std::endl;
    
    // usage:
    FunctionPtr<void(int)> f1 = [](int x){
        std::cout << ++x << std::endl;
    };
    
