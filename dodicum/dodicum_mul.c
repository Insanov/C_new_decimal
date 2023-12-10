#include "../s21_decimal.h"

void dodicum_mul(dodicum multiplicand, dodicum multiplicator,
                 dodicum *product) {
  dodicum product_arr[7] = {0};

  for (int i = 0; i <= 6; i++)
    mul_dodicum_by_digit(i, multiplicand, multiplicator.bits[i],
                         product_arr + i);

  dodicum sum = {0};
  for (int i = 0; i <= 6; i++)
    dodicum_add(product_arr[i], sum, &sum);

  for (int i = 0; i <= 14; i++) {
    product->bits[i] = sum.bits[i];
  }
}
