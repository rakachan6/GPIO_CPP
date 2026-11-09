#ifndef GPIO_CPP_RINGBUFFER_H
#define GPIO_CPP_RINGBUFFER_H
#include <stm32f4xx.h>

enum class Ringbuffer_status_t
{
    OK,
    FULL,
    EMPTY
};

template <typename T, size_t N>
class Ringbuffer
{
    static_assert(N > 0, "N must be greater than 0");
public:
    Ringbuffer() = default;
    Ringbuffer_status_t put(T item)
    {
        size_t new_head = (head + 1) % N;
        if (new_head == tail)
            return Ringbuffer_status_t::FULL;

        data[head] = item;
        head = new_head;
        return Ringbuffer_status_t::OK;
    }
    Ringbuffer_status_t get(T& item)
    {
        if (head == tail)
            return Ringbuffer_status_t::EMPTY;

        item = data[tail];
        tail = (tail + 1) % N;
        return Ringbuffer_status_t::OK;
    }
    bool is_full() const
    {
        size_t new_head = (head + 1) % N;
        if (new_head == tail)
            return true;
        else
            return false;
    }
    bool is_empty() const
    {
        if (head == tail)
            return true;
        else
            return false;
    }
    Ringbuffer_status_t put_array(T* items, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            //buffer is full
            if (put(items[i]) == Ringbuffer_status_t::FULL)
                return Ringbuffer_status_t::FULL;
        }
        return Ringbuffer_status_t::OK;
    }

private:
    T data[N];
    size_t head = 0;
    size_t tail = 0;
};


#endif //GPIO_CPP_RINGBUFFER_H
