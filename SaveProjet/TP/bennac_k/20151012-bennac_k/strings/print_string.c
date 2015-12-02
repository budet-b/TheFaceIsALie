# include <stdio.h>
# include <stdlib.h>
 
int main() {
  // a simple static string
  char a[] = "abcdefghijklmnopqrstuvwxyz";
  for (size_t i = 0; i < sizeof (a); i++)
      printf("s[%2zu] = 0x%02x - '%c'\n", i, a[i], a[i]);
  return 0;
}
