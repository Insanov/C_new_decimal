#include "../s21_decimal.h"

dodicum normalize(s21_decimal decimal) {
  dodicum result = get_dodicum_from_decimal(decimal);

  for (int i = get_exp(decimal); i < 28; i++) {
    dodicum dodicum_ten = {10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    dodicum_mul(result, dodicum_ten, &result);
  }

  return result;
}

int denormalize(dodicum dodic, s21_decimal *result) {
  int exponent = dodic.bits[14] + 28;
  dodicum dodic_copy = dodic;
  int extra_exp_placeholder;

  while (is_divisible_by_10(dodic_copy) && exponent > 0) {
    dodicum_div(dodic_copy, ONE_DIGIT_DODICUM(10), &dodic_copy,
                &extra_exp_placeholder);
    exponent--;
  }

  if (exponent <= 28 && does_dodicum_fit_in_decimal(dodic_copy)) {
    result->bits[0] = dodic_copy.bits[0];
    result->bits[1] = dodic_copy.bits[1];
    result->bits[2] = dodic_copy.bits[2];
    set_exp(exponent, result);

    return 0;
  } else {
    return 1;
  }
}

int does_dodicum_fit_in_decimal(dodicum dodic) {
  for (int i = 12; i >= 3; i--)
    if (dodic.bits[i])
      return 0;

  return 1;
}

int is_dodicum_odd(dodicum src) {
  dodicum res = ONE_DIGIT_DODICUM(0);
  int stub = 0;
  // TODO(M || V): handle extra_exp
  dodicum whole_part = dodicum_div(src, ONE_DIGIT_DODICUM(1), &res, &stub);

  return !(dodicum_get_last_digit(whole_part) % 2);
}

void bank_rounding(dodicum src, int extra_exp) {
  int last_digit = 0;
  dodicum stub = ONE_DIGIT_DODICUM(0);
  dodicum src_copy = src;
  int unused_exp = 0;

  while (extra_exp-- && !does_dodicum_fit_in_decimal(src)) {    
    last_digit = dodicum_get_last_digit(src_copy);
    src_copy = dodicum_div(src_copy, ONE_DIGIT_DODICUM(10), &stub, &unused_exp);
    if (last_digit > 5 || (last_digit == 5 && is_dodicum_odd(src_copy)))
      dodicum_add(src_copy, ONE_DIGIT_DODICUM(1), &src_copy);
  } 
}

s21_decimal decimal_abs(s21_decimal src) {
  s21_decimal result = src;
  set_sign(0, &result);

  return result;
}

int dodicum_get_last_digit(dodicum dodic) {
  int last_digit = 0;

  for (int i = 12; i >= 0; i--) {
    last_digit += ((uint64_t)dodic.bits[i] % 10) * 6;
    last_digit %= 10;
  }

  return last_digit;
}

bool is_divisible_by_10(dodicum number) {
  return !dodicum_get_last_digit(number);
}

dodicum get_dodicum_from_decimal(s21_decimal decimal) {
  dodicum result = ONE_DIGIT_DODICUM(0);

  for (int i = 0; i <= 2; i++)
    result.bits[i] = decimal.bits[i];

  return result;
}

void mul_dodicum_by_digit(int shift, dodicum multiplicand,
                          uint64_t multiplicator, dodicum *product) {
  uint64_t remainder = 0;
  for (int i = 0; i <= 6; i++) {
    uint64_t tmp_product =
        (uint64_t)multiplicand.bits[i] * multiplicator + remainder;

    product->bits[i + shift] = tmp_product % ((int64_t)1 << 32);
    remainder = tmp_product / ((int64_t)1 << 32);
  }

  product->bits[7 + shift] = remainder;
}

int get_exp(s21_decimal decimal) {
  int tmp_exp = decimal.bits[3] << 1;
  tmp_exp >>= 17;
  return (tmp_exp > -1 && tmp_exp < 29) ? tmp_exp : -1;
}

void dodicum_digit_shift(int shift, dodicum *result) {
  dodicum tmp = {0};
  for (int i = 0; i <= 12 - shift; i++)
    tmp.bits[i + shift] = result->bits[i];

  for (int i = 0; i <= 12; i++)
    result->bits[i] = tmp.bits[i];
}

int is_dodicum_less(dodicum first, dodicum second) {
  for (int i = 12; i >= 0; i--)
    if (first.bits[i] < second.bits[i])
      return 1;
    else if (first.bits[i] > second.bits[i])
      return 0;

  return 0;
}

int is_dodicum_greater(dodicum first, dodicum second) {
  return !is_dodicum_less_or_equal(first, second);
}

int get_digits_count(dodicum dodic) {
  int res = 0;
  for (int i = 12; i >= 0; i--)
    if (dodic.bits[i])
      return i + 1;

  return res;
}

int is_dodicum_equal(dodicum first, dodicum second) {
  for (int i = 12; i >= 0; i--)
    if (first.bits[i] != second.bits[i])
      return 0;

  return 1;
}

int is_dodicum_less_or_equal(dodicum first, dodicum second) {
  return is_dodicum_less(first, second) || is_dodicum_equal(first, second);
}

int dodicum_binary_search_v2(dodicum dividend, dodicum divisor) {
  uint32_t step = UINT32_MAX / 2 + 1, half_step = step / 2;
  uint32_t last_lesser = 0;

  dodicum product = divisor;
  for (int i = 0; i < 32; i++) {
    dodicum_mul(divisor, ONE_DIGIT_DODICUM(step), &product);

    if (is_dodicum_equal(product, dividend)) {
      return step;
    } else if (is_dodicum_greater(product, dividend)) {
      step -= half_step;
    } else {
      last_lesser = step;
      step += half_step;
    }

    half_step /= 2;
  }

  return last_lesser;
}

void toggle_sign(s21_decimal *src) {
  set_sign(!get_sign(*src), src);
}

void set_exp(int exp, s21_decimal *decimal) { decimal->bits[3] = exp << 16; }

bool get_sign(s21_decimal decimal) { return decimal.bits[3] >> 31; }
