#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  int result=0;
  char outbuf[512];
  va_list argslst;
  va_start(argslst, fmt);
  result = vsprintf(outbuf, fmt, argslst);
  va_end(argslst);
  putstr(outbuf);
  return result;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char* tmp=out;
  char* sreceive=NULL;
  int base=10;
  int num=0;
  while(*fmt!='\0'){
    if(*fmt!='%'){
      *tmp=*fmt;
      tmp++;
    }
    else{
      fmt++;
      if(*fmt=='d'){
        num=va_arg(ap,int);
        int numsto[128];
        if(num<0){
          num=-num;
          *tmp='-';
          tmp++;
        }
        int count=0;
        do{
          numsto[count]=num%base;
          count++;
          num=num/10;
        }while(num!=0);
        while(count!=0){
          *tmp=(char)(numsto[count-1]+'0');
          tmp++;
          count--;
        }
      }
      else if(*fmt=='s'){
        sreceive=va_arg(ap,char*);
        while(*sreceive!='\0'){
          *tmp=*sreceive;
          tmp++;
          sreceive++;
        }
      }
      else if(*fmt=='c'){
        //只是因为测试文件里有'c'选项，为了检验效果，所以针对这个加上
        *tmp=(unsigned char)va_arg(ap,int);
        tmp++;
      }
      else{
        *tmp='%';
        tmp++;
        *tmp=*fmt;
        tmp++;
      }
    }
    fmt++;
  }
  *tmp='\0';
  return tmp-out;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list arglst;
  int receive=0;
  va_start(arglst,fmt);
  receive=vsprintf(out,fmt,arglst);
  va_end(arglst);
  return receive;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
