global context_switch
context_switch:
     cli;
     mov ecx, [esp+4]   ; EIP
     mov ebp, [esp+8]  ; EBP
     mov esp, [esp+12]  ; ESP
     mov eax, 0x12345   ; magic number to detect a task switch
     sti;
     jmp ecx

global read_eip
read_eip:
    pop eax          ;Get the return address from the stack
    jmp eax         ;Jump to the return address