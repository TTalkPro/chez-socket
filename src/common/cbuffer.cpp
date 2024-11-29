#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include "cbuffer.h"

#define CBUFFER_BLOCK_SIZE 1024       // 默认为1kb
#define CBUFFER_BLOCK_MASK 0xFFFFFC00 // 将数据进行round操作，最大4G
#define CBUFFER_MAX_SIZE 0xFFFFFFFF

cbuffer::cbuffer(): _buffer(NULL), _size(0), _capacity(0)
{
}

cbuffer::cbuffer(cbuffer&& src) noexcept: _buffer(NULL), _size(0), _capacity(0)
{
    std::cout << "In cbuffer(const cbuffer&&). length = "
          << src._size << ". Copying resource." << std::endl;
    _buffer = src._buffer;
    _size = src._size;
    _capacity = src._capacity;

    // Release the data pointer from the source object so that
    // the destructor does not free the memory multiple times.
    src._buffer = nullptr;
    src._size = 0;
    src._capacity = 0;
}

cbuffer::cbuffer(const cbuffer& src): _size(0),_capacity(0),_buffer(NULL)
{
    std::cout << "In cbuffer(const cbuffer&). length = "
              << src._size << ". Copying resource." << std::endl;

    if (!ensure(src._size))
    {
       throw std::bad_alloc();
    }
    memcpy(_buffer, src._buffer, src._size);
}

cbuffer::~cbuffer()
{
    std::cout << "cbuffer desctruct "<< std::endl;
    if (_buffer)
    {
        free(_buffer);
    }
    _buffer = NULL;
}

cbuffer& cbuffer::operator=(cbuffer &&src) noexcept
{
    std::cout << "In operator=(const cbuffer&&). length = "
           << src._size << ". Copying resource." << std::endl;

    if (this != &src)
    {
        // Free the existing resource.
        if (_buffer)
        {
            free(_buffer);
            _capacity = 0;
            _size = 0;
        }
        _buffer = src._buffer;
        _size = src._size;
        _capacity = src._capacity;
        src._buffer = nullptr;
        src._size = 0;
        src._capacity = 0;
    }
    return *this;
}

cbuffer& cbuffer::operator=(const cbuffer& src)
{
    std::cout << "In operator=(const cbuffer&). length = "
               << src._size << ". Copying resource." << std::endl;
    if (this != &src)
    {
        _size = 0;
        if (!ensure(src._size))
        {
            throw std::bad_alloc();
        }
        memcpy(_buffer, src._buffer, src._size);
    }
    return *this;
}


size_t cbuffer::add(const void *data, const size_t size)
{
    if (NULL == data)
    {
        return 0;
    }

    if (!ensure(size))
    {
        return 0;
    }
    memcpy(_buffer + _size, data, size);
    _size += size;
    return size;
}

size_t cbuffer::insert(const size_t offset,
                     const void *data,
                     const size_t size)
{
    if (NULL == data)
    {
        return 0;
    }

    if (!ensure(size))
    {
        return 0 ;
    }
    // 如果offset已经大于了长度，就不需要移动了
    // 之中情况是不应该出现了
    assert((_size >= offset));
    if (_size > offset)
    {
        // 先移动数据，得到相应的空间
        memmove(_buffer + offset + size, _buffer + offset,
                _size - offset);
    }
    // 插入数据
    memcpy(_buffer + offset, data, size);
    _size += size;
    return size;
}

void cbuffer::remove(const size_t size)
{
    if (size >= _size)
    {
        _size = 0;
    } else if (size)
    {
        _size -= size;
        memmove(_buffer, _buffer + size, _size);
    }
}

size_t cbuffer::add(cbuffer *src, const size_t size)
{
    assert(src && src != this);
    if (NULL == src || this == src)
    {
        return 0;
    }

    // 最大4G
    if (size > INT32_MAX)
    {
        return 0;
    }
    if (src->_size < size)
    {
        add(src->_buffer, src->_size);
        return src->_size;
    } else
    {
        add(src->_buffer, size);
        return size;
    }
}


size_t cbuffer::add_reversed(const void *data, const size_t size)
{
    if (NULL == data)
    {
        return 0;
    }

    if (!ensure(size))
    {
        return 0;
    }
    reverse(data, _buffer + _size, size);
    _size += size;
    return size;
}

bool cbuffer::ensure(const size_t size)
{
    if (size > CBUFFER_MAX_SIZE - _capacity)
    {
        return false;
    }

    if (_capacity - _size >= size)
    {
        // 分配了512KB，但是实际使用不足256KB
        // 让内存进行缩小
        if (_capacity > 0x80000 && (_size + size) < 0x40000)
        {
            char *buffer = (char *) realloc(_buffer, 0x40000);
            // 虽然重新定位失败，但是内存空间的大小是足够的
            if (!buffer)
            {
                return true;
            }
            _capacity = 0x40000;
            _buffer = buffer;
        }
        return true;
    }

    size_t capacity = _size + size;
    // 获取最合适的大小
    capacity = (capacity + CBUFFER_BLOCK_SIZE - 1) & CBUFFER_BLOCK_MASK;

    char *buffer = (char *) realloc(_buffer, capacity);
    if (!buffer)
    {
        return false;
    }
    _capacity = capacity;
    _buffer = buffer;
    return true;
}

size_t cbuffer::read(void *data, const size_t size)
{
    size_t expect_size = size;
    // 如果buffer数据小于想要的，直接全读取了
    if (size > _size)
    {
        expect_size = _size;
    }
    memcpy(data, _buffer, expect_size);
    if (expect_size == _size)
    {
        _size = 0;
    } else
    {
        remove(expect_size);
    }
    return expect_size;
}

bool cbuffer::read_bytes(void *data, const size_t size)
{
    if (size > _size)
    {
        return false;
    }
    read(data, size);
    return true;
}

bool cbuffer::read_line(std::string &ref_str, bool peek)
{
    if (0 == _size)
    {
        return false;
    }

    size_t idx = 0;

    // 寻找\n
    for (; idx < _size; idx++)
    {
        if (_buffer[idx] == '\n')
            break;
    }

    if (idx >= _size)
    {
        return false;
    }

    if (idx > 0)
    {
        ref_str.append(_buffer, idx);
    }

    if (!peek)
    {
        remove(idx + 1);
    }

    return true;
}

void cbuffer::reverse(const void *src, void *dst, size_t size)
{
    if (size)
    {
        const char *input = (const char *) src + size;
        char *output = (char *) dst;
        while (size--)
        {
            *output++ = *--input;
        }
    }
}
