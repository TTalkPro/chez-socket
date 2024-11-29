#include <cstddef>
#include <string>

class cbuffer {

public:
  cbuffer();
  // move constructor
  cbuffer(cbuffer&& src) noexcept;
  // copy constructor
  cbuffer(const cbuffer& src);
  ~cbuffer();
  // move assignment operator
  cbuffer& operator=(cbuffer&& src) noexcept;
  // copy assignment operator.
  cbuffer& operator=(const cbuffer& src);
private:
  char *_buffer; //buffer本体
  size_t _size; // 使用了多少
  size_t _capacity; // buffer总大小

public:
  inline size_t size() const { return _size; }
  inline char* begin() const { return _buffer; }
  inline size_t capacity() const { return _capacity; }
  inline char* end() const { return _buffer + _size; }
  // 返回剩余多少空间
  inline size_t remain() const { return _capacity - _size; }
  inline void reserve(size_t size)
  {
    if (ensure(size + _size))
    {
      _size  += size;
    }
  }
  size_t add(const void *data, const size_t size);
  size_t add(cbuffer* src,const size_t size);
  size_t add(cbuffer* src) {
    return add(src, src->_size);
  }
  size_t add_reversed(const void *data, const size_t size);
  // 将数据插入指定的位置
  size_t insert(const size_t offset, const void *data, const size_t size);
  void remove(const size_t size);
  //将另外一个buffer添加到当前buffer中

  //确保有足够多的内存
  bool  ensure(const size_t size);
  //有多少读取多少
  size_t read(void *pData, const size_t nLength);
  //精确的读取
  bool read_bytes(void *pData, const size_t nLength);
  //读取一行，不包含\n
  bool read_line(std::string &str,bool peek=false);
  //清除内容
  inline void clear() {_size = 0; }


  void prefix(const char* c_str, const size_t size){
    insert(0, (void *)c_str, size);
  }
  static void reverse(const void *src, void *dst, size_t size);
};
