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

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  //由于页故障，只对yield加4,其余后补。
  if(NO==-1 || NO==1)
    epc+=4;
  cpu.csrs.mcause = NO;
  cpu.csrs.mepc = epc;
  //cpu.csrs.mstatus=cpu.csrs.mstatus|((1<<11)+(1<<12));
  //二次审阅讲义，发现mstatus不要求，注释掉，防bug
  //uint32_t tmp=0;
  //tmp=(cpu.csrs.mstatus&(1<<3))>>3;
  //if(tmp==1)
    //cpu.csrs.mstatus=((cpu.csrs.mstatus&(~(1<<3)))&(~(1<<7)))|(tmp<<7);
  //else
    //cpu.csrs.mstatus=((cpu.csrs.mstatus&(~(1<<3)))&(~(1<<7)))&(~(1<<7));
  return cpu.csrs.mtvec;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
