#include <iostream>

void print_hi() { std::cout << "hi" << std::endl;}
void print_foo() { std::cout << "foo" << std::endl;}

struct Action
{
  void (*a)(void) = print_hi;
  Action (void (*x) (void)):a(x){}
};

int main()
{
  // void f(void) = print_hi;
  void (*action)(void) = print_hi;
  action();
  action = print_foo;
  action();

  Action a(print_foo);
  a.a();
  // a.a = print_foo;
  // a.a();
  Action b([](){std::cout << "bar" << std::endl;});
  b.a();
  return 0;
}
