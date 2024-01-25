# My Allocator and Vector Implementation

This C++ code provides an implementation of a custom allocator, `My_Allocator`, and a dynamic array container, `vector`, designed for memory management and dynamic array operations.

## My_Allocator Class

The `My_Allocator` class is a generic allocator that facilitates manual memory management for objects of type `A`. It provides functions to allocate, deallocate, construct, and destroy objects.

### Functions:

- `allocate(size_t n)`: Allocates space on the heap for `n` objects of type `A`.
- `deallocate(A* ptr, size_t n)`: Deallocates space on the heap pointed by `ptr`.
- `construct(A* ptr, const A& v = A{})`: Constructs an object of type `A` at the specified pointer `ptr`.
- `destroy(A* ptr)`: Destructs the object at the specified pointer `ptr` of type `A`.

## Vector Class

The `vector` class is a dynamic array container that utilizes the custom allocator `My_Allocator` for memory management. It supports various functionalities such as resizing, reserving space, and moving elements.

### Constructors:

- `vector()`: Default constructor.
- `explicit vector(size_t size, const T& t = T{})`: Constructs a vector of a given size and initializes elements with the provided value.
- `vector(std::initializer_list<T> lst)`: Constructs a vector using brace-type initialization.

### Member Functions:

- `reserve(size_t newalloc)`: Reserves space for adding new elements.
- `capacity() const`: Returns the total space allocated.
- `size() const`: Returns the current size of the vector.
- `resize(size_t newsize, T t = T{})`: Resizes the vector and initializes new elements.
- `push_back(const T& t)`: Adds an element to the end of the vector.

### Operators:

- `operator[](size_t i)`: Returns a reference to the element at index `i`.
- `operator[](size_t i) const`: Returns the element at index `i` in a const context.

### Copy and Move Operations:

- Copy Constructor: `vector(const vector& arg)`
- Assignment Operator: `vector& operator= (const vector& v)`
- Move Constructor: `vector(vector&& v)`
- Move Assignment Operator: `vector& operator= (vector&& v)`

### Destructor:

- `~vector()`: Destructs the vector and frees the allocated memory.

## Example Usage

```cpp
vector<vector<int>> dp(4, vector<int>(3, 4));

for(int i = 0; i < dp.size(); ++i){
    for(int j = 0; j < dp[i].size(); ++j){
        cout << dp[i][j] << " ";
    }
    cout << endl;
}
