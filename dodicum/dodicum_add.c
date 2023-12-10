#include "../s21_decimal.h"

void dodicum_add(dodicum value_1, dodicum value_2, dodicum *result) {
  uint64_t remainder = 0;
  int i = 0;
  for (; i <= 12; i++) {
    uint64_t sum =
        (uint64_t)value_1.bits[i] + (uint64_t)value_2.bits[i] + remainder;

    remainder = sum > UINT32_MAX;

    result->bits[i] = sum % ((uint64_t)1 << 32);
  }

  result->bits[i] = remainder;
}

