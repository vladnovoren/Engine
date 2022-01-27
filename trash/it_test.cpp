#include <cstdio>

enum class TypeIds {
  CE_A,
  CE_B
};

int counter = 0;

class A {
 public:
  TypeIds id = TypeIds::CE_A;
};


class B {
 public:
  TypeIds id = TypeIds::CE_B;
};


int main() {
  A a1;
  B b1;
  A a2;
  printf("%d %d %d\n", a1.id, b1.id, a2.id);
  return 0;
}