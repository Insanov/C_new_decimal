#include <string.h>

#include "../s21_decimal.h"

#define SIZE 100

int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
void set_bit_1(int index, unsigned int *num);
void set_sign_1(s21_decimal *decimal);
void dodicum_add(dodicum value_1, dodicum value_2, dodicum *result);
void dodicum_mul(dodicum multiplicand, dodicum multiplicator, dodicum *product);
void mul_dodicum_by_digit(int shift, dodicum multiplicand,
                          uint64_t multiplicator, dodicum *product);
void dodic_print(dodicum number);
bool get_sign(s21_decimal decimal);
void one_digit_decimal_adding(double whole_part, s21_decimal *result);

int main() {
  float dest = 12.1325;

  s21_decimal res = {0};
  s21_from_float_to_decimal(dest, &res);

  // s21_decimal src = {UINT32_MAX, UINT32_MAX, UINT32_MAX, 16 << 16};
  // int is_error = s21_from_decimal_to_float(src ,&dest);
  // printf("%d\n%.7f\n", is_error, dest);
  return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  float tmp = 0;
  s21_from_decimal_to_float(value, &tmp);

  double whole_part = floor(tmp);
  one_digit_decimal_add(whole_part, result);
}

int s21_round(s21_decimal value, s21_decimal *result) {
  float tmp = 0;
  s21_from_decimal_to_float(value, &tmp);

  double whole_part = round(tmp);
  one_digit_decimal_add(whole_part, result);
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  float tmp = 0;
  s21_from_decimal_to_float(value, &tmp);

  double whole_part = 0;
  double rubbish_left_part = modf(tmp, &whole_part);
  one_digit_decimal_add(whole_part, result);
}

void one_digit_decimal_adding(double whole_part, s21_decimal *result) {
  while (whole_part - UINT32_MAX > 0)
    s21_add(*result, ONE_DIGIT_DECIMAL(UINT32_MAX), result);
  s21_add(*result, ONE_DIGIT_DECIMAL((uint32_t)whole_part), result);
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  set_sign(!get_sign(value), result);
}

int s21_from_float_to_decimal(
    float src,
    s21_decimal
        *dst) {  // rounding may be false, but I don't remember in which case :)
  // TODO(MV): remove all prints
  long double new_src = (long double)src;
  char str[SIZE] = {0};
  char significant_digits[SIZE] = {0};
  int zeros_count = 0;
  int exp = 0;
  dodicum dodic_for_convert = {0};

  if (new_src < 0) set_sign_1(dst);

  new_src = fabs(new_src);
  snprintf(str, SIZE, "%.36Lf", new_src);
  printf("FLOAT - %.36Lf\n", new_src);

  if (new_src < 1) {  // convert to common view
    zeros_count = (int)strcspn(str, "123456789");
    if (zeros_count > 29) return 1;

    new_src *= pow(10, zeros_count + 6);
    exp = zeros_count - 2 + 7;
    snprintf(str, SIZE, "%.36Lf", new_src);
  } else {
    exp = (int)strcspn(str, ".");
    if (exp > 29) return 1;  // TODO(MV): handle exp 28, 29
    exp = (exp < 7) ? 7 - exp : exp;
  }
  char *dot_adress = strchr(str, '.');
  size_t copy_digits_amount = (dot_adress - str > 7) ? 7 : dot_adress - str;
  strncpy(significant_digits, str, copy_digits_amount);
  strncpy(significant_digits + strlen(significant_digits), dot_adress + 1,
          7 - copy_digits_amount);

  dodic_for_convert.bits[0] = atoi(significant_digits);
  int for_round = (exp < 8) ? 8 : 7;

  if (str[for_round] && str[for_round] >= 53 && str[for_round] <= 57)
    dodic_for_convert.bits[0]++;
  printf("ROUNDED_SIGNIFICANT_PART - %d\n", dodic_for_convert.bits[0]);
  if (zeros_count) {
    for (int i = 0; i < 28 - exp; i++)  // for src < 1
      dodicum_mul(dodic_for_convert, ONE_DIGIT_DODICUM(10), &dodic_for_convert);
    exp = 28;
    // TODO(M || V): denormalization or may be it isn't necessary
    dodic_print(dodic_for_convert);
  } else {
    for (int i = 0; exp - i - 7 > 0; i++)  // for new_src > 1
      dodicum_mul(dodic_for_convert, ONE_DIGIT_DODICUM(10), &dodic_for_convert);
    exp = (exp > 7) ? 0 : exp;
    // TODO(M || V): denormalization or may be it isn't necessary
    dodic_print(dodic_for_convert);
  }
  printf("%d\n", exp);

  return 0;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int is_negative = 0;
  if (get_sign(src)) {
    is_negative = 1;
    src.bits[3] -= 1 << 31;
  }
  long double divisor = pow(10, src.bits[3] >> 16);
  long double tmp =
      (pow(2, 64) * src.bits[2] + pow(2, 32) * src.bits[1] + src.bits[0]) /
      divisor;
  *dst = (is_negative) ? -1 * tmp : tmp;

  return ((fabs)(tmp) > 79228162514264337593543950335.0) ? 1 : 0;
}

void set_sign_1(s21_decimal *decimal) { set_bit_1(31, decimal->bits + 3); }

void set_bit_1(int index, unsigned int *num) { *num |= 1 << index; }

void dodicum_mul(dodicum multiplicand, dodicum multiplicator,
                 dodicum *product) {
  dodicum product_arr[7] = {0};

  for (int i = 0; i <= 6; i++)
    mul_dodicum_by_digit(i, multiplicand, multiplicator.bits[i],
                         product_arr + i);

  dodicum sum = {0};
  for (int i = 0; i <= 6; i++) dodicum_add(product_arr[i], sum, &sum);

  for (int i = 0; i <= 14; i++) {
    product->bits[i] = sum.bits[i];
  }
}

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

void dodic_print(dodicum number) {
  for (int i = 0; i < 15; i++) {
    printf("%u", number.bits[i]);
    if (i < 14) printf(", ");
  }
  printf("\n");
}

bool get_sign(s21_decimal decimal) { return decimal.bits[3] >> 31; };
