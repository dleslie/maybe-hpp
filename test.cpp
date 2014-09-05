#include <iostream>
#include "maybe.hpp"

using namespace std;

class foo {
public:
  foo() : _value(1) {
    cout << "foo created" << endl;
  }
  virtual ~foo() {
    cout << "foo destroyed" << endl;
  }

  int get_value() { return _value; }
  void set_value(int i) { _value = i; }

private:
  int _value;
};

int main(int argc, char **argv) {
  // Sizes
  {
    cout << "Size of maybe<int>: " << sizeof(maybe<int>) << endl;
    cout << "Size of maybe<foo>: " << sizeof(maybe<foo>) << endl;
  }

  // Has value
  {
    cout << "Testing has_value:" << endl;

    // Immediates
    maybe<int> no_int = maybe<int>::nothing();
    maybe<int> has_int = maybe<int>::just(1);

    cout << "Does no_int have a value? " << no_int.has_value() << endl;
    cout << "Does has_int have a value? " << has_int.has_value() << endl;

    // Non-immediates
    maybe<foo> no_foo = maybe<foo>::nothing();
    maybe<foo> has_foo = maybe<foo>::just(new foo());

    cout << "Does no_foo have a value? " << no_foo.has_value() << endl;
    cout << "Does has_foo have a value? " << has_foo.has_value() << endl;
  }

  cout << endl;

  // Copy
  {
    cout << "Testing copy:" << endl;

    maybe<int> nothing_a = maybe<int>::nothing();
    maybe<int> nothing_b = nothing_a;

    cout << "Does nothing_a have a value? " << nothing_a.has_value() << endl;
    cout << "Does nothing_b have a value? " << nothing_b.has_value() << endl;

    maybe<int> int_a = maybe<int>::just(1);
    maybe<int> int_b = int_a;

    cout << "Does int_a have a value? " << int_a.has_value() << endl;
    cout << "Does int_b have a value? " << int_b.has_value() << endl;
    cout << "Value of int_b: " << int_b.get() << endl;

    maybe<foo> foo_a = maybe<foo>::just_emplace();
    maybe<foo> foo_b = foo_a;

    cout << "Does foo_a have a value? " << foo_a.has_value() << endl;
    cout << "Does foo_b have a value? " << foo_b.has_value() << endl;
    cout << "Value of foo_b: " << foo_b.get().get_value() << endl;
  }

  return 0;
}
