#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

/*
 *  Returns the number of input items assigned. The value EOF is returned
 *  if an input failure occurs before any conversion such as an end-of-file
 *  occurs. If an error or end-of-file occurs after conversion has begun,
 *  the number of conversions which were successfully completed is returned.
 */

// XXX Only %d, %s, %c, whitespace and character matching, no flags except '*'

int vfscanf(FILE *stream, const char *format, va_list arg)
{
  int r = 0;                    // Number of conversions
  int c = getc(stream);         // Look-ahead
  int flags;
  enum { star=1 };
  int sign, n;
  char *s;

  for (; c>=0 && *format; format++) {
    switch (*format) {
    case ' ':
      while (isspace(c))
        c = getc(stream);
      continue;
    case '%':
      flags = 0;
      format++;
      if (*format == '*') {     // Flag to suppress assignment
        flags |= star;
        format++;
      }
      switch (*format) {
      case 'd':                 // Decimal %d
        while (isspace(c))
          c = getc(stream);
        if (c == '-' || c == '+') {
          sign = c;
          c = getc(stream);
        } else
          sign = 0;
        if (isdigit(c)) {
          n = 0;
          do {
            n = n * 10 + c - '0';
            c = getc(stream);
          } while (isdigit(c));
          if ((flags & star) == 0) {
            *va_arg(arg, int*) = (sign == '-') ? -n : n;
            r++;
          }
          continue;
        }
        break;
      case 's':                 // String %s
        s = va_arg(arg, char*);
        while (isspace(c))
          c = getc(stream);
        while (c>=0 && !isspace(c)) {
          if ((flags & star) == 0)
            *s++ = c;
          c = getc(stream);
        }
        if ((flags & star) == 0) {
          *s = 0;
          r++;
        }
        continue;
      case 'c':                 // Character %c
        s = va_arg(arg, char*);
        if ((flags & star) == 0) {
          *s = c;
          r++;
        }
        c = getc(stream);
        continue;
      default:                  // Also handles %%
        break;
      }
      // !!! Fall through !!!
    default:
      if (c == *format) {       // Matching character
        c = getc(stream);
        continue;
      }
      break;
    }
    break; // Unable to continue processing
  }

  if (ungetc(c,stream)<0 && r==0)// Push back the look-ahead
    return EOF;                 // Error before first conversion
  else
    return r;                   // Number of conversions
}
