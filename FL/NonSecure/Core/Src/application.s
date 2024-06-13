	.cpu cortex-m33
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.global	input
	.section	.data.input,"aw",%progbits
	.align	2
	.type	input, %object
	.size	input, 4
input:
	.byte	117
	.byte	0
	.byte	16
	.byte	58
	.global	data
	.section	.bss.data,"aw",%nobits
	.align	2
	.type	data, %object
	.size	data, 16
data:
	.space	16
	.global	temp_data
	.section	.bss.temp_data,"aw",%nobits
	.align	2
	.type	temp_data, %object
	.size	temp_data, 5
temp_data:
	.space	5
	.global	valid_reading
	.section	.bss.valid_reading,"aw",%nobits
	.type	valid_reading, %object
	.size	valid_reading, 1
valid_reading:
	.space	1
	.comm	ult_runs,4,4
	.comm	temp_runs,4,4
	.comm	hum_runs,4,4
	.comm	seq_runs,4,4
	.comm	runs,4,4
	.section	.text.delay,"ax",%progbits
	.align	1
	.global	delay
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	delay, %function
delay:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7, lr}
	sub	sp, sp, #20
	add	r7, sp, #0
	str	r0, [r7, #4]
	adr	r10, .L3
	ldr	r11,  [r7, #4]
	bl	SECURE_log_loop_cond
	movs	r3, #0
	str	r3, [r7, #12]
	b	.L2
.L3:
	ldr	r3, [r7, #12]
	adds	r3, r3, #1
	str	r3, [r7, #12]
.L2:
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #4]
	cmp	r2, r3
	bcc	.L3
	bl	SECURE_log_cond_br_not_taken
	adds	r7, r7, #20
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
	.size	delay, .-delay
	.section	.text.read_data,"ax",%progbits
	.align	1
	.global	read_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	read_data, %function
read_data:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #8
	add	r7, sp, #0
	movs	r3, #0
	strb	r3, [r7, #7]
	movs	r3, #0
	strh	r3, [r7, #4]	@ movhi
	ldr	r3, .L11
	mov	r2, #256
	str	r2, [r3, #40]
	movs	r0, #250
	bl	delay
	ldr	r3, .L11
	mov	r2, #256
	str	r2, [r3, #24]
	movs	r0, #20
	bl	delay
	ldr	r3, .L11
	mov	r2, #256
	str	r2, [r3, #40]
	movs	r0, #40
	bl	delay
	movs	r3, #0
	strh	r3, [r7, #2]	@ movhi
	b	.L5
.L8:
	bl	SECURE_log_cond_br_taken
	ldr	r3, .L11
	ldr	r3, [r3, #16]
	lsrs	r3, r3, #8
	and	r3, r3, #1
	uxtb	r2, r3
	ldrb	r3, [r7, #7]
	add	r3, r3, r2
	strb	r3, [r7, #7]
	ldrh	r3, [r7, #2]
	cmp	r3, #3
	bls	.L66
	bl	SECURE_log_cond_br_not_taken
	ldrh	r3, [r7, #2]
	and	r3, r3, #1
	cmp	r3, #0
	bne	.L66
	bl	SECURE_log_cond_br_not_taken
	ldrh	r3, [r7, #4]
	lsrs	r3, r3, #3
	uxth	r3, r3
	mov	r2, r3
	ldrh	r3, [r7, #4]
	lsrs	r3, r3, #3
	uxth	r3, r3
	mov	r1, r3
	ldr	r3, .L11+4
	ldrb	r3, [r3, r1]	@ zero_extendqisi2
	lsls	r3, r3, #1
	uxtb	r1, r3
	ldr	r3, .L11+4
	strb	r1, [r3, r2]
	ldrb	r3, [r7, #7]	@ zero_extendqisi2
	cmp	r3, #6
	bls	.L65
	bl	SECURE_log_cond_br_not_taken
	ldrh	r3, [r7, #4]
	lsrs	r3, r3, #3
	uxth	r3, r3
	mov	r1, r3
	ldrh	r3, [r7, #4]
	lsrs	r3, r3, #3
	uxth	r3, r3
	mov	r2, r3
	ldr	r3, .L11+8
	ldr	r3, [r3, r2, lsl #2]
	orr	r3, r3, #1
	ldr	r2, .L11+8
	str	r3, [r2, r1, lsl #2]
	b	.L7
.L65:
	bl	SECURE_log_cond_br_taken
.L7:
	ldrh	r3, [r7, #4]
	adds	r3, r3, #1
	strh	r3, [r7, #4]	@ movhi
	b	.L6
.L66:
	bl	SECURE_log_cond_br_taken
.L6:
	ldrh	r3, [r7, #2]
	adds	r3, r3, #1
	strh	r3, [r7, #2]	@ movhi
.L5:
	ldrh	r3, [r7, #2]
	cmp	r3, #82
	bls	.L8
	bl	SECURE_log_cond_br_not_taken
	ldrh	r3, [r7, #4]
	cmp	r3, #39
	bls	.L67
	bl	SECURE_log_cond_br_not_taken
	ldr	r3, .L11+4
	ldrb	r3, [r3, #4]	@ zero_extendqisi2
	mov	r1, r3
	ldr	r3, .L11+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	mov	r2, r3
	ldr	r3, .L11+4
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	add	r3, r3, r2
	ldr	r2, .L11+4
	ldrb	r2, [r2, #2]	@ zero_extendqisi2
	add	r3, r3, r2
	ldr	r2, .L11+4
	ldrb	r2, [r2, #3]	@ zero_extendqisi2
	add	r3, r3, r2
	uxtb	r3, r3
	cmp	r1, r3
	bne	.L67
	bl	SECURE_log_cond_br_not_taken
	ldr	r3, .L11+12
	movs	r2, #1
	strb	r2, [r3]
	b	.L10
.L67:
	bl	SECURE_log_cond_br_taken
.L9:
	ldr	r3, .L11+12
	movs	r2, #0
	strb	r2, [r3]
.L10:
	adds	r7, r7, #8
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
.L12:
	.align	2
.L11:
	.word	1107427328
	.word	temp_data
	.word	data
	.word	valid_reading
	.size	read_data, .-read_data
	.section	.text.get_temperature,"ax",%progbits
	.align	1
	.global	get_temperature
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	get_temperature, %function
get_temperature:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #8
	add	r7, sp, #0
	bl	read_data
	ldr	r3, .L15
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	strh	r3, [r7, #6]	@ movhi
	ldr	r3, .L15
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	lsls	r3, r3, #8
	sxth	r2, r3
	ldrsh	r3, [r7, #6]
	orrs	r3, r3, r2
	sxth	r3, r3
	strh	r3, [r7, #6]	@ movhi
	ldrh	r3, [r7, #6]
	mov	r0, r3
	adds	r7, r7, #8
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
.L16:
	.align	2
.L15:
	.word	temp_data
	.size	get_temperature, .-get_temperature
	.section	.text.get_humidity,"ax",%progbits
	.align	1
	.global	get_humidity
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	get_humidity, %function
get_humidity:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #8
	add	r7, sp, #0
	bl	read_data
	ldr	r3, .L19
	ldrb	r3, [r3]	@ zero_extendqisi2
	strh	r3, [r7, #6]	@ movhi
	ldr	r3, .L19
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	lsls	r3, r3, #8
	sxth	r2, r3
	ldrsh	r3, [r7, #6]
	orrs	r3, r3, r2
	sxth	r3, r3
	strh	r3, [r7, #6]	@ movhi
	ldrh	r3, [r7, #6]
	mov	r0, r3
	adds	r7, r7, #8
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
.L20:
	.align	2
.L19:
	.word	temp_data
	.size	get_humidity, .-get_humidity
	.section	.text.pulseIn,"ax",%progbits
	.align	1
	.global	pulseIn
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	pulseIn, %function
pulseIn:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7, lr}
	sub	sp, sp, #12
	add	r7, sp, #0
	movs	r3, #0
	str	r3, [r7, #4]
	adr	r10, .L23
	mov	r11, #1000
	bl	SECURE_log_loop_cond
	movs	r3, #0
	str	r3, [r7]
	b	.L22
.L23:
	ldr	r3, .L25
	ldr	r3, [r3, #16]
	lsrs	r3, r3, #8
	and	r3, r3, #2
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r3, [r7]
	adds	r3, r3, #1
	str	r3, [r7]
.L22:
	ldr	r3, [r7]
	cmp	r3, #1000
	blt	.L23
	bl	SECURE_log_cond_br_not_taken
	ldr	r3, [r7, #4]
	mov	r0, r3
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
.L26:
	.align	2
.L25:
	.word	1107427328
	.size	pulseIn, .-pulseIn
	.section	.text.getUltrasonicReading,"ax",%progbits
	.align	1
	.global	getUltrasonicReading
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	getUltrasonicReading, %function
getUltrasonicReading:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #8
	add	r7, sp, #0
	ldr	r3, .L29
	mov	r2, #512
	str	r2, [r3, #24]
	movs	r0, #2
	bl	delay
	ldr	r3, .L29
	mov	r2, #512
	str	r2, [r3, #40]
	movs	r0, #5
	bl	delay
	ldr	r3, .L29
	mov	r2, #512
	str	r2, [r3, #24]
	bl	pulseIn
	str	r0, [r7, #4]
	ldr	r3, [r7, #4]
	mov	r0, r3
	adds	r7, r7, #8
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
.L30:
	.align	2
.L29:
	.word	1107427328
	.size	getUltrasonicReading, .-getUltrasonicReading
	.section	.text.run_ultrasonic,"ax",%progbits
	.align	1
	.global	run_ultrasonic
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	run_ultrasonic, %function
run_ultrasonic:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #16
	add	r7, sp, #0
	str	r0, [r7, #4]
	movs	r3, #0
	str	r3, [r7, #8]
	movs	r3, #0
	str	r3, [r7, #12]
	b	.L32
.L33:
	bl	SECURE_log_cond_br_taken
	bl	getUltrasonicReading
	mov	r2, r0
	ldr	r3, [r7, #4]
	sdiv	r3, r2, r3
	ldr	r2, [r7, #8]
	add	r3, r3, r2
	str	r3, [r7, #8]
	ldr	r3, [r7, #12]
	adds	r3, r3, #1
	str	r3, [r7, #12]
.L32:
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #4]
	cmp	r2, r3
	blt	.L33
	bl	SECURE_log_cond_br_not_taken
	ldr	r3, [r7, #8]
	mov	r0, r3
	adds	r7, r7, #16
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
	.size	run_ultrasonic, .-run_ultrasonic
	.section	.text.run_temperature,"ax",%progbits
	.align	1
	.global	run_temperature
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	run_temperature, %function
run_temperature:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #16
	add	r7, sp, #0
	str	r0, [r7, #4]
	movs	r3, #0
	str	r3, [r7, #8]
	movs	r3, #0
	str	r3, [r7, #12]
	b	.L36
.L37:
	bl	SECURE_log_cond_br_taken
	bl	get_temperature
	mov	r3, r0
	mov	r2, r3
	ldr	r3, [r7, #4]
	sdiv	r3, r2, r3
	ldr	r2, [r7, #8]
	add	r3, r3, r2
	str	r3, [r7, #8]
	ldr	r3, [r7, #12]
	adds	r3, r3, #1
	str	r3, [r7, #12]
.L36:
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #4]
	cmp	r2, r3
	blt	.L37
	bl	SECURE_log_cond_br_not_taken
	ldr	r3, [r7, #8]
	mov	r0, r3
	adds	r7, r7, #16
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
	.size	run_temperature, .-run_temperature
	.section	.text.run_humidity,"ax",%progbits
	.align	1
	.global	run_humidity
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	run_humidity, %function
run_humidity:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #16
	add	r7, sp, #0
	str	r0, [r7, #4]
	movs	r3, #0
	str	r3, [r7, #8]
	movs	r3, #0
	str	r3, [r7, #12]
	b	.L40
.L41:
	bl	SECURE_log_cond_br_taken
	bl	get_humidity
	mov	r3, r0
	mov	r2, r3
	ldr	r3, [r7, #4]
	sdiv	r3, r2, r3
	ldr	r2, [r7, #8]
	add	r3, r3, r2
	str	r3, [r7, #8]
	ldr	r3, [r7, #12]
	adds	r3, r3, #1
	str	r3, [r7, #12]
.L40:
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #4]
	cmp	r2, r3
	blt	.L41
	bl	SECURE_log_cond_br_not_taken
	ldr	r3, [r7, #8]
	adr	r10, .L45
	mov	r11, #58
	bl	SECURE_log_loop_cond
	mov	r0, r3
	adds	r7, r7, #16
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
	.size	run_humidity, .-run_humidity
	.comm	send_data,4,4
	.section	.text.read_command,"ax",%progbits
	.align	1
	.global	read_command
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	read_command, %function
read_command:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7, lr}
	sub	sp, sp, #12
	add	r7, sp, #0
	str	r0, [r7, #4]
	str	r1, [r7]
	b	.L44
.L45:
	ldr	r3, [r7]
	ldrb	r2, [r3]	@ zero_extendqisi2
	ldr	r3, [r7, #4]
	strb	r2, [r3]
	ldr	r3, [r7, #4]
	adds	r3, r3, #1
	str	r3, [r7, #4]
	ldr	r3, [r7]
	adds	r3, r3, #1
	str	r3, [r7]
.L44:
	ldr	r3, [r7]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #58
	bne	.L45
	bl	SECURE_log_cond_br_not_taken
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
	.size	read_command, .-read_command
	.comm	cmd,1,1
	.section	.rodata
	.align	2
.LC0:
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.byte	97
	.section	.text.process_command,"ax",%progbits
	.align	1
	.global	process_command
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	process_command, %function
process_command:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r7, lr}
	sub	sp, sp, #20
	add	r7, sp, #0
	ldr	r3, .L53
	mov	r4, r7
	ldm	r3, {r0, r1, r2, r3}
	stm	r4, {r0, r1, r2, r3}
	mov	r3, r7
	ldr	r1, .L53+4
	mov	r0, r3
	bl	read_command
	ldrb	r3, [r7]	@ zero_extendqisi2
	cmp	r3, #116
	beq	.L69
	bl	SECURE_log_cond_br_not_taken
	cmp	r3, #117
	beq	.L68
	bl	SECURE_log_cond_br_not_taken
	cmp	r3, #97
	beq	.L70
	bl	SECURE_log_cond_br_not_taken
	b	.L52
.L68:
	bl	SECURE_log_cond_br_taken
.L49:
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	lsls	r3, r3, #8
	ldrb	r2, [r7, #2]	@ zero_extendqisi2
	orrs	r3, r3, r2
	ldr	r2, .L53+8
	str	r3, [r2]
	b	.L51
.L69:
	bl	SECURE_log_cond_br_taken
.L48:
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	lsls	r3, r3, #8
	ldrb	r2, [r7, #2]	@ zero_extendqisi2
	orrs	r3, r3, r2
	ldr	r2, .L53+12
	str	r3, [r2]
	ldr	r3, .L53+12
	ldr	r3, [r3]
	mov	r0, r3
	bl	SECURE_record_output_data
	bl	SECURE_record_output_data
	ldrb	r3, [r7, #4]	@ zero_extendqisi2
	lsls	r3, r3, #8
	ldrb	r2, [r7, #5]	@ zero_extendqisi2
	orrs	r3, r3, r2
	ldr	r2, .L53+16
	str	r3, [r2]
	ldr	r3, .L53+16
	ldr	r3, [r3]
	mov	r0, r3
	bl	SECURE_record_output_data
	bl	SECURE_record_output_data
	b	.L51
.L70:
	bl	SECURE_log_cond_br_taken
.L50:
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	lsls	r3, r3, #8
	ldrb	r2, [r7, #2]	@ zero_extendqisi2
	orrs	r3, r3, r2
	ldr	r2, .L53+20
	str	r3, [r2]
	ldrb	r3, [r7, #4]	@ zero_extendqisi2
	lsls	r3, r3, #8
	ldrb	r2, [r7, #5]	@ zero_extendqisi2
	orrs	r3, r3, r2
	ldr	r2, .L53+8
	str	r3, [r2]
	ldrb	r3, [r7, #7]	@ zero_extendqisi2
	lsls	r3, r3, #8
	ldrb	r2, [r7, #8]	@ zero_extendqisi2
	orrs	r3, r3, r2
	ldr	r2, .L53+12
	str	r3, [r2]
	ldrb	r3, [r7, #10]	@ zero_extendqisi2
	lsls	r3, r3, #8
	ldrb	r2, [r7, #11]	@ zero_extendqisi2
	orrs	r3, r3, r2
	ldr	r2, .L53+16
	str	r3, [r2]
.L52:
.L51:
	ldrb	r2, [r7]	@ zero_extendqisi2
	ldr	r3, .L53+24
	strb	r2, [r3]
	adds	r7, r7, #20
	mov	sp, r7
	@ sp needed
	pop	{r4, r7, lr}
	b	SECURE_log_ret
.L54:
	.align	2
.L53:
	.word	.LC0
	.word	input
	.word	ult_runs
	.word	temp_runs
	.word	hum_runs
	.word	seq_runs
	.word	cmd
	.size	process_command, .-process_command
	.comm	sensor_action,4,4
	.section	.text.record_output_data,"ax",%progbits
	.align	1
	.global	record_output_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	record_output_data, %function
record_output_data:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7, lr}
	sub	sp, sp, #12
	add	r7, sp, #0
	mov	r3, r0
	strb	r3, [r7, #7]
	ldrb	r3, [r7, #7]
	mvns	r3, r3
	uxtb	r3, r3
	mov	r0, r3
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
	.size	record_output_data, .-record_output_data
	.section	.text.application,"ax",%progbits
	.align	1
	.global	application
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv5-sp-d16
	.type	application, %function
application:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	bl	process_command
	ldr	r3, .L63
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #116
	beq	.L72
	bl	SECURE_log_cond_br_not_taken
	cmp	r3, #117
	beq	.L71
	bl	SECURE_log_cond_br_not_taken
	cmp	r3, #97
	beq	.L73
	bl	SECURE_log_cond_br_not_taken
	b	.L62
.L71:
	bl	SECURE_log_cond_br_taken
.L60:
	movs	r0, #117
	bl	record_output_data
	mov	r2, r0
	ldr	r3, .L63+4
	str	r2, [r3]
	ldr	r3, .L63+8
	ldr	r2, .L63+12
	str	r2, [r3]
	ldr	r3, .L63+16
	ldr	r3, [r3]
	ldr	r2, .L63+4
	str	r3, [r2]
	b	.L62
.L72:
	bl	SECURE_log_cond_br_taken
.L59:
	movs	r0, #116
	bl	record_output_data
	mov	r2, r0
	ldr	r3, .L63+4
	str	r2, [r3]
	ldr	r3, .L63+8
	ldr	r2, .L63+20
	str	r2, [r3]
	ldr	r3, .L63+24
	ldr	r3, [r3]
	ldr	r2, .L63+4
	str	r3, [r2]
	b	.L62
.L73:
	bl	SECURE_log_cond_br_taken
.L61:
	movs	r0, #97
	bl	record_output_data
	mov	r2, r0
	ldr	r3, .L63+4
	str	r2, [r3]
	ldr	r3, .L63+8
	ldr	r2, .L63+28
	str	r2, [r3]
	ldr	r3, .L63+32
	ldr	r3, [r3]
	ldr	r2, .L63+4
	str	r3, [r2]
.L62:
	ldr	r3, .L63+8
	ldr	r3, [r3]
	ldr	r2, .L63+4
	ldr	r2, [r2]
	uxtb	r2, r2
	mov	r0, r2
	mov	r10, r3
	bl	SECURE_log_call
	pop	{r7, lr}
	b	SECURE_log_ret
.L64:
	.align	2
.L63:
	.word	cmd
	.word	runs
	.word	sensor_action
	.word	run_ultrasonic
	.word	ult_runs
	.word	run_temperature
	.word	temp_runs
	.word	run_humidity
	.word	hum_runs
	.size	application, .-application
	.ident	"GCC: (15:6.3.1+svn253039-1build1) 6.3.1 20170620"
