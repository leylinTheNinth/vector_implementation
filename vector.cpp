#include<iostream>
#include<initializer_list>
#include<algorithm>
#include<new>

using namespace std;


template<typename A>
class My_Allocator{
	public:
	A* allocate(size_t n); // allocate space on heap for n objects of type A
	void deallocate(A* ptr, size_t n); // deallocate space on heap pointed by ptr
	void construct(A* ptr, const A& v = A{}); // construct object of type A at ptr
	void destroy(A* ptr); // destruct object at ptr of type A
};

template<typename A>
A* My_Allocator<A>::allocate(size_t n){
	return static_cast<A*>(operator new[](n*sizeof(A)));
}

template<typename A>
void My_Allocator<A>::deallocate(A* ptr, size_t n){
	operator delete[](ptr);
}

template<typename A>
void My_Allocator<A>::construct(A* ptr, const A& v){
	new(ptr) A{v};
}

template<typename A>
void My_Allocator<A>::destroy(A* ptr){
	ptr->~A();
}


template<typename T, typename A = My_Allocator<T>>
class vector{
    A alloc;
    size_t sz;  // size of our vector
    T* elem;  // pointer to array declared on heap
    size_t space; // capacity of total allocated space for our vector

    public:
    vector(): sz{0}, elem{nullptr}, space{0} {} // default constructor

    explicit vector(size_t size, const T& t = T{});  // constructor
    vector(std::initializer_list<T> lst);  // for brace type initialization

    vector(const vector& arg); // copy constructor
    vector& operator= (const vector& v);  // assignment operator

    vector(vector&& v); // move constructor
    vector& operator= (vector&& v) ; // move assignment

    T& operator[](size_t i); // [] operator
    T operator[](size_t i) const; // const [] operator

    void reserve(size_t newalloc); // for adding new space for elements
    size_t capacity() const {return space;} // total space allocated
    size_t size() const {return sz;}  // current size of vector
    void resize(size_t newsize, T t = T{}); // reserving and initializing new elements
    void push_back(const T& t); // pushing element at the end 
 
    ~vector(); // destructor
};

template<typename T, typename A>
vector<T, A>::vector(std::initializer_list<T> lst): sz{lst.size()}, elem{alloc.allocate(sz)}, space{sz}{
    auto itr = lst.begin();
    for(size_t i = 0; i < lst.size(); ++i){
        alloc.construct(&elem[i], *itr);
        ++itr;
    }
}

template<typename T, typename A>
vector<T, A>::vector(size_t s, const T& t): sz{s}, elem{alloc.allocate(sz)}, space{sz}{
    for(size_t i = 0; i < sz; ++i){
        alloc.construct(&elem[i], t);
    }
}

template<typename T, typename A>
vector<T, A>::vector(const vector& arg): sz{arg.sz}, elem{alloc.allocate(sz)}, space{sz}{
    for(size_t i = 0; i < sz; ++i){
        alloc.construct(&elem[i], arg[i]);
    }
}

template<typename T, typename A>
vector<T, A>& vector<T, A>::operator= (const vector& v){
    if(this == &v)
        return *this;
    if(v.sz <= space){
        for(size_t i = 0; i < v.sz; ++i){
            alloc.construct(&elem[i], v[i]);
        }
        sz = v.sz;
        return *this;  
    }
    T* ptr = alloc.allocate(v.sz);
    for(size_t i = 0; i < v.sz; ++i){
        alloc.construct(&ptr[i], v[i]);
    }
    for(size_t i = 0; i < sz; ++i){
        alloc.destroy(&elem[i]);
    }
    alloc.deallocate(elem, space);
    elem = ptr;
    sz = v.sz;
    space = sz;
    return *this;
}

template<typename T, typename A>
vector<T, A>::vector(vector&& v): sz{v.sz}, elem{v.elem}, space{v.space}{
    v.sz = 0;
    v.elem = nullptr;
    v.space = 0;
}

template<typename T, typename A>
vector<T, A>& vector<T, A>::operator= (vector&& v){
    for(size_t i = 0; i < sz; ++i){
        alloc.destroy(&elem[i]);
    }
    alloc.deallocate(elem, space);
    elem = v.elem;
    sz = v.sz;
    v.sz = 0;
    space = v.space;
    v.space = 0;
    v.elem = nullptr;
    return *this;
}

template<typename T, typename A>
T& vector<T, A>::operator[](size_t i){
    return elem[i];
}

template<typename T, typename A>
T vector<T, A>::operator[](size_t i) const{
    return elem[i];
}

template<typename T, typename A>
vector<T, A>::~vector(){
    std::cout << "destructor called\n";
    for(size_t i = 0; i < sz; ++i){
        alloc.destroy(&elem[i]);
    }
    alloc.deallocate(elem, space);
}

template<typename T, typename A>
void vector<T, A>::reserve(size_t newalloc){
    if(newalloc <= space)
        return;
    T* ptr = alloc.allocate(newalloc);
    for(size_t i = 0; i < sz; ++i){
        alloc.construct(&ptr[i], elem[i]);
    }
    for(size_t i = 0; i < sz; ++i){
        alloc.destroy(&elem[i]);
    }
    alloc.deallocate(elem, space);
    elem = ptr;
    space = newalloc;
}

template<typename T, typename A>
void vector<T, A>::resize(size_t newsize, T t){
    vector::reserve(newsize);
    for(size_t i = sz; i < newsize; ++i){
        alloc.construct(&elem[i], t);
    }
    for(size_t i = newsize; i < sz; ++i){
        alloc.destroy(&elem[i]);
    }
    sz = newsize;
}

template<typename T, typename A>
void vector<T, A>::push_back(const T& t){
    if(space == 0){
        vector::reserve(8); 
    }
    else if(sz == space){
        vector::reserve(2*space);
    }
    alloc.construct(&elem[sz], t);
    ++sz;
}

int main(){
  //  vector<int> a(5, 1);
    vector<vector<int>> dp(4, vector<int>(3, 4));
   // dp.resize(2, vector<int>(6, 7));
    
    for(int i = 0; i < dp.size(); ++i){
        for(int j = 0; j < dp[i].size(); ++j){
            cout << dp[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
