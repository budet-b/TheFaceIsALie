# include <stdio.h>
# include <stdlib.h>
 
size_t mystrlen(char *s) {
   size_t len = 0;
    while(s[len] != '\0')
        len++;
    return len;
}
 
char *mystrncpy(char *dst, char *src, size_t len) {
    if(mystrlen(dst) < len)
        return dst;
    size_t srclen = mystrlen(src);
    for(unsigned long i = 0;i<len;i++) {
        if (srclen > i)
            dst[i] = src[i];
        else
            dst[i] = '\0';
    }
    return dst;
}
 
void print_str_as_array(char *s, size_t len) {
  for (size_t i = 0; i < len; i++)
    printf("0x%02x ", s[i]);
  printf("\n");
}
 
int main() {
  char src[] = "abc";
  char *dst = malloc(2 * sizeof (src) * sizeof (char));
  // Fill dst with 0x7f
  for (char *cur = dst; cur < dst + 2 * sizeof (src); cur++)
    *cur = 0x7f;
  // Print dst and src
  printf("src = ");
  print_str_as_array(src, sizeof (src));
  printf("dst = ");
  print_str_as_array(dst, 2 * sizeof (src));
 
  // copy exactly the length of src
  mystrncpy(dst, src, mystrlen(src));
  printf("\ndst = ");
  print_str_as_array(dst, 2 * sizeof (src));
    // Fill dst with 0x7f
  for (char *cur = dst; cur < dst + 2 * sizeof (src); cur++)
    *cur = 0x7f;
 
  // copy the length of src + 1
  mystrncpy(dst, src, mystrlen(src) + 1);
  printf("\ndst = ");
  print_str_as_array(dst, 2 * sizeof (src));
    // Fill dst with 0x7f
  for (char *cur = dst; cur < dst + 2 * sizeof (src); cur++)
    *cur = 0x7f;
 
  // copy the size of dst
  mystrncpy(dst, src, 2 * sizeof (src));
  printf("\ndst = ");
  print_str_as_array(dst, 2 * sizeof (src));
    // Fill dst with 0x7f
  for (char *cur = dst; cur < dst + 2 * sizeof (src); cur++)
    *cur = 0x7f;
 
  free(dst);
  return 0;
}
