/*
  Copyright (c) Daniel Leslie <dan@ironoxide.ca>, All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. Neither the name of the nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MAYBE_HPP
#define MAYBE_HPP

#include <type_traits>
#include <memory>

template<typename T, bool use_trivial = std::is_fundamental<T>::value>
class maybe {
public:
  static maybe<T, use_trivial> just(T _val) {
    return maybe<T, use_trivial>(_val);
  }

  template<typename... Targs>
  static maybe<T, use_trivial> just_emplace(Targs... args) {
    return new T(args...);
  }

  static maybe<T, use_trivial> just(T *_val) {
    return maybe<T, use_trivial>(_val);
  }

  static maybe<T, use_trivial> nothing() {
    return maybe<T, use_trivial>();
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

  maybe(maybe<T, use_trivial> &other) {
    container = other.container;
  }

  maybe(maybe<T, use_trivial> &&other) {
    container = other.container;
  }
  
  inline maybe<T, use_trivial> &operator = (maybe<T, use_trivial> &right) {
    container = right.container;
    return *this;
  }

  inline maybe<T, use_trivial> &operator = (maybe<T, use_trivial> right) {
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
      : has_data(true), data(std::move(*_val)) {
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
      : data(new T(_val)) {
      
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
    use_trivial,
    maybe_container_trivial,
    maybe_container_non_trivial>::type maybe_container_t;

  maybe_container_t container;
};

#endif
