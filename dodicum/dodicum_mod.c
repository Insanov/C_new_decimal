#include "../s21_decimal.h"

dodicum dodicum_mod(dodicum dividend, dodicum divisor, int *extra_exp) {
  dodicum div_result = ONE_DIGIT_DODICUM(0);
  dodicum result = ONE_DIGIT_DODICUM(0);
  dodicum whole_part = dodicum_div(dividend, divisor, &div_result, extra_exp);

  dodicum_mul(whole_part, divisor, &whole_part);
  dodicum_sub(dividend, whole_part, &result);

  return result;
}
