#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = ONE_DIGIT_DECIMAL(0);
  dodicum value_1_dodicum = normalize(value_1);
  dodicum value_2_dodicum = normalize(value_2);

  if (get_sign(value_1) ^ get_sign(value_2))
    return s21_sub(value_1, value_2, result);

  if (get_sign(value_1)) set_sign(1, result);

  dodicum sum_dodicum = ONE_DIGIT_DODICUM(0);
  dodicum_add(value_1_dodicum, value_2_dodicum, &sum_dodicum);

  int error_code = denormalize(sum_dodicum, result);

  return error_code;
}
