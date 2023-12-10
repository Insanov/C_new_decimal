#ifndef SRC_S21_DECIMAL_
#define SRC_S21_DECIMAL_

#include <check.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                    \
  ((byte & 0x80) ? '1' : '0'), ((byte & 0x40) ? '1' : '0'),     \
      ((byte & 0x20) ? '1' : '0'), ((byte & 0x10) ? '1' : '0'), \
      ((byte & 0x08) ? '1' : '0'), ((byte & 0x04) ? '1' : '0'), \
      ((byte & 0x02) ? '1' : '0'), ((byte & 0x01) ? '1' : '0')
#define MAX_LOWER 1 << 96
#define ONE_DIGIT_DODICUM(N) \
  (dodicum) { N, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#define ONE_DIGIT_DODICUM_WITH_EXP(N, M) \
  (dodicum) { N, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, M }
#define ONE_DIGIT_DECIMAL(N) \
  (s21_decimal) { N, 0, 0, 0 }

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[15];
} dodicum;

int s21_from_int_to_decimal(int src, s21_decimal *dst);  // +
int s21_from_decimal_to_int(s21_decimal src, int *dst);  // +
int s21_from_float_to_decimal(float src, s21_decimal *dst);

void print_dec(s21_decimal decimal);
bool get_bit(int bit, unsigned int num);
void set_bit_0(int index, unsigned int *num);
// void set_bit_1(int index, unsigned int *num);
int get_exp(s21_decimal decimal);
// void set_exp(int exp, s21_decimal *decimal);
bool get_sign(s21_decimal decimal);
void set_sign(bool sign, s21_decimal *decimal);
// void set_sign_1(s21_decimal *decimal);
void set_sign_0(s21_decimal *decimal);
bool is_whole(double value);
void decrease_exponent_and_mul(int change, s21_decimal *decimal);
void toggle_sign(s21_decimal *src);
void digit_shift(int shift, s21_decimal *decimal);
int binary_search(s21_decimal dividend, s21_decimal divisor);
// int get_digits_count(s21_decimal decimal);
int has_remainder_for_first_digit(s21_decimal value_1, s21_decimal value_2,
                                  s21_decimal *result);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_greater(s21_decimal value1, s21_decimal value2);
int s21_is_greater_or_equal(s21_decimal value1, s21_decimal value2);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

void dodicum_add(dodicum value_1, dodicum value_2, dodicum *result);
void dodicum_mul(dodicum multiplicand, dodicum multiplicator, dodicum *product);
dodicum dodicum_mod(dodicum dividend, dodicum divisor, int *extra_exp);
void mul_dodicum_by_digit(int shift, dodicum multiplicand,
                          uint64_t multiplicator, dodicum *product);
// int get_exp(s21_decimal decimal);
int get_digits_count(dodicum dodic);
int is_dodicum_less(dodicum first, dodicum second);
void dodicum_sub(dodicum minuend, dodicum subtrahend, dodicum *quotient);
int is_dodicum_less_or_equal(dodicum first, dodicum second);
dodicum dodicum_div(dodicum dividend, dodicum divisor, dodicum *full_number,
                    int *extra_exp);
void dodicum_digit_shift(int shift, dodicum *result);
int dodicum_binary_search(dodicum dividend, dodicum divisor);
void dodic_print(dodicum dodic);
void set_exp(int exp, s21_decimal *decimal);
int dodicum_binary_search_v2(dodicum dividend, dodicum divisor);
int is_dodicum_equal(dodicum first, dodicum second);
bool is_divisible_by_10(dodicum number);
dodicum normalize(s21_decimal decimal);
int denormalize(dodicum dodic, s21_decimal *result);
int is_dodicum_greater(dodicum first, dodicum second);
dodicum get_dodicum_from_decimal(s21_decimal decimal);
int does_dodicum_fit_in_decimal(dodicum dodic);

s21_decimal decimal_abs(s21_decimal src);
int dodicum_get_last_digit(dodicum dodic);
int is_dodicum_odd(dodicum src);

#endif  // SRC_S21_DECIMAL_
