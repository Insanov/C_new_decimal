#include "../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = ONE_DIGIT_DECIMAL(0);
  dodicum value_1_dodicum = normalize(value_1);
  dodicum value_2_dodicum = normalize(value_2);

  if (get_sign(value_1) ^ get_sign(value_2)) set_sign(1, result);

  // TODO(M || V) = check infinite values
  dodicum mul_dodicum = ONE_DIGIT_DODICUM(0);
  dodicum_mul(value_1_dodicum, value_2_dodicum, &mul_dodicum);

  int error_code = denormalize(mul_dodicum, result);

  return error_code;
}
