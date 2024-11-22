#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
//所有未定义情况我都没有处理，因为这样的情况太多太复杂
size_t strlen(const char *s) {
  size_t result=0;
  while(*s!='\0'){
    result++;
    s++;
  }
  return result;
}

char *strcpy(char *dst, const char *src) {
  char* tmp=dst;
  while(*src!='\0'){
    *tmp=*src;
    tmp++;
    src++;
  }
  *tmp='\0';
  return dst;

}

char *strncpy(char *dst, const char *src, size_t n) {
  char* tmp=dst;
  while(*src!='\0'&&n!=0){
    *tmp=*src;
    tmp++;
    src++;
    n--;
  }
  while(n!=0){
    *tmp='\0';
    n--;
    tmp++;
  }
  return dst;
}

char *strcat(char *dst, const char *src) {
  char* tmp=dst;
  while(*tmp!='\0')
    tmp++;
  while(*src!='\0'){
    *tmp=*src;
    tmp++;
    src++;
  }
  *tmp='\0';
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  unsigned char *tmp1=(unsigned char *)s1;
  unsigned char *tmp2=(unsigned char *)s2;
  while(!(*tmp1=='\0' && *tmp2=='\0')){
    if(*tmp1!=*tmp2)
      return (*tmp1>*tmp2)?1:-1;
    tmp1++;
    tmp2++;
  }
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  unsigned char *tmp1=(unsigned char *)s1;
  unsigned char *tmp2=(unsigned char *)s2;
  while(n!=0){
    if(*tmp1!=*tmp2)
      return (*tmp1>*tmp2)?1:-1;
    if(*tmp1=='\0'&&*tmp2=='\0')
      break;
    n--;
    tmp1++;
    tmp2++;

  }
  return 0;
}

void *memset(void *s, int c, size_t n) {
  unsigned char *tmp=(unsigned char *)s;
  unsigned char newc=(unsigned char) c;
  while(n!=0){
    *tmp=newc;
    n--;
    tmp++;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  unsigned char* tmp=(unsigned char *)dst;
  unsigned char tmparr[n];
  for(size_t i=0;i<n;i++){
    tmparr[i]=*((unsigned char *)src+i);
  }
  for(size_t i=0;i<n;i++)
  { *tmp=tmparr[i];
    tmp++;}
  return dst;}

void *memcpy(void *out, const void *in, size_t n) {
  unsigned char* tmp2=(unsigned char *)out;
  unsigned char* tmp1=(unsigned char *)in;
  while(n!=0){
    *tmp2=*tmp1;
    tmp2++;
    tmp1++;
    n--;

  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  unsigned char* tmp1=(unsigned char *)s1;
  unsigned char* tmp2=(unsigned char *)s2;
  while(n!=0){
    if(*tmp1!=*tmp2){
      return (*tmp1>*tmp2)?1:-1;
    }
    tmp1++;
    tmp2++;
    n--;
  }
  return 0;
}

#endif
