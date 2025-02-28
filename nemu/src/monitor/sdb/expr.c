/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ=1,TK_NUM=2,TK_HEX=3,TK_REG=4,TK_NEG=5  

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"-",'-'},
  {"\\*",'*'},
  {"/",'/'},
  {"\\(",'('},
  {"\\)",')'},
  {"0[xX][0-9a-fA-F]+",TK_HEX},
  {"\\b[0-9]+\\b",TK_NUM},
  {"\\$(\\$0|ra|[sgt]p|t[0-6]|a[0-7]|s([0-9]|1[0-1]))", TK_REG},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[512] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
	Token tknsto;
  switch (rules[i].token_type) {
	case 256:
		break;
	case 2:case 3: 
		strncpy(tknsto.str,substr_start,substr_len);
		tknsto.str[substr_len]='\0';
    tknsto.type=rules[i].token_type;
		tokens[nr_token]=tknsto;
		nr_token++;
    break;
  case 4:
    char *newstart=substr_start+1;
    strncpy(tknsto.str,newstart,substr_len-1);
		tknsto.str[substr_len-1]='\0';
  default: 
		tknsto.type=rules[i].token_type;
		tokens[nr_token]=tknsto;
		nr_token++;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

int checkparentness(int p,int q,bool jud){
	int lftbra=0;
	for(int i=p;i<=q;i++){
		if(tokens[i].type=='(')
			lftbra++;
		else if(tokens[i].type==')'){
			lftbra--;
		if(lftbra<0)
			return -1;}}
  if (lftbra>0)
    return -1;
  if (jud==false || tokens[p].type!='(' || tokens[q].type!=')')
    return 0;
  else 
    return checkparentness(p+1,q-1,false)+1;
}

word_t eval(int p,int q){
	if (p > q) {
    /* Bad expression */
	return false;
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    bool flag=false;
    if (tokens[p].type==2)
		  return (word_t) strtol(tokens[p].str,NULL,10);
    else if (tokens[p].type==3)
      return (word_t) strtol(tokens[p].str,NULL,16);
    else if (tokens[p].type==4)
      return isa_reg_str2val(tokens[p].str,&flag);
  }
  int rec=checkparentness(p, q,true);
  if (rec==-1) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return false;
     
  }
  else if(rec==1)
    return eval(p + 1, q - 1);
  int tmpop[512];
  int now=0;
  int leftbra=0;
  int op=0;
  int op_type=0;
  word_t val1=0;
  word_t val2=0;
  for (int j=p;j<=q;j++)
    {if(tokens[j].type=='-'&&(j==p||tokens[j-1].type=='+'||tokens[j-1].type=='-'||
    tokens[j-1].type=='*'||tokens[j-1].type=='/'||tokens[j-1].type=='('||tokens[j-1].type==5)){
      tokens[j].type=5;
    }
    if ((tokens[j].type=='+'||tokens[j].type=='-'||tokens[j].type=='*'||tokens[j].type=='/')&&leftbra==0)
	  {tmpop[now]=j;
	  now++;}
    else if (tokens[j].type=='(')
	    leftbra++;
    else if (tokens[j].type==')')
	    leftbra--;}
  if (now==0&&tokens[p].type==5)
    return (word_t)(-eval(p+1,q));
  for (int j=now-1;j>=0;j--){
	  if (tokens[tmpop[j]].type=='+'||tokens[tmpop[j]].type=='-')
		  {op=tmpop[j];
      break;}
	  if (j==0)
		  op=tmpop[now-1];}
    val1 = eval(p, op - 1);
    val2= eval(op + 1, q);
    op_type=tokens[op].type;
    switch (op_type) {
      case 43: return val1 + val2;
      case 45: return val1-val2;
      case 42: return val1*val2;
      case 47: return val1/val2;
      default: assert(0);
    }
  
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  return eval(0,nr_token-1);//changed
  /* TODO: Insert codes to evaluate the expression. */
  

  return 0;
}
