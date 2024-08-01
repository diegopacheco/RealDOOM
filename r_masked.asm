;
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
	.MODEL  medium
	INCLUDE defs.inc

.DATA



EXTRN   _spryscale:DWORD
EXTRN   _sprtopscreen:WORD
EXTRN   _mfloorclip:WORD
EXTRN   _mceilingclip:WORD

EXTRN  _R_DrawColumnPrepCallHigh:WORD

EXTRN  _fuzzpos:BYTE





DRAWCOL_PREP_SEGMENT          = 06A42h
;DRAWCOL_PREP_SEGMENT_HIGH    = DRAWCOL_PREP_SEGMENT  - 06800h + 08C00h
DRAWCOL_PREP_SEGMENT_HIGH     = 08E42h
DRAWCOL_PREP_OFFSET           = 09D0h

FUZZ_OFFSET_SEGMENT           = 04B52h
COLORMAPS_HIGH_SEG_DIFF_SEGMENT = 08C60h




;=================================

.CODE


; ax pixelsegment
; cx:bx column


;
; R_DrawMaskedColumn
;
	
PROC  R_DrawMaskedColumn_ NEAR
PUBLIC  R_DrawMaskedColumn_ 

;  bp - 02 cx/column segment
;  bp - 04  ax/pixelsegment cache
;  bp - 06  cached dc_texturemid intbits to restore before function

push  dx
push  si
push  di
push  bp
mov   bp, sp
push  cx            ; bp - 2
mov   si, bx        ; si now holds column address.
mov   es, cx
push  ax            ; bp - 4

mov   cx, word ptr [_dc_texturemid+2]
push  cx            ; bp - 6
xor   di, di        ; di used as currentoffset.

cmp   byte ptr es:[si], 0FFh
jne   draw_next_column_patch
jmp   exit_function
draw_next_column_patch:

;        topscreen.w = sprtopscreen + FastMul16u32u(column->topdelta, spryscale.w);

mov   bx, word ptr [_spryscale]
mov   ax, word ptr [_spryscale+2]

mov   cl, byte ptr es:[si]
xor   ch, ch

;inlined fastmul16u32u
MUL  CX        ; AX * CX
XCHG CX, AX    ; store low product to be high result. Retrieve orig AX
MUL  BX        ; AX * BX
ADD  DX, CX    ; add 

; bx was preserved im mul
; DX:AX = fastmult result. 


add   ax, word ptr [_sprtopscreen]
adc   dx, word ptr [_sprtopscreen+2]

; topscreen = DX:AX.

;		dc_yl = topscreen.h.intbits; 
;		if (topscreen.h.fracbits)
;			dc_yl++;


neg  ax
adc  dx, 0
mov  [_dc_yl], dx
neg  ax
sbb  dx, 0

mov  ds, ax    ; store old topscreen
mov  ax, word ptr ss:[_spryscale+2]    ; use ss as ds as a hack...

mov  cl, byte ptr es:[si + 1] ; get length for mult
xor  ch, ch

mov  es, dx   ;  es:ds stores old topscreen result
 

;inlined fastmul16u32u
MUL  CX        ; AX * CX
XCHG CX, AX    ; store low product to be high result. Retrieve orig AX
MUL  BX        ; AX * BX
ADD  DX, CX    ; add 


;        bottomscreen.w = topscreen.w + FastMul16u32u(column->length, spryscale.w);
; add cached topscreen
mov   cx, ds
add   ax, cx
mov   cx, es
adc   dx, cx
mov   cx, ss
mov   ds, cx


;		dc_yh = bottomscreen.h.intbits;
;		if (!bottomscreen.h.fracbits)
;			dc_yh--;

neg ax
sbb dx, 0FFFFh
;mov [_dc_yh], dx
;neg ax
;adc dx, 0FFFFh

; dx is dc_yh but needs to be written back 

; dc_yh, dc_yl are set



;        if (dc_yh >= mfloorclip[dc_x])
;            dc_yh = mfloorclip[dc_x]-1;

mov   bx, word ptr [_dc_x]
mov   ax, word ptr [_mfloorclip]
add   bx, bx
mov   es, word ptr [_mfloorclip+2]
add   bx, ax

mov   cx, word ptr es:[bx]
cmp   dx, cx
jl    skip_floor_clip_set
mov   dx, cx
dec   dx
skip_floor_clip_set:
mov   word ptr [_dc_yh], dx



;        if (dc_yl <= mceilingclip[dc_x])
;            dc_yl = mceilingclip[dc_x]+1;


sub   bx, ax

mov   es, word ptr [_mceilingclip+2]
add   bx, word ptr [_mceilingclip]
mov   ax, word ptr [_dc_yl]
mov   cx, word ptr es:[bx]
cmp   ax, cx
jg    skip_ceil_clip_set
mov   ax, cx
inc   ax
mov   word ptr [_dc_yl], ax
skip_ceil_clip_set:

cmp   ax, word ptr [_dc_yh]
jg    increment_column_and_continue_loop
mov   ax, di

shr   ax, 1
shr   ax, 1
shr   ax, 1
shr   ax, 1
add   ax, word ptr [bp - 4]
mov   word ptr [_dc_source_segment], ax
mov   dx, word ptr [bp - 6]
mov   es, word ptr [bp - 2]
mov   al, byte ptr es:[si]
xor   ah, ah
sub   dx, ax
mov   word ptr [_dc_texturemid+2], dx
mov   ax, 02400h


db 0FFh
db 01Eh
dw _R_DrawColumnPrepCallHigh

increment_column_and_continue_loop:
mov   es, word ptr [bp - 2]
mov   al, byte ptr es:[si + 1]
xor   ah, ah

add   di, ax

neg   ax
and   ax, 0Fh
add   si, 2
add   di, ax
cmp   byte ptr es:[si], 0FFh
je    exit_function
jmp   draw_next_column_patch ; todo inverse and skip jump
exit_function:

mov   ax, word ptr [bp - 6]             ; restore dc_texture_mid
mov   word ptr [_dc_texturemid+2], ax
mov   sp, bp
pop   bp
pop   di
pop   si
pop   dx
ret


ENDP



;
; R_DrawSingleMaskedColumn
;
	
PROC  R_DrawSingleMaskedColumn_ NEAR
PUBLIC  R_DrawSingleMaskedColumn_ 

push  bx
push  cx
push  si
push  di
push  bp

mov   word ptr [_dc_source_segment], ax	; set this early. 

mov   cl, dl
xor   ch, ch		; count used once for mul and not again. todo is dh already zero?



;    topscreen.w = sprtopscreen;

mov   di, word ptr [_sprtopscreen]
mov   si, word ptr [_sprtopscreen+2]
mov   bx, word ptr [_spryscale]
mov   ax, word ptr [_spryscale+2]

;   topscreen = si:di 

; fastmul1632, ax/cx preswapped

; FastMul16u32u(length, spryscale.w)

MUL  CX        ; AX * CX
XCHG CX, AX    ; store low product to be high result. Retrieve orig AX
MUL  BX        ; AX * BX
ADD  DX, CX    ; add 

;    bottomscreen.w = topscreen.w + FastMul16u32u(length, spryscale.w);


add ax, di
adc dx, si

; dx:ax = bottomscreen
; si:di = topscreen (still)

;    dc_yh = bottomscreen.h.intbits;
;    if (!bottomscreen.h.fracbits)
;        dc_yh--;



neg  di
adc  si, 0
;mov  word ptr [_dc_yl], si

; dc_yl written back


;		dc_yh = bottomscreen.h.intbits;
;		if (!bottomscreen.h.fracbits)
;			dc_yh--;

neg ax
sbb dx, 0FFFFh
mov  word ptr [_dc_yh], dx
; dx is dc_yh
; si is dc_yl





skip_inc_dc_yl:

;        if (dc_yh >= mfloorclip[dc_x])
;            dc_yh = mfloorclip[dc_x]-1;

mov   bx, word ptr [_dc_x]
mov   ax, word ptr [_mfloorclip]
add   bx, bx
mov   es, word ptr [_mfloorclip+2]
add   bx, ax

mov   cx, word ptr es:[bx]
cmp   dx, cx
jl    skip_floor_clip_set_single	; todo consider making this jump out and back? whats the better default branch
mov   dx, cx
dec   dx
skip_floor_clip_set_single:



;        if (dc_yl <= mceilingclip[dc_x])
;            dc_yl = mceilingclip[dc_x]+1;


sub   bx, ax

mov   es, word ptr [_mceilingclip+2]
add   bx, word ptr [_mceilingclip]

mov   cx, word ptr es:[bx]
cmp   si, cx
jg    skip_ceil_clip_set_single   ; todo consider making this jump out and back? whats the better default branch
mov   si, cx
inc   si
skip_ceil_clip_set_single:

cmp   si, dx			
jnle   exit_function_single


mov   word ptr [_dc_yh], dx ; todo eventually just pass this in as an arg instead of write it
mov   word ptr [_dc_yl], si ;  dc_x could also be trivially recovered from bx

mov   ax, 02400h


db 0FFh
db 01Eh
dw _R_DrawColumnPrepCallHigh


exit_function_single:

pop   bp
pop   di
pop   si
pop   cx
pop   bx
ret   

ENDP


;
; R_DrawFuzzColumn
;
	
PROC  R_DrawFuzzColumn_ NEAR
PUBLIC  R_DrawFuzzColumn_ 



push dx
push si
push di
mov  dl, byte ptr [_fuzzpos]
xor  dh, dh
mov  si, ax

mov  di, FUZZ_OFFSET_SEGMENT
mov  ds, di
mov  di, bx

cmp  ax, 010h
jg   draw_16_fuzzpixels
jmp  done_drawing_16_fuzzpixels
draw_16_fuzzpixels:



DRAW_SINGLE_FUZZPIXEL MACRO 





mov  bx, dx
mov  al, byte ptr ds:[bx]
cbw 
mov  bx, ax
mov  es, cx
add  bx, di
mov  al, byte ptr es:[bx]
mov  bx, COLORMAPS_HIGH_SEG_DIFF_SEGMENT
xor  ah, ah
mov  es, bx
mov  bx, ax
mov  al, byte ptr es:[bx]
mov  es, cx
inc  dl
;mov  byte ptr es:[di], al
stosb

add  di, 04Fh
ENDM

REPT 16
    DRAW_SINGLE_FUZZPIXEL
endm






cmp  dl, 032h
jl   fuzzpos_ok
; subtract 50 from fuzzpos
sub  dl, 032h
fuzzpos_ok:
sub  si, 010h
cmp  si, 010h
jle  done_drawing_16_fuzzpixels
jmp  draw_16_fuzzpixels
done_drawing_16_fuzzpixels:
test si, si
je   finished_drawing_fuzzpixels

mov  bx, dx
mov  al, byte ptr ds:[bx]
cbw 
mov  bx, di
mov  es, cx
add  bx, ax
mov  bl, byte ptr es:[bx]
mov  ax, COLORMAPS_HIGH_SEG_DIFF_SEGMENT
xor  bh, bh
mov  es, ax
mov  al, byte ptr es:[bx]
mov  es, cx
inc  dl
;mov  byte ptr es:[di], al
stosb

add  di, 04Fh
dec  si
cmp  dl, 032h
je   zero_out_fuzzpos
finish_one_fuzzpixel_iteration:
jmp  done_drawing_16_fuzzpixels
zero_out_fuzzpos:
xor  dl, dl
jmp  done_drawing_16_fuzzpixels
; write back fuzzpos
finished_drawing_fuzzpixels:

; restore ds
mov  di, ss
mov  ds, di

; write back fuzzpos
mov  byte ptr [_fuzzpos], dl

pop  di
pop  si
pop  dx
retf 

ENDP


END