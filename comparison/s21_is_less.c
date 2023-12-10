#include "../s21_decimal.h"

int s21_is_less(s21_decimal first, s21_decimal second) {
  bool result = 0;

  if (get_sign(first) == 1 && get_sign(second) == 0) {
    result = 1;
  } else if (get_sign(first) == 0 && get_sign(second) == 1) {
    // Does nothing, but prevents going into else and makes if's simpler
    result = 0;
  } else if (get_sign(first) == 1) {
    for (int i = 2, finished = 0; i >= 0 && !finished; i--) {
      if (first.bits[i] > second.bits[i])
        finished = result = 1;
    }
  } else {
    for (int i = 2, finished = 0; i >= 0 && !finished; i--) {
      if (first.bits[i] < second.bits[i])
        finished = result = 1;
    }
  }

  return result;
}
