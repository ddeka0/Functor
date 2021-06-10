#pragma once

#include <iostream>
#include <string_view>

#ifdef DEBUG
#define PrintF std::cout<< __LINE__ <<" : "<< __PRETTY_FUNCTION__ << std::endl;
template <class T>
constexpr
std::string_view
type_name() {
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}
#else
#define PrintF 0
template <class T>
constexpr std::string type_name() {return "";}
#endif


// The FunctionBridge class template is reponsible for the ownership and
// manipulation of the underlying function object.
template<typename Ret,typename...Args>
class FunctorBridge {
    public:
        virtual ~FunctorBridge() {
            PrintF;
        }
        virtual FunctorBridge* clone() const = 0;
        virtual Ret invoke(Args... args) const = 0;
};

// body
template<typename Functor, typename Ret, typename...Args>
class SpecificFunctorBridge : public FunctorBridge<Ret, Args...> {
    Functor functor;
    public:
        template<typename FunctorFwd>
        SpecificFunctorBridge(FunctorFwd&& functor)
            : functor(std::forward<FunctorFwd>(functor)) {
            std::cout <<"Deduced type of Functor "<<type_name<Functor>() << std::endl; 
            PrintF;
        }
        virtual SpecificFunctorBridge* clone() const override {
            PrintF;
            return new SpecificFunctorBridge(functor);
        }
        virtual Ret invoke(Args... args) const override {
            PrintF;
            return functor(std::forward<Args>(args)...);
        }
};

template<typename PointerToObj,typename PointerToMemFun, typename Ret, typename...Args>
class MemFuncFunctorBridge : public FunctorBridge<Ret, Args...> {
    public:
        template<typename FwdPointerToObj,typename FwdPointerToMemFun>
        MemFuncFunctorBridge(FwdPointerToObj&& _obj,FwdPointerToMemFun&& _fun)
            : obj(std::forward<FwdPointerToObj>(_obj)),fun(std::forward<FwdPointerToMemFun>(_fun)) {
            // std::cout <<"Deduced type of Obj "<< type_name<PointerToObj>() << std::endl;
            // std::cout <<"Deduced type of Func "<< type_name<PointerToMemFun>() << std::endl; 
            PrintF;
        }
        virtual MemFuncFunctorBridge* clone() const override {
            PrintF;
            return new MemFuncFunctorBridge(obj,fun);
        }
        virtual Ret invoke(Args... args) const override {
            PrintF;
            return ((*obj).*fun)(std::forward<Args>(args)...);
        }
    private:
        PointerToObj obj;
        PointerToMemFun fun;
};


template<typename Signature>
class FunctionPtr;

// Handle
template<typename Ret,typename...Args>
class FunctionPtr<Ret(Args...)> {
    private:
        FunctorBridge<Ret,Args...>* m_bridge;
    public:
        // Default constructor
        FunctionPtr():m_bridge(nullptr){
            PrintF;
        }

        // Copy constructor
        FunctionPtr(FunctionPtr const& other) : m_bridge(nullptr) {
            PrintF;
            if(other.m_bridge) {
                m_bridge = other.m_bridge->clone();
            }
        }

        FunctionPtr(FunctionPtr& other) 
            : FunctionPtr(static_cast<FunctionPtr const&>(other)){
            PrintF;
        }

        FunctionPtr(FunctionPtr&& other) : m_bridge(other.m_bridge) {
            PrintF;
            other.m_bridge = nullptr;
        }

        template<typename F>
        FunctionPtr(F&& f) : m_bridge(nullptr) {
            PrintF;
            // F is of reference type 
            using Functor = std::decay_t<F>;
            using Bridge = SpecificFunctorBridge<Functor,Ret,Args...>;
            std::cout << __LINE__ <<" : "<<type_name<Functor>() << std::endl;
            std::cout << __LINE__ <<" : "<<type_name<Bridge>() << std::endl;
            m_bridge = new Bridge(std::forward<F>(f));
        }

        template<typename PointerToObj,typename PointerToMemFun>
        FunctionPtr(PointerToObj&& _obj,PointerToMemFun&& _fun) : m_bridge(nullptr) {
            PrintF;
            // remove references from the pointer and the method
            // becuase they will be stored inside MemFuncFunctorBridge **TODO check**
            using Bridge = MemFuncFunctorBridge<std::decay_t<PointerToObj>,std::decay_t<PointerToMemFun>,Ret,Args...>;
            //std::cout << __LINE__ <<" : "<<type_name<Bridge>() << std::endl;
            m_bridge = new Bridge(std::forward<PointerToObj>(_obj),std::forward<PointerToMemFun>(_fun));
        }

        FunctionPtr& operator=(FunctionPtr && other) {
            PrintF;
            delete m_bridge;
            m_bridge = other.m_bridge;
            other.m_bridge = nullptr;
            return *this;
        }

        template<typename F>
        FunctionPtr& operator=(F&& f) {
            PrintF;
            FunctionPtr temp(std::forward<F>(f));
            swap(*this,temp);
            return *this;
        }

        ~FunctionPtr() {
            PrintF;
            delete m_bridge;
        }

        friend void swap(FunctionPtr& fp1, FunctionPtr& fp2) {
            PrintF;
            std::swap(fp1.m_bridge,fp2.m_bridge);
        }

        explicit operator bool() const {
            PrintF;
            return m_bridge == nullptr;
        }

        Ret operator()(Args... args) const {
            PrintF;
            return m_bridge->invoke(std::forward<Args>(args)...);
        }

};