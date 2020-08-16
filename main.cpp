#include "functor.hpp"
#define Break std::cout << std::endl << std::endl;

void func1(int x) {
    x++;
    std::cout << x << std::endl;
}

void func2(int x) {
    x--;
    std::cout << x << std::endl;
}

class A {
public:
    int func(int x) {
        x++;
        return x + 1;
    }
    int func1(int x,int y) {
        x++;y++;
        return x + 1 + y;
    }
};



int main() {



    FunctionPtr<void(int)> f1 = [](int x){
        std::cout << ++x << std::endl;
    };
    
    Break;

    FunctionPtr<void(int)> f2 = func2;

    Break;

    swap(f1,f2);

    f1(0);
    
    Break;

    f2(0);
    
    Break;

    A a;
    // wrapper(&a,&A::func);

    using mFunc = FunctionPtr<int(int,int)>;

    mFunc f(&a,&A::func1);

    Break;

    mFunc g;

    Break;

    g = f;

    std::cout << g(1,1) << std::endl;
    
    Break;
    
    std::cout << f(1,2) << std::endl;

}