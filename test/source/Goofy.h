//
// Created by dave on 01.12.20.
//

#ifndef NAMI_GOOFY_H
#define NAMI_GOOFY_H

#include <iostream>

template <typename T> class Goofy {
public:
  constexpr Goofy(T n = 0) : num(n) {}
  constexpr Goofy(const Goofy &g2) : num(g2.num) {}
  constexpr Goofy &operator=(const Goofy &g2) {
    num = g2.num;
    return *this;
  }
  constexpr Goofy &operator+=(const Goofy &g2) {
    num += g2.num;
    return *this;
  }
  constexpr bool operator<(const Goofy &g2) const { return num < g2.num; }

  friend std::ostream &operator<<(std::ostream &out, const Goofy &g2) { return out << g2.num; }

private:
  T num;
};

template <typename T> constexpr Goofy<T> operator+(const Goofy<T> &g1, const Goofy<T> &g2) {
  Goofy<T> result(g1);
  result += g2;
  return result;
}

#endif  // NAMI_GOOFY_H
