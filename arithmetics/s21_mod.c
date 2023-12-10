#include "../s21_decimal.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = ONE_DIGIT_DECIMAL(0);
  dodicum value_1_dodicum = normalize(value_1);
  dodicum value_2_dodicum = normalize(value_2);

  if (get_sign(value_1) ^ get_sign(value_2)) set_sign(1, result);

  // TODO(M || V) = check infinite values
  int extra_exp = 0;
  dodicum mod_dodicum = ONE_DIGIT_DODICUM(0);
  mod_dodicum = dodicum_mod(value_1_dodicum, value_2_dodicum, &extra_exp);
  // TODO(M || V) = handle extra_exp and modision by 0

  int error_code = denormalize(mod_dodicum, result);

  return error_code;
}
