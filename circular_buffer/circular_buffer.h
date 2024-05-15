#pragma once
#include <array>
#include <utility>

template<typename T, std::size_t CAPACITY>
class circular_buffer
{
public:
    circular_buffer() = default;
    circular_buffer(const circular_buffer& other) = delete;
    circular_buffer(circular_buffer&& other) = delete;
    circular_buffer& operator=(const circular_buffer& other) = delete;
    circular_buffer& operator=(circular_buffer&& other) = delete;

    ~circular_buffer();

    auto push(T&& other) -> void;
    template<typename... Args>
    auto push(Args&&... args) -> void;
    auto pop() -> void;
    auto top() const -> T;
    auto empty() const -> bool;
    auto full() const -> bool;
    auto size() const -> std::size_t;
    auto capacity() const -> std::size_t;
    auto clear()-> void;
private:
    template<typename... Args>
    auto add_item(Args&&... args) -> void;
    auto remove_item() -> void;
    auto write_ptr() -> unsigned char*;
    auto read_ptr() const -> const unsigned char*;
    auto get() const -> const T&;

private:
    std::array<unsigned char, CAPACITY * sizeof(T)> buffer_;
    std::size_t head_{0};
    std::size_t tail_{0};
    std::size_t size_{0};
};

template<typename T, std::size_t CAPACITY>
circular_buffer<T, CAPACITY>::~circular_buffer()
{
    clear();
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::push(T&& other) -> void
{
    add_item(std::forward<T>(other));
}

template<typename T, std::size_t CAPACITY>
template<typename... Args>
auto circular_buffer<T, CAPACITY>::push(Args&&... args) -> void
{
    add_item(std::forward<Args>(args)...);
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::pop() -> void
{
    remove_item();
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::top() const -> T
{
    return get();
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::empty() const -> bool
{
    return size_ == 0;
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::full() const -> bool
{
    return size() == capacity();
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::size() const -> std::size_t
{
    return size_;
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::capacity() const -> std::size_t
{
    return CAPACITY;
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::clear() -> void
{
    while(!empty())
    {
        pop();
    }
}

template<typename T, std::size_t CAPACITY>
template<typename... Args>
auto circular_buffer<T, CAPACITY>::add_item(Args&&... args) -> void
{
    if(!full())
    {
        new (write_ptr()) T(std::forward<Args>(args)...);
        head_ = (head_ + 1) % CAPACITY;
        size_ += 1;
    }
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::remove_item() -> void
{
    if(!empty())
    {
        get().~T();
        tail_ = (tail_+1) % CAPACITY ;
        size_ -= 1;
    }
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::write_ptr() -> unsigned char*
{
    return &buffer_[head_*sizeof(T)];
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::read_ptr() const -> const unsigned char*
{
    return &buffer_[tail_*sizeof(T)];
}

template<typename T, std::size_t CAPACITY>
auto circular_buffer<T, CAPACITY>::get() const -> const T&
{
    return *reinterpret_cast<const T*>(read_ptr());
}
