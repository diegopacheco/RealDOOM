; Copyright (C) 1993-1996 Id Software, Inc.
; Copyright (C) 1993-2008 Raven Software
; Copyright (C) 2016-2017 Alexey Khokholov (Nuke.YKT)
;
; This program is free software; you can redistribute it and/or
; modify it under the terms of the GNU General Public License
; as published by the Free Software Foundation; either version 2
; of the License, or (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; DESCRIPTION:
;
    .286
	.MODEL  medium


FINESINE_SEGMENT               = 31e4h

EXTRN _tantoangle:DWORD
EXTRN _viewx:DWORD
EXTRN _viewy:DWORD
EXTRN _viewangle_shiftright3:WORD
EXTRN _projection:WORD
EXTRN _rw_distance:WORD
EXTRN _rw_normalangle:WORD

EXTRN FastDiv3232_shift_3_8_:PROC
EXTRN FixedMulTrig_:PROC
EXTRN div48_32_:PROC

INCLUDE defs.inc

.CODE


octant_6:
test  cx, cx

jne   octant_6_do_divide
cmp   bx, 0200h
jae   octant_6_do_divide
octant_6_out_of_bounds:
mov   dx, 0e000h
xor   ax, ax

retf  
octant_6_do_divide:
call FastDiv3232_shift_3_8_
cmp   ax, 0800h
jae   octant_6_out_of_bounds
mov   bx, word ptr [_tantoangle]
shl   ax, 2
mov   es, word ptr [_tantoangle+2]
add   bx, ax
mov   ax, word ptr es:[bx]
mov   dx, word ptr es:[bx + 2]
add   dx, 0c000h

retf  

y_is_negative:
;			y.w = -y.w;

neg   cx
neg   bx
sbb   cx, 0

cmp   dx, cx
jg    octant_7
jne   octant_6
cmp   ax, bx
jbe   octant_6
octant_7:
test  dx, dx
jne   octant_7_do_divide
cmp   ax, 0200h
jae   octant_7_do_divide
octant_7_out_of_bounds:
mov   dx, 0e000h
xor   ax, ax

retf  
; result 16f01520
; 7ffd1a dx:ax
; 3077f6 cx:bx
; 5400000 -> 0x2A000000
; d400000  > 0xD4000    32B 811

;mov dx, cx
;mov ax, bx


octant_7_do_divide:

; swap params. y over x not x over y
xchg dx, cx
xchg ax, bx

call FastDiv3232_shift_3_8_

; 16f0  1520 instead of 32b

cmp   ax, 0800h
jae   octant_7_out_of_bounds
mov   bx, word ptr [_tantoangle]
shl   ax, 2
mov   es, word ptr [_tantoangle+2]
add   bx, ax
mov   ax, word ptr es:[bx]
mov   dx, word ptr es:[bx + 2]
neg   dx
neg   ax
sbb   dx, 0

retf  

;R_PointToAngle_

PROC R_PointToAngle_
PUBLIC R_PointToAngle_

; inputs:
; DX:AX = x  (32 bit fixed pt 16:16)
; CX:BX = y  (32 bit fixed pt 16:16)

; places to improve -
; 1.default branches taken. count branches taken and modify to optimize

;	x.w -= viewx.w;
;	y.w -= viewy.w;



sub   ax, word ptr [_viewx]
sbb   dx, word ptr [_viewx+2]

sub   bx, word ptr [_viewy]
sbb   cx, word ptr [_viewy+2]

; 	if ((!x.w) && (!y.w))
;		return 0;

test  dx, dx
jne   inputs_not_zero   ; todo rearrange this. rare case
test  cx, cx
jne   inputs_not_zero   ; todo rearrange this. rare case
test  ax, ax
jne   inputs_not_zero   ; todo rearrange this. rare case
test  bx, bx
jne   inputs_not_zero   ; todo rearrange this. rare case
return_0:

xor   ax, ax
cwd

retf  


inputs_not_zero:

test  dx, dx
jl   x_is_negative

x_is_positive:
test  cx, cx

jl   y_is_negative
y_is_positive:

cmp   dx, cx
jg    octant_0

jne   octant_1
cmp   ax, bx
jbe   octant_1


octant_0:
test  dx, dx

;	if (x.w < 512)

jne   octant_0_do_divide
cmp   ax, 0200h
jae   octant_0_do_divide
octant_0_out_of_bounds:
mov   dx, 02000h
xor   ax, ax

retf  


octant_0_do_divide:
;x_is_negative
xchg dx, cx
xchg ax, bx
call FastDiv3232_shift_3_8_
cmp   ax, 0800h
jae   octant_0_out_of_bounds

mov   bx, word ptr [_tantoangle]
shl   ax, 2
mov   es, word ptr [_tantoangle+2]
add   bx, ax
mov   ax, word ptr es:[bx]
mov   dx, word ptr es:[bx + 2]

retf  


octant_1:
test  cx, cx

jne   octant_1_do_divide
cmp   bx, 0200h
jae   octant_1_do_divide
octant_1_out_of_bounds:
mov   ax, 0ffffh
mov   dx, 01fffh

retf  
octant_1_do_divide:
call FastDiv3232_shift_3_8_
cmp   ax, 0800h
jae   octant_1_out_of_bounds
mov   bx, word ptr [_tantoangle]
shl   ax, 2
mov   es, word ptr [_tantoangle+2]
add   bx, ax
mov   ax, 0ffffh
sub   ax, word ptr es:[bx]
mov   dx, 03fffh
sbb   dx, word ptr es:[bx + 2]

retf  



x_is_negative:

;		x.w = -x.w;

neg   dx
neg   ax
sbb   dx, 0

test  cx, cx

jg    y_is_positive_x_neg
jne   y_is_negative_x_neg
y_is_positive_x_neg:
cmp   dx, cx
jg    octant_3
jne   octant_2
cmp   ax, bx
jbe   octant_2

octant_3:
test  dx, dx
jne   octant_3_do_divide
cmp   ax, 0200h
jae   octant_3_do_divide
octant_3_out_of_bounds:
mov   ax, 0ffffh
mov   dx, 05fffh

retf  
octant_3_do_divide:
xchg dx, cx
xchg ax, bx
call FastDiv3232_shift_3_8_
cmp   ax, 0800h
jae   octant_3_out_of_bounds
mov   bx, word ptr [_tantoangle]
shl   ax, 2
mov   es, word ptr [_tantoangle+2]
add   bx, ax
mov   ax, 0ffffh
sub   ax, word ptr es:[bx]
mov   dx, 07fffh
sbb   dx, word ptr es:[bx + 2]

retf  
octant_2:
test  cx, cx

jne   octant_2_do_divide
cmp   ax, 0200h
jae   octant_2_do_divide
octant_2_out_of_bounds:
mov   dx, 06000h
xor   ax, ax
retf  
octant_2_do_divide:

call FastDiv3232_shift_3_8_
cmp   ax, 0800h
jae   octant_2_out_of_bounds
mov   bx, word ptr [_tantoangle]
shl   ax, 2
mov   es, word ptr [_tantoangle+2]
add   bx, ax
mov   ax, word ptr es:[bx]
mov   dx, word ptr es:[bx + 2]
add   dx, 04000h

retf  
y_is_negative_x_neg:

;			y.w = -y.w;

neg   cx
neg   bx
sbb   cx, 0
cmp   dx, cx
jg    octant_4
jne   octant_5
cmp   ax, bx
jbe   octant_5
octant_4:
test  dx, dx
jne   octant_4_do_divide
cmp   ax, 0200h
jae   octant_4_do_divide
octant_4_out_of_bounds:
mov   dx, 0a000h
xor   ax, ax

retf  
octant_4_do_divide:
xchg dx, cx
xchg ax, bx
call FastDiv3232_shift_3_8_
cmp   ax, 0800h
jae   octant_4_out_of_bounds

mov   bx, word ptr [_tantoangle]
shl   ax, 2
mov   es, word ptr [_tantoangle+2]
add   bx, ax
mov   ax, word ptr es:[bx]
mov   dx, word ptr es:[bx + 2]
add   dx, 08000h

retf  
octant_5:
test  cx, cx

jne   octant_5_do_divide
cmp   ax, 0200h
jae   octant_5_do_divide
octant_5_out_of_bounds:
mov   ax, 0ffffh
mov   dx, 09fffh

retf  
octant_5_do_divide:

call FastDiv3232_shift_3_8_
cmp   ax, 0800h
jae   octant_5_out_of_bounds
mov   bx, word ptr [_tantoangle]
shl   ax, 2
mov   es, word ptr [_tantoangle+2]
add   bx, ax
mov   ax, 0ffffh
sub   ax, word ptr es:[bx]
mov   dx, 0bfffh
sbb   dx, word ptr es:[bx + 2]

retf  
endp



;R_ScaleFromGlobalAngle_

PROC R_ScaleFromGlobalAngle3_
PUBLIC R_ScaleFromGlobalAngle3_


push  bx
push  cx
push  si
push  di

; input ax = visangle_shift3

;    anglea = MOD_FINE_ANGLE(FINE_ANG90 + (visangle_shift3 - viewangle_shiftright3));
;    angleb = MOD_FINE_ANGLE(FINE_ANG90 + (visangle_shift3) - rw_normalangle);

add   ah, 8      
mov   dx, ax      ; copy input
sub   dx, word ptr [_viewangle_shiftright3]  ; 
sub   ax, word ptr [_rw_normalangle]

and   dh, 01Fh
and   ah, 01Fh

mov   di, ax

; dx = anglea
; di = angleb

mov   ax, FINESINE_SEGMENT
mov   si, ax
mov   bx, word ptr [_rw_distance]
mov   cx, word ptr [_rw_distance+2]

;    den = FixedMulTrig(FINE_SINE_ARGUMENT, anglea, rw_distance);
 
call FixedMulTrig_

xchg  dx, di
xchg  si, ax
;  dx now has anglea
;  ax has finesine_segment
;  di:si is den

mov   bx, word ptr [_projection]
mov   cx, word ptr [_projection+2]


;    num.w = FixedMulTrig(FINE_SINE_ARGUMENT, angleb, projection.w)<<detailshift.b.bytelow;
 
call FixedMulTrig_

; di:si had den
; dx:ax has num

mov   cl, byte ptr [_detailshift]
xor   ch, ch

; cl is 0 to 2

jcxz  shift_done
shl   ax, 1
rcl   dx, 1
dec   cl
jcxz  shift_done
shl   ax, 1
rcl   dx, 1

shift_done:


; di:si had den
; dx:ax has num



;    if (den > num.h.intbits) {

; annoying - we have to account for sign!
; is there a cleaner way?

 
mov    cx, ax  ; temp storage
mov    ax, dx
cwd            ; sign extend

cmp   di, dx

jg    do_divide  ; compare sign bits..

; todo we can bitshift and catch more cases here...

; this is fixeddiv so result is shifted by 16 basically...
; DX:AX:00  /  CX:BX
; result is bits

; if cx > dx then the result is less than than 0x10000 (not greater than the max of 0x400000)


jne   return_maxvalue   ; less than case - result is greater than 0x1,0000,0000


; result smaller than 1

cmp   si, ax    
ja    do_divide
return_maxvalue:
mov   dx, 040h
xor   ax, ax
normal_return:

pop   di
pop   si
pop   cx
pop   bx
ret
do_divide:

; set up params
mov   dx, ax  ; mov back
mov   ax, cx  ; mov back..
mov   cx, di 
mov   bx, si 

; we actually already bounds check more aggressively than fixeddiv
;  and guarantee positives here so the fixeddiv wrapper is unnecessary

; NOTE: a high word bounds triggered early return on the first divide result 
;   is super rare due to the outer checks...
;   doesnt occur even every frame. lets avoid the "optimized" dupe function.


call div48_32_

cmp   dx, 040h
jg    return_maxvalue
test  dx, dx
; dont need to check for negative result, this was unsigned.
jne   normal_return
cmp   ax, 0100h
jae   normal_return
return_minvalue:
mov   ax, 0100h
xor   dx, dx

pop   di
pop   si
pop   cx
pop   bx
ret

endp






END
