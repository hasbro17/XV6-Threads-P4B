#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

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
int
sys_clone(void)
{ 
  void * stack;	  
  if(argptr(0, (void *)&stack, sizeof(void *)) < 0) //Is the size correct?
    return -1;
  return clone(stack);
}

int
sys_lock(void)
{

  void *argLock;
  if(argptr(0, (void*)&argLock, sizeof(void*)) < 0)
    return -1; 
  int *intLock=argLock;

  //To spin: comment out guardlocks and sleep
  acquire(&guardlock);
  while(xchg((unsigned int*)intLock, 1)==1)
  {
    sleep(intLock,&guardlock);
    //sleep(proc->pgdir,&guardlock);//FIXME sleep on your page directory as the channel? All threads share that
  }
  release(&guardlock);
    
  return proc->pid;
}
int
sys_unlock(void)
{
  void *argLock;
  if(argptr(0, (void*)&argLock, sizeof(void*)) < 0)
    return -1; 
  int *intLock=argLock;

  xchg((unsigned int*)intLock, 0); 

    wakeup(intLock);
 // wakeup(proc->pgdir);//FIXME wakeup all threads sleeping on your page directory. Seems fine

  return proc->pid;
}
int
sys_join(void)
{
  return join();
}
