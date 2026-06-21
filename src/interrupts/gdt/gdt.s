.section .data
gdtr:
    .short 0
    .long 0

.section .text
.global setGdt
setGdt:
    movw 4(%esp), %ax 		/* c calls setGdt and then parameters are pushed onto stack rather than reghisters,
							   each slot in ram occupies 4 bytes but we want just 2 byte limit and movw(move word(16 bit = 1 byte)) is moved onto ax */
    movw %ax, gdtr			/* ax is moved into gdtr which is then loaded into cpu */


	
    movl 8(%esp), %eax		/* moves next aergument into eax (+8 cuz each cell is 4 bytes so after limit was stored in cell 1 2 bytes remain empty and
							   parameter is stored in eax(32 bit) register */
    movl %eax, gdtr+2		/* gdtr is 6 bytes so first 2 bytes weer loaded and now next 4 bytes are loaded taking 2 offset from gdtr base */
    lgdt gdtr				/* load gdtr into cpu */
    ret


	/* ive to flush old config properties as cpu stores them,  to change cs ive to perfrom a long jump which changes both eip (instruction pointer)
	   and cs where we jumped */
.global reloadSegments
reloadSegments:

    ljmp $0x08, $.reload_CS  /* new value into cs register is 0x08 and 2nd arg loads address fo next instruction into eip and it starts executing from that instruction 							   here that is movw ($0x10, %ax)*/
.reload_CS:
    movw $0x10, %ax

	/* reloading all rehgisters*/
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret
