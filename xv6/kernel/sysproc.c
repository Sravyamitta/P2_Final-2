#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

/* The following code is added/modified by Kavimayil(kxp230053) and Sravya(smm230008)
 *Included pstat and spinlock header files   
 */
#include "pstat.h"
#include "spinlock.h"
/* End of code added/modified */

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/* The following code is added/modified by Kavimayil(kxp230053) and Sravya(smm230008)
** Added system call function definition for set tickets and getpinfo
*/
int
sys_settickets(void)
{
  int reqtickets;

  //gets the first argument from the user space and stores it in "reqtickets"
  if(argint(0,&reqtickets) <0)
    return -1;
  
 //tickets are set to current process as per its passed value
 if(reqtickets > 0)
 {
   proc->numoftickets=reqtickets;
   return 0;
 }
 //return -1 for negative values of tickets
 else{ 
   return -1;
 }
}

//system call to get process statistics
int
sys_getpinfo(void)
{
  
  struct pstat* p = NULL;

  //gets the first argument from user space
  if(argptr(0, (void *)&p, sizeof(struct pstat)) <0)
    return -1;
  
  if (p == NULL)
    return -1;

  getpinfo_wrapper(p);

  return 0;
}
/* End of code added/modified */
