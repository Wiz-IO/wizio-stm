
.syntax unified
.thumb

#ifdef CM4F
.cpu cortex-m4
.arch armv7e-m
.fpu fpv4-sp-d16
#else
.cpu cortex-m0
.fpu softvfp
#endif


.global PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
  cpsid i
  mrs r0, psp
  isb

#ifdef CM4F  
  stmdb  r0!, {r4-r11, r14}
  vstmdb r0!, {s16-s31} 
#else
	subs	r0, #16
	stmia	r0!,{r4-r7}
	mov	r4, r8
	mov	r5, r9
	mov	r6, r10
	mov	r7, r11
	subs	r0, #32
	stmia	r0!,{r4-r7}
	subs	r0, #16
#endif  

  ldr r2, =os_curr_task /* Save current task SP */
  ldr r1, [r2]
  str r0, [r1]

  bl os_switch
  ldr r0, [r0]

#ifdef CM4F
  vldmia  r0!, {s16-s31}
  ldmia   r0!, {r4-r11, r14}  
#else 
	ldmia	r0!,{r4-r7}
	mov	r8, r4
	mov	r9, r5
	mov	r10, r6
	mov	r11, r7
	ldmia	r0!,{r4-r7}  
#endif   
     
  msr psp, r0
  isb

  ldr r0, =0xFFFFFFFD /* EXC_RETURN - Thread mode with PSP */
  cpsie i
  bx r0


.global os_load
.type os_load, %function
os_load: // private  void os_load();
  BX LR