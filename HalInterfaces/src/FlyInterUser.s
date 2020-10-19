		;使用汇编的时候，可以使用CPS(修改处理器状态)指令修改PRIMASK寄存器的数值
		THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.CPU_Disable_Irq||, CODE, READONLY, ALIGN=4
		AREA ||i.CPU_Enable_Irq||, CODE, READONLY, ALIGN=4

CPU_Disable_Irq PROC
    cpsid i 				;Disable interrupt

    ENDP

CPU_Enable_Irq PROC
    cpsie i 				;Enable interrupt

    ENDP
			
	EXPORT CPU_Disable_Irq [CODE]
	EXPORT CPU_Enable_Irq [CODE]
	END