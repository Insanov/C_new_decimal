#include "../s21_decimal.h"

// TODO(Vi): -0 == 0
int s21_is_equal(s21_decimal first, s21_decimal second) {
  bool is_same_sign = get_sign(first) == get_sign(second);
  bool is_same_exp = get_exp(first) == get_exp(second);
  bool is_same_value = 1;

  for (int i = 0; is_same_value && i <= 2; i++) {
    if (first.bits[i] != second.bits[i])
      is_same_value = 0;
  }

  return is_same_sign && is_same_exp && is_same_value;
}
