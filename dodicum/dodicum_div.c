#include "../s21_decimal.h"

dodicum dodicum_div(dodicum dividend, dodicum divisor, dodicum *result,
                    int *extra_exp) {
  *result = ONE_DIGIT_DODICUM(0);
  dodicum whole_part = {0};
  bool found_whole_part = 0;

  dodicum copy_divisor;
  int loop_limit = 30;
  while (!is_dodicum_equal(dividend, ONE_DIGIT_DODICUM(0)) && loop_limit) {
    int shift = 0;
    copy_divisor = divisor;
    dodicum tmp_copy_divisor = {0};
    dodicum_mul(copy_divisor, ONE_DIGIT_DODICUM(UINT32_MAX), &tmp_copy_divisor);

    dodicum emergency_divisor = ONE_DIGIT_DODICUM(0);
    while (is_dodicum_less(tmp_copy_divisor, dividend)) {
      emergency_divisor = copy_divisor;
      shift++;
      dodicum_digit_shift(1, &copy_divisor);
      dodicum_mul(copy_divisor, ONE_DIGIT_DODICUM(UINT32_MAX),
                  &tmp_copy_divisor);
    }

    if (is_dodicum_greater(copy_divisor, dividend)) {
      shift--;
      copy_divisor = emergency_divisor;
    }

    while (is_dodicum_less(dividend, copy_divisor)) {
      if (!found_whole_part) {
        found_whole_part = 1;
        whole_part = dividend;
      }

      dodicum_mul(dividend, ONE_DIGIT_DODICUM(10), &dividend);
      dodicum_mul(*result, ONE_DIGIT_DODICUM(10), result);
      (*extra_exp)++;
    }

    dodicum to_mult_by = {dodicum_binary_search_v2(dividend, copy_divisor), 0,
                          0, 0};
    result->bits[shift] += to_mult_by.bits[0];
    dodicum_mul(copy_divisor, to_mult_by, &copy_divisor);
    dodicum_sub(dividend, copy_divisor, &dividend);
    loop_limit--;
  }

  return whole_part;
}
