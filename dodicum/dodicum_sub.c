#include "../s21_decimal.h"

void dodicum_sub(dodicum minuend, dodicum subtrahend, dodicum *quotient) {
  bool remainder = 0;
  for (int i = 0; i <= 12; i++) {
    int64_t difference =
        (int64_t)minuend.bits[i] - (int64_t)subtrahend.bits[i] - remainder;

    if (difference < 0) {
      remainder = 1;
      quotient->bits[i] = difference + ((int64_t)1 << 32);
    } else {
      remainder = 0;
      quotient->bits[i] = difference;
    }
  }
}
