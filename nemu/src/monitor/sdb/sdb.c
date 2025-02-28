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
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include "memory/paddr.h"//this is teh one I add

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char* args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  /* TODO: Add more commands */
  { "si", "Step into the next instruction, entering functions if necessary", cmd_si },
  { "info", "Show information about a specific topic", cmd_info },
  { "x", "Examine memory: x/N EXPR", cmd_x },
  { "p", "Print the value of an expression: p EXPR", cmd_p },
 // { "w", "Set a watchpoint: w EXPR", cmd_w },
 // { "d", "Delete a watchpoint by number: d N", cmd_d },
};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args){
//imitate the cmd_help func
         char *arg=strtok(NULL," ");
         int steps=1;
	 if (arg!=NULL){
                 sscanf(arg,"%d",&steps);
                 if(steps<1){
                         printf("Error!The integer you give should be larger than 0\n");
                         return 0;}}
         cpu_exec(steps);
         return 0;}

static int cmd_info(char *args){
	char *arg=strtok(NULL," ");
	if (arg==NULL)
		printf("Error.No input is catched.\n");
	else if (strcmp(arg,"r")==0)
		isa_reg_display();
	else if (strcmp("w",arg)==0)
		printf("To be realized\n");
	else
		printf("Error!No command like this.\n");
	return 0;
}	

static int cmd_x(char *args){//still some work to do
	char *N=strtok(NULL," ");
	char *expr=strtok(NULL," ");
	int n = 0;
	paddr_t addr=0;
	sscanf(N, "%d", &n);
  sscanf(expr,"%x", &addr);
	printf("%x : ",addr);
	for (int i=1;i<n;i++){
		printf("%x ",paddr_read(addr,4));
	        addr+=4;}
	printf("%x\n",paddr_read(addr,4));
	return 0;}

static int cmd_p(char* args){
    if(args == NULL){
        printf("No args\n");
        return 0;
    }
    //printf("args = %s\n", args);
    bool flag = false;
    word_t rec=expr(args, &flag);
    printf("%u\n",rec);
    return 0;
}


void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
