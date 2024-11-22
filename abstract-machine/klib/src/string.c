#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  if(dst==NULL || src==NULL)
    return dst;
  char* result=dst;
  while(*src!='\0'){
    *dst=*src;
    dst++;
    src++;
  }
  *dst='\0';
  return result;

}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  char* result=dst;
  while(*dst!='\0')
    dst++;
  while(*src!='\0'){
    *dst=*src;
    dst++;
    src++;
  }
  *dst='\0';
  return result;
}

int strcmp(const char *s1, const char *s2) {
  char *tmp1=(char *)s1;
  char *tmp2=(char *)s2;
    while(*tmp1!='\0' && *tmp2!='\0'){
    if(*tmp1!=*tmp2){
      if (*tmp1>*tmp2)
        return 1;
      else
        return -1;
    }
    tmp1++;
    tmp2++;
  }
  if(*tmp1=='\0'&&*tmp2=='\0')
    return 0;
  if(*tmp1=='\0')
    return -1;
  else
    return 1;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  char *sglcha=(char *)s;
  while(n!=0){
    *sglcha=c;
    n--;
    sglcha++;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  char* str1=(char *)s1;
  char* str2=(char *)s2;
  while(n!=0){
    if(*str1!=*str2){
      if(*str1>*str2)
        return 1;
      else
        return -1;
    }
    str1++;
    str2++;
    n--;
  }
  return 0;
  panic("Not implemented");
}

#endif
