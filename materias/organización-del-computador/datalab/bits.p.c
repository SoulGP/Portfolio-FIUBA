#line 145 "bits.c"
int bitOr(int x, int y) {
  return ~(~x & ~y);
#line 0 "<command-line>"
#include "/usr/include/stdc-predef.h"
#line 147 "bits.c"
}
#line 155
int bitParity(int x) {
  return 2;
}
#line 165
int bitNor(int x, int y) {
  return ~x & ~y;;
}
#line 175
int bitXor(int x, int y) {
  return 2;
}
#line 185
int evenBits(void) {
  int byte=  0x55;
  int word=(  byte << 8) | byte;
  return (word << 16) | word;
}
#line 199
int anyOddBit(int x) {
    return 2;
}
#line 211
int byteSwap(int x, int n, int m) {
  int posicion_n=  n << 3;
  int posicion_m=  m << 3;

  int byte_n=(  x >> posicion_n) & 0xFF;
  int byte_m=(  x >> posicion_m) & 0xFF;

  int swaped_n=(  byte_m << posicion_n);
  int swaped_m=(  byte_n << posicion_m);

  int mascara_n=  0xFF << posicion_n;
  int mascara_m=  0xFF << posicion_m;
  int mascara_total=  ~(mascara_n | mascara_m);

  int x_no_movidos=(  x & mascara_total);

  return x_no_movidos | swaped_n | swaped_m;
}
#line 238
int fitsBits(int x, int n) {
  return 2;
}
#line 247
int oddBits(void) {
  int byte=  0xAA;
  int word=(  byte << 8) | byte;
  return (word << 16) | word;
}
#line 260
int sign(int x) {
    return 2;
}
#line 272
int addOK(int x, int y) {
  int msb_x=  x >> 31;
  int msb_y=  y >> 31;
  int msb_resultado=(  x + y) >> 31;

  int desbordamiento_positivo=  ~msb_x & ~msb_y & msb_resultado;
  int desbordamiento_negativo=  msb_x & msb_y & ~msb_resultado;

  return !(desbordamiento_negativo | desbordamiento_positivo);
}
#line 292
int bitMask(int highbit, int lowbit) {
  return 2;
}
#line 302
int conditional(int x, int y, int z) {
    int mask_x=  !!x;
    mask_x = ~mask_x + 1;
    return (mask_x & y) |( ~mask_x & z);
}
#line 314
int bitCount(int x) {
  return 2;
}
#line 325
int bitMatch(int x, int y) {
  return (~x & ~y) |( x & y);
}
#line 337
int replaceByte(int x, int n, int c) {
  return 2;
}
#line 351
int satAdd(int x, int y) {
  int max=  ~(1 << 31);
  int min=  1 << 31;

  int suma=(  x + y);

  int msb_x=  x >> 31;
  int msb_y=  y >> 31;
  int msb_suma=  suma >> 31;

  int desbordamiento_positivo=  ~msb_x & ~msb_y & msb_suma;
  int desbordamiento_negativo=  msb_x & msb_y & ~msb_suma;
  return (desbordamiento_positivo & max) |( desbordamiento_negativo & min) |(
           ~(desbordamiento_positivo | desbordamiento_negativo) & suma);
}
#line 375
int satMul2(int x) {
  return 2;
}
#line 386
int isNonZero(int x) {
  return ((x |( ~x + 1)) >> 31) & 1;
}
#line 397
int rotateRight(int x, int n) {
  return 2;
}
#line 412
unsigned floatAbsVal(unsigned uf) {
  unsigned mask=  ~(1 << 31);
  int exponente=(  uf >> 23) & 0xFF;
  int mantisa=  uf & 0x7FFFFF;
  if (exponente == 0xFF && mantisa != 0) {
    return uf;
  }
  return uf & mask;
}
#line 432
int floatIsEqual(unsigned uf, unsigned ug) {
    return 2;
}
#line 446
unsigned floatNegate(unsigned uf) {
  unsigned mask=(  1 << 31);
  int exponente=(  uf >> 23) & 0xFF;
  int mantisa=  uf & 0x7FFFFF;
  if (exponente == 0xFF && mantisa != 0) {
    return uf;
  }
 return uf ^ mask;
}
#line 466
int floatIsLess(unsigned uf, unsigned ug) {
    return 2;
}
#line 481
int floatFloat2Int(unsigned uf) {
    int signo=(  uf >> 31) & 1;
    int exponente=((  uf >> 23) & 0xFF) - 127;
    int mantisa=(  uf & 0x7FFFFF) |( 1 << 23);

    if (exponente >= 31) {
      return 0x80000000u;
    }
    else if (exponente < 0) {
      return 0;
    }

    if (exponente > 23) {
      mantisa = mantisa <<( exponente - 23);
    }
    else {
      mantisa = mantisa >>( 23 - exponente);
    }

    if (signo) {
      mantisa = -mantisa;
    }

    return mantisa;
}
#line 519
unsigned floatPower2(int x) {
    return 2;
}
