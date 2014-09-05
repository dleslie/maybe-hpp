#ifndef MAYBE_HPP
#define MAYBE_HPP

#include <type_traits>
#include <memory>

template<typename T>
class maybe {
public:
  static maybe<T> just(T _val) {
    return maybe<T>(_val);
  }

  template<typename... Targs>
  static maybe<T> just_emplace(Targs... args) {
    return new T(args...);
  }

  static maybe<T> just(T *_val) {
    return maybe<T>(_val);
  }

  static maybe<T> nothing() {
    return maybe<T>();
  }

  bool has_value() const {
    return container.has_value();
  }

  T &get() {
    return container.get();
  }
  
  const T &get() const {
    return container.get();
  }

  inline operator bool () const {
    return has_value();
  }

  maybe(T _val) 
    : container(_val) {
  }

  maybe(T *_val) 
    : container(_val) {
  }

  maybe() {
  }

  maybe(maybe<T> &other) {
    container = other.container;
  }

  maybe(maybe<T> &&other) {
    container = other.container;
  }
  
  inline maybe<T> &operator = (maybe<T> &right) {
    container = right.container;
    return *this;
  }

  inline maybe<T> &operator = (maybe<T> right) {
    container = right.container;
    return *this;
  }

private:
  class maybe_container_trivial {
  public:
    maybe_container_trivial(T _val)
      : has_data(true), data(_val) {
    }

    maybe_container_trivial(T *_val)
      : has_data(true), data(_val) {
    }

    maybe_container_trivial()
      : has_data(false){
    }

    maybe_container_trivial(maybe_container_trivial &&other) {
      data = other.data;
      has_data = other.has_data;
      other.has_data = false;
    }

    maybe_container_trivial &operator = (maybe_container_trivial &right) {
      has_data = right.has_data;
      data = right.eject();
      return *this;
    }

    T &eject() {
      has_data = false;
      return data;
    }

    const T &get() const {
      return data;
    }

    T &get() {
      return data;
    }

    const bool has_value() const {
      return has_data;
    }

  private:
    bool has_data;
    T data;
  };
  
  class maybe_container_non_trivial {
  public:
    maybe_container_non_trivial(T _val)
      : data(std::move(_val)) {
    }

    maybe_container_non_trivial(T *_val)
      : data(std::move(_val)) {
    }

    maybe_container_non_trivial()
      : data(nullptr) {
    }

    maybe_container_non_trivial(maybe_container_non_trivial &&other) {
      data = other.data;
      other.data.reset(nullptr);
    }

    T &get() {
      return *data;
    }
 
    const T &get() const {
      return *data;
    }

    maybe_container_non_trivial &operator = (maybe_container_non_trivial &right) {
      data = right.eject();
      return *this;
    }

    std::unique_ptr<T> eject() {
      return std::unique_ptr<T>(data.release());
    }

    const bool has_value() const {
      return data != nullptr;
    }

  private:
    std::unique_ptr<T> data;
  };

  typedef typename std::conditional<
    std::is_fundamental<T>::value,
    maybe_container_trivial,
    maybe_container_non_trivial>::type maybe_container_t;

  maybe_container_t container;
};


template<typename T, typename... Targs>
maybe<T> new_maybe(Targs... args) {
  T *data = new T(args...);
  if (data != nullptr)
    return maybe<T>::just(data);
  else
    return maybe<T>::nothing();
}

#endif
