# include <stdio.h>
# include <stdlib.h>
 
int main() {
  // a simple static string
  char s[] = "abcdefghijklmnopqrstuvwxyz";
  for (size_t i = 0; i < sizeof (s); i++)
    printf("s[%2zu] = 0x%02x - '%c'\n", i, s[i], s[i]);
  return 0;
}
