//
// Created by david on 12/3/24.
//

#ifndef BASE_HANDLER_H
#define BASE_HANDLER_H
#include <memory>

class reactor;
class base_handler : public std::enable_shared_from_this<base_handler> {
  friend class reactor;
public:
  base_handler() = default;
  virtual ~base_handler() = default;

protected:
  void attach(const std::shared_ptr<reactor> &r) {
    _intance = r;
  };
  std::shared_ptr<reactor> attached() const {
    return _intance;
  };

private:
  std::shared_ptr<reactor> _intance;

};


#endif //BASE_HANDLER_H