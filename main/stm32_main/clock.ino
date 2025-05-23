
// void Readclock(){
//   DateTime now = rtc.now();
//   // تبدیل تاریخ میلادی به شمسی
//   int jy, jm, jd;
//   gregorianToJalali(now.year(), now.month(), now.day(), &jy, &jm, &jd);

// }

//SET 2025 05 18 14 20 00

void gregorianToJalali(int gy, int gm, int gd, int *jy, int *jm, int *jd) {
  int g_d_m[] = {0,31,59,90,120,151,181,212,243,273,304,334};
  int jy2 = (gy <= 1600) ? 0 : 979;
  gy -= (gy <= 1600) ? 621 : 1600;
  int gy2 = (gm > 2) ? gy + 1 : gy;
  long days = (365 * gy) + ((gy2 + 3) / 4) - ((gy2 + 99) / 100) + ((gy2 + 399) / 400) - 80 + gd + g_d_m[gm - 1];
  *jy = jy2 + 33 * (days / 12053);
  days %= 12053;
  *jy += 4 * (days / 1461);
  days %= 1461;
  if (days > 365) {
    *jy += (days - 1) / 365;
    days = (days - 1) % 365;
  }
  int jm2, jd2;
  if (days < 186) {
    jm2 = 1 + days / 31;
    jd2 = 1 + (days % 31);
  } else {
    jm2 = 7 + (days - 186) / 30;
    jd2 = 1 + ((days - 186) % 30);
  }
  *jm = jm2;
  *jd = jd2;
}


void jalaliToGregorian(int jy, int jm, int jd, int *gy, int *gm, int *gd) {
  int gy_temp;
  if (jy > 979) {
    gy_temp = 1600;
    jy -= 979;
  } else {
    gy_temp = 621;
  }

  int days = (365 * jy) + ((jy / 33) * 8) + (((jy % 33) + 3) / 4);
  for (int i = 0; i < jm - 1; i++) {
    days += (i < 6) ? 31 : 30;
  }
  days += jd - 1;

  gy_temp += 400 * (days / 146097);
  days %= 146097;

  if (days > 36524) {
    gy_temp += 100 * (--days / 36524);
    days %= 36524;

    if (days >= 365) days++;
  }

  gy_temp += 4 * (days / 1461);
  days %= 1461;

  if (days > 365) {
    gy_temp += (days - 1) / 365;
    days = (days - 1) % 365;
  }

  static const int g_days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};

  int i;
  for (i = 0; i < 12; i++) {
    int dim = g_days_in_month[i];
    if (i == 1 && ((gy_temp % 4 == 0 && gy_temp % 100 != 0) || (gy_temp % 400 == 0))) {
      dim++; // Leap year
    }
    if (days < dim) break;
    days -= dim;
  }

  *gy = gy_temp;
  *gm = i + 1;
  *gd = days + 1;
}
