#ifndef TETRISMACROS
#define TETRISMACROS

#define HIWORD(n) *((unsigned short *)&n + 1)
#define LOWORD(n) *((unsigned short *)&n)

#endif