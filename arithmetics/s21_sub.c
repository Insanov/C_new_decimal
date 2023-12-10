#include "../s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = ONE_DIGIT_DECIMAL(0);
  s21_decimal minuend = value_2;
  s21_decimal subtrahend = value_1;

  if (s21_is_greater(decimal_abs(value_1), decimal_abs(value_2))) {
    minuend = value_1;
    subtrahend = value_2;
  }

  toggle_sign(&subtrahend);

  if (get_sign(minuend) == get_sign(subtrahend))
    return s21_add(minuend, subtrahend, result);

  if (s21_is_greater(subtrahend, minuend)) set_sign(1, result);

  dodicum value_1_dodicum = normalize(minuend);
  dodicum value_2_dodicum = normalize(subtrahend);

  dodicum sum_dodicum = ONE_DIGIT_DODICUM(0);
  dodicum_add(value_1_dodicum, value_2_dodicum, &sum_dodicum);

  int error_code = denormalize(sum_dodicum, result);

  return error_code;
}
