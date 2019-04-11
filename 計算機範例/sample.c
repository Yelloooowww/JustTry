#include "ASA_Lib.h"
#include "math.h"
#define ID_7S00 1
#define ID_KB00 4
int num1,num2;
char key,operator,new_start,flag_num_or_op=3;

void unlock(){
  printf("UnLock and Restart\n" );
  key=0;
  num1=0;
  num2=0;
  operator=0;
  flag_num_or_op=3;
  new_start=0;
  display(0);
}

void Be_locked(){
  char output[4]={'-','-','-','-'};
  ASA_7S00_put(ID_7S00, 0, 4,  output);
  while (key!='C') {
    printf("Is Locked\n" );
    ASA_KB00_get(ID_KB00, 100, 1, &key);
  }
  unlock();
}

void display(int num){
  if(num>9999 || num<(-999)){
    Be_locked();
    return;
  }
  char output[4]={0,0,0,0};
  if(num>=0){
    if(num>=1000) output[0]='0'+num/1000;
    if(num>=100) output[1]='0'+(num%1000)/100;
    if(num>=10) output[2]='0'+(num%100)/10;
    output[3]='0'+(num%10);
  }else{
    int index=2;
    output[3]='0'+(-num)%10;
    if((-num) >=10){
      output[2]='0'+((-num)%100)/10;
      index=1;
    }
    if((-num) >=100){
      output[1]='0'+(-num)/100;
      index=0;
    }
    output[index]='-';
  }
  ASA_7S00_put(ID_7S00, 0, 4,  output);
}

int calculation(int n1,char op,int n2){
  if(op=='/' && n2==0) Be_locked();
  switch (op) {
    case '+': return n1+n2;
    case '-': return n1-n2;
    case '*': return n1*n2;
    case '/': return n1/n2;
  }
  return 0;
}

void keyboard(){
  ASA_KB00_get(ID_KB00, 100, 1, &key);
  switch (key) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if(flag_num_or_op==2){
          unlock();
          return;
        }
        else if(flag_num_or_op==1) num2=0;
        num2 = num2*10 + key - '0';
        flag_num_or_op=0;
        display(num2);
        break;

      case '=':
        num1=calculation(num1,operator,num2);
        flag_num_or_op=2;
        display(num1);
        break;

      case '+':
      case '-':
      case '*':
      case '/':
        if(new_start==0){
           num1=num2;
           new_start=1;
        }else if(flag_num_or_op){
          ;
        }else{
          num1=calculation(num1,operator,num2);
        }
        operator=key;
        flag_num_or_op=1;
        display(num1);
        break;

      case 'C':
        unlock();
        break;
  }
}


int main() {
    ASA_M128_set();
    ASA_KB00_set(ID_KB00, 200, 0xFF, 0, 1);
    ASA_KB00_set(ID_KB00, 201, 0xFF, 0, '+');
    ASA_KB00_set(ID_KB00, 202, 0xFF, 0, '-');
    ASA_KB00_set(ID_KB00, 203, 0xFF, 0, '*');
    ASA_KB00_set(ID_KB00, 204, 0xFF, 0, '/');
    ASA_KB00_set(ID_KB00, 205, 0xFF, 0, '=');
    ASA_KB00_set(ID_KB00, 209, 0xFF, 0, 'C');

    display(0);
    while (1) {
      printf("num1=%d,num2=%d,operator=%d,flag=%d\n",num1,num2,operator,flag_num_or_op );
      keyboard();
      char dontjump=key;
      while(dontjump==key){
        ASA_KB00_get(ID_KB00, 100, 1, &dontjump);
      }
    }
    return 0;
}
