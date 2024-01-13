///
// Copyright (C) 1993-1996 Id Software, Inc.
// Copyright (C) 2016-2017 Alexey Khokholov (Nuke.YKT)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Thing frame/state LUT,
//	generated by multigen utilitiy.
//	This one is the original DOOM version, preserved.
//

#ifndef __INFO__
#define __INFO__

// Needed for action function pointer handling.
#include "d_think.h"

#define STATENUM_NULL -1
#define SPR_TROO 0
#define SPR_SHTG 1
#define SPR_PUNG 2
#define SPR_PISG 3
#define SPR_PISF 4
#define SPR_SHTF 5
#define SPR_SHT2 6
#define SPR_CHGG 7
#define SPR_CHGF 8
#define SPR_MISG 9
#define SPR_MISF 10
#define SPR_SAWG 11
#define SPR_PLSG 12
#define SPR_PLSF 13
#define SPR_BFGG 14
#define SPR_BFGF 15
#define SPR_BLUD 16
#define SPR_PUFF 17
#define SPR_BAL1 18
#define SPR_BAL2 19
#define SPR_PLSS 20
#define SPR_PLSE 21
#define SPR_MISL 22
#define SPR_BFS1 23
#define SPR_BFE1 24
#define SPR_BFE2 25
#define SPR_TFOG 26
#define SPR_IFOG 27
#define SPR_PLAY 28
#define SPR_POSS 29
#define SPR_SPOS 30
#define SPR_VILE 31
#define SPR_FIRE 32
#define SPR_FATB 33
#define SPR_FBXP 34
#define SPR_SKEL 35
#define SPR_MANF 36
#define SPR_FATT 37
#define SPR_CPOS 38
#define SPR_SARG 39
#define SPR_HEAD 40
#define SPR_BAL7 41
#define SPR_BOSS 42
#define SPR_BOS2 43
#define SPR_SKUL 44
#define SPR_SPID 45
#define SPR_BSPI 46
#define SPR_APLS 47
#define SPR_APBX 48
#define SPR_CYBR 49
#define SPR_PAIN 50
#define SPR_SSWV 51
#define SPR_KEEN 52
#define SPR_BBRN 53
#define SPR_BOSF 54
#define SPR_ARM1 55
#define SPR_ARM2 56
#define SPR_BAR1 57
#define SPR_BEXP 58
#define SPR_FCAN 59
#define SPR_BON1 60
#define SPR_BON2 61
#define SPR_BKEY 62
#define SPR_RKEY 63
#define SPR_YKEY 64
#define SPR_BSKU 65
#define SPR_RSKU 66
#define SPR_YSKU 67
#define SPR_STIM 68
#define SPR_MEDI 69
#define SPR_SOUL 70
#define SPR_PINV 71
#define SPR_PSTR 72
#define SPR_PINS 73
#define SPR_MEGA 74
#define SPR_SUIT 75
#define SPR_PMAP 76
#define SPR_PVIS 77
#define SPR_CLIP 78
#define SPR_AMMO 79
#define SPR_ROCK 80
#define SPR_BROK 81
#define SPR_CELL 82
#define SPR_CELP 83
#define SPR_SHEL 84
#define SPR_SBOX 85
#define SPR_BPAK 86
#define SPR_BFUG 87
#define SPR_MGUN 88
#define SPR_CSAW 89
#define SPR_LAUN 90
#define SPR_PLAS 91
#define SPR_SHOT 92
#define SPR_SGN2 93
#define SPR_COLU 94
#define SPR_SMT2 95
#define SPR_GOR1 96
#define SPR_POL2 97
#define SPR_POL5 98
#define SPR_POL4 99
#define SPR_POL3 100
#define SPR_POL1 101
#define SPR_POL6 102
#define SPR_GOR2 103
#define SPR_GOR3 104
#define SPR_GOR4 105
#define SPR_GOR5 106
#define SPR_SMIT 107
#define SPR_COL1 108
#define SPR_COL2 109
#define SPR_COL3 110
#define SPR_COL4 111
#define SPR_CAND 112
#define SPR_CBRA 113 
#define SPR_COL6 114
#define SPR_TRE1 115
#define SPR_TRE2 116
#define SPR_ELEC 117
#define SPR_CEYE 118
#define SPR_FSKU 119
#define SPR_COL5 120
#define SPR_TBLU 121
#define SPR_TGRN 122
#define SPR_TRED 123
#define SPR_SMBT 124
#define SPR_SMGT 125
#define SPR_SMRT 126
#define SPR_HDB1 127
#define SPR_HDB2 128
#define SPR_HDB3 129
#define SPR_HDB4 130
#define SPR_HDB5 131
#define SPR_HDB6 132
#define SPR_POB1 133
#define SPR_POB2 134
#define SPR_BRS1 135
#define SPR_TLMP 136
#define SPR_TLP2 137
#define NUMSPRITES 138

typedef uint8_t spritenum_t;
typedef uint8_t spriteframenum_t;

#define S_NULL 			0
#define S_LIGHTDONE       1
#define S_PUNCH           2
#define S_PUNCHDOWN       3
#define S_PUNCHUP         4
#define S_PUNCH1          5
#define S_PUNCH2          6
#define S_PUNCH3          7
#define S_PUNCH4          8
#define S_PUNCH5          9
#define S_PISTOL          10
#define S_PISTOLDOWN      11
#define S_PISTOLUP        12
#define S_PISTOL1         13
#define S_PISTOL2         14
#define S_PISTOL3         15
#define S_PISTOL4         16
#define S_PISTOLFLASH     17
#define S_SGUN            18
#define S_SGUNDOWN        19
#define S_SGUNUP          20
#define S_SGUN1           21
#define S_SGUN2           22
#define S_SGUN3           23
#define S_SGUN4           24
#define S_SGUN5           25
#define S_SGUN6           26
#define S_SGUN7           27
#define S_SGUN8           28
#define S_SGUN9           29
#define S_SGUNFLASH1      30
#define S_SGUNFLASH2      31
#define S_DSGUN           32
#define S_DSGUNDOWN       33
#define S_DSGUNUP         34
#define S_DSGUN1          35
#define S_DSGUN2          36
#define S_DSGUN3          37
#define S_DSGUN4          38
#define S_DSGUN5          39
#define S_DSGUN6          40
#define S_DSGUN7          41
#define S_DSGUN8          42
#define S_DSGUN9          43
#define S_DSGUN10         44
#define S_DSNR1           45
#define S_DSNR2           46
#define S_DSGUNFLASH1     47
#define S_DSGUNFLASH2     48
#define S_CHAIN           49
#define S_CHAINDOWN       50
#define S_CHAINUP         51
#define S_CHAIN1          52
#define S_CHAIN2          53
#define S_CHAIN3          54
#define S_CHAINFLASH1     55
#define S_CHAINFLASH2     56
#define S_MISSILE         57
#define S_MISSILEDOWN     58
#define S_MISSILEUP       59
#define S_MISSILE1        60
#define S_MISSILE2        61
#define S_MISSILE3        62
#define S_MISSILEFLASH1   63
#define S_MISSILEFLASH2   64
#define S_MISSILEFLASH3   65
#define S_MISSILEFLASH4   66
#define S_SAW             67
#define S_SAWB            68
#define S_SAWDOWN         69
#define S_SAWUP           70
#define S_SAW1            71
#define S_SAW2            72
#define S_SAW3            73
#define S_PLASMA          74
#define S_PLASMADOWN      75
#define S_PLASMAUP        76
#define S_PLASMA1         77
#define S_PLASMA2         78
#define S_PLASMAFLASH1    79
#define S_PLASMAFLASH2    80
#define S_BFG             81
#define S_BFGDOWN         82
#define S_BFGUP           83
#define S_BFG1            84
#define S_BFG2            85
#define S_BFG3            86
#define S_BFG4            87
#define S_BFGFLASH1       88
#define S_BFGFLASH2       89
#define S_BLOOD1          90
#define S_BLOOD2          91
#define S_BLOOD3          92
#define S_PUFF1           93
#define S_PUFF2           94
#define S_PUFF3           95
#define S_PUFF4           96
#define S_TBALL1          97
#define S_TBALL2          98
#define S_TBALLX1         99
#define S_TBALLX2         100
#define S_TBALLX3         101
#define S_RBALL1          102
#define S_RBALL2          103
#define S_RBALLX1         104
#define S_RBALLX2         105
#define S_RBALLX3         106
#define S_PLASBALL        107
#define S_PLASBALL2       108
#define S_PLASEXP         109
#define S_PLASEXP2        110
#define S_PLASEXP3        111
#define S_PLASEXP4        112
#define S_PLASEXP5        113
#define S_ROCKET          114
#define S_BFGSHOT         115
#define S_BFGSHOT2        116
#define S_BFGLAND         117
#define S_BFGLAND2        118
#define S_BFGLAND3        119
#define S_BFGLAND4        120
#define S_BFGLAND5        121
#define S_BFGLAND6        122
#define S_BFGEXP          123
#define S_BFGEXP2         124
#define S_BFGEXP3         125
#define S_BFGEXP4         126
#define S_EXPLODE1        127
#define S_EXPLODE2        128
#define S_EXPLODE3        129
#define S_TFOG            130
#define S_TFOG01          131
#define S_TFOG02          132
#define S_TFOG2           133
#define S_TFOG3           134
#define S_TFOG4           135
#define S_TFOG5           136
#define S_TFOG6           137
#define S_TFOG7           138
#define S_TFOG8           139
#define S_TFOG9           140
#define S_TFOG10          141
#define S_IFOG            142
#define S_IFOG01          143
#define S_IFOG02          144
#define S_IFOG2           145
#define S_IFOG3           146
#define S_IFOG4           147
#define S_IFOG5           148
#define S_PLAY            149
#define S_PLAY_RUN1       150
#define S_PLAY_RUN2       151
#define S_PLAY_RUN3       152
#define S_PLAY_RUN4       153
#define S_PLAY_ATK1       154
#define S_PLAY_ATK2       155
#define S_PLAY_PAIN       156
#define S_PLAY_PAIN2      157
#define S_PLAY_DIE1       158
#define S_PLAY_DIE2       159
#define S_PLAY_DIE3       160
#define S_PLAY_DIE4       161
#define S_PLAY_DIE5       162
#define S_PLAY_DIE6       163
#define S_PLAY_DIE7       164
#define S_PLAY_XDIE1      165
#define S_PLAY_XDIE2      166
#define S_PLAY_XDIE3      167
#define S_PLAY_XDIE4      168
#define S_PLAY_XDIE5      169
#define S_PLAY_XDIE6      170
#define S_PLAY_XDIE7      171
#define S_PLAY_XDIE8      172
#define S_PLAY_XDIE9      173
#define S_POSS_STND       174
#define S_POSS_STND2      175
#define S_POSS_RUN1       176
#define S_POSS_RUN2       177
#define S_POSS_RUN3       178
#define S_POSS_RUN4       179
#define S_POSS_RUN5       180
#define S_POSS_RUN6       181
#define S_POSS_RUN7       182
#define S_POSS_RUN8       183
#define S_POSS_ATK1       184
#define S_POSS_ATK2       185
#define S_POSS_ATK3       186
#define S_POSS_PAIN       187
#define S_POSS_PAIN2      188
#define S_POSS_DIE1       189
#define S_POSS_DIE2       190
#define S_POSS_DIE3       191
#define S_POSS_DIE4       192
#define S_POSS_DIE5       193
#define S_POSS_XDIE1      194
#define S_POSS_XDIE2      195
#define S_POSS_XDIE3      196
#define S_POSS_XDIE4      197
#define S_POSS_XDIE5      198
#define S_POSS_XDIE6      199
#define S_POSS_XDIE7      200
#define S_POSS_XDIE8      201
#define S_POSS_XDIE9      202
#define S_POSS_RAISE1     203
#define S_POSS_RAISE2     204
#define S_POSS_RAISE3     205
#define S_POSS_RAISE4     206
#define S_SPOS_STND       207
#define S_SPOS_STND2      208
#define S_SPOS_RUN1       209
#define S_SPOS_RUN2       210
#define S_SPOS_RUN3       211
#define S_SPOS_RUN4       212
#define S_SPOS_RUN5       213
#define S_SPOS_RUN6       214
#define S_SPOS_RUN7       215
#define S_SPOS_RUN8       216
#define S_SPOS_ATK1       217
#define S_SPOS_ATK2       218
#define S_SPOS_ATK3       219
#define S_SPOS_PAIN       220
#define S_SPOS_PAIN2      221
#define S_SPOS_DIE1       222
#define S_SPOS_DIE2       223
#define S_SPOS_DIE3       224
#define S_SPOS_DIE4       225
#define S_SPOS_DIE5       226
#define S_SPOS_XDIE1      227
#define S_SPOS_XDIE2      228
#define S_SPOS_XDIE3      229
#define S_SPOS_XDIE4      230
#define S_SPOS_XDIE5      231
#define S_SPOS_XDIE6      232
#define S_SPOS_XDIE7      233
#define S_SPOS_XDIE8      234
#define S_SPOS_XDIE9      235
#define S_SPOS_RAISE1     236
#define S_SPOS_RAISE2     237
#define S_SPOS_RAISE3     238
#define S_SPOS_RAISE4     239
#define S_SPOS_RAISE5     240
#define S_VILE_STND       241
#define S_VILE_STND2      242
#define S_VILE_RUN1       243
#define S_VILE_RUN2       244
#define S_VILE_RUN3       245
#define S_VILE_RUN4       246
#define S_VILE_RUN5       247
#define S_VILE_RUN6       248
#define S_VILE_RUN7       249
#define S_VILE_RUN8       250
#define S_VILE_RUN9       251
#define S_VILE_RUN10      252
#define S_VILE_RUN11      253
#define S_VILE_RUN12      254
#define S_VILE_ATK1       255
#define S_VILE_ATK2       256
#define S_VILE_ATK3       257
#define S_VILE_ATK4       258
#define S_VILE_ATK5       259
#define S_VILE_ATK6       260
#define S_VILE_ATK7       261
#define S_VILE_ATK8       262
#define S_VILE_ATK9       263
#define S_VILE_ATK10      264
#define S_VILE_ATK11      265
#define S_VILE_HEAL1      266
#define S_VILE_HEAL2      267
#define S_VILE_HEAL3      268
#define S_VILE_PAIN       269
#define S_VILE_PAIN2      270
#define S_VILE_DIE1       271
#define S_VILE_DIE2       272
#define S_VILE_DIE3       273
#define S_VILE_DIE4       274
#define S_VILE_DIE5       275
#define S_VILE_DIE6       276
#define S_VILE_DIE7       277
#define S_VILE_DIE8       278
#define S_VILE_DIE9       279
#define S_VILE_DIE10      280
#define S_FIRE1           281
#define S_FIRE2           282
#define S_FIRE3           283
#define S_FIRE4           284
#define S_FIRE5           285
#define S_FIRE6           286
#define S_FIRE7           287
#define S_FIRE8           288
#define S_FIRE9           289
#define S_FIRE10          290
#define S_FIRE11          291
#define S_FIRE12          292
#define S_FIRE13          293
#define S_FIRE14          294
#define S_FIRE15          295
#define S_FIRE16          296
#define S_FIRE17          297
#define S_FIRE18          298
#define S_FIRE19          299
#define S_FIRE20          300
#define S_FIRE21          301
#define S_FIRE22          302
#define S_FIRE23          303
#define S_FIRE24          304
#define S_FIRE25          305
#define S_FIRE26          306
#define S_FIRE27          307
#define S_FIRE28          308
#define S_FIRE29          309
#define S_FIRE30          310
#define S_SMOKE1          311
#define S_SMOKE2          312
#define S_SMOKE3          313
#define S_SMOKE4          314
#define S_SMOKE5          315
#define S_TRACER          316
#define S_TRACER2         317
#define S_TRACEEXP1       318
#define S_TRACEEXP2       319
#define S_TRACEEXP3       320
#define S_SKEL_STND       321
#define S_SKEL_STND2      322
#define S_SKEL_RUN1       323
#define S_SKEL_RUN2       324
#define S_SKEL_RUN3       325
#define S_SKEL_RUN4       326
#define S_SKEL_RUN5       327
#define S_SKEL_RUN6       328
#define S_SKEL_RUN7       329
#define S_SKEL_RUN8       330
#define S_SKEL_RUN9       331
#define S_SKEL_RUN10      332
#define S_SKEL_RUN11      333
#define S_SKEL_RUN12      334
#define S_SKEL_FIST1      335
#define S_SKEL_FIST2      336
#define S_SKEL_FIST3      337
#define S_SKEL_FIST4      338
#define S_SKEL_MISS1      339
#define S_SKEL_MISS2      340
#define S_SKEL_MISS3      341
#define S_SKEL_MISS4      342
#define S_SKEL_PAIN       343
#define S_SKEL_PAIN2      344
#define S_SKEL_DIE1       345
#define S_SKEL_DIE2       346
#define S_SKEL_DIE3       347
#define S_SKEL_DIE4       348
#define S_SKEL_DIE5       349
#define S_SKEL_DIE6       350
#define S_SKEL_RAISE1     351
#define S_SKEL_RAISE2     352
#define S_SKEL_RAISE3     353
#define S_SKEL_RAISE4     354
#define S_SKEL_RAISE5     355
#define S_SKEL_RAISE6     356
#define S_FATSHOT1        357
#define S_FATSHOT2        358
#define S_FATSHOTX1       359
#define S_FATSHOTX2       360
#define S_FATSHOTX3       361
#define S_FATT_STND       362
#define S_FATT_STND2      363
#define S_FATT_RUN1       364
#define S_FATT_RUN2       365
#define S_FATT_RUN3       366
#define S_FATT_RUN4       367
#define S_FATT_RUN5       368
#define S_FATT_RUN6       369
#define S_FATT_RUN7       370
#define S_FATT_RUN8       371
#define S_FATT_RUN9       372
#define S_FATT_RUN10      373
#define S_FATT_RUN11      374
#define S_FATT_RUN12      375
#define S_FATT_ATK1       376
#define S_FATT_ATK2       377
#define S_FATT_ATK3       378
#define S_FATT_ATK4       379
#define S_FATT_ATK5       380
#define S_FATT_ATK6       381
#define S_FATT_ATK7       382
#define S_FATT_ATK8       383
#define S_FATT_ATK9       384
#define S_FATT_ATK10      385
#define S_FATT_PAIN       386
#define S_FATT_PAIN2      387
#define S_FATT_DIE1       388
#define S_FATT_DIE2       389
#define S_FATT_DIE3       390
#define S_FATT_DIE4       391
#define S_FATT_DIE5       392
#define S_FATT_DIE6       393
#define S_FATT_DIE7       394
#define S_FATT_DIE8       395
#define S_FATT_DIE9       396
#define S_FATT_DIE10      397
#define S_FATT_RAISE1     398
#define S_FATT_RAISE2     399
#define S_FATT_RAISE3     400
#define S_FATT_RAISE4     401
#define S_FATT_RAISE5     402
#define S_FATT_RAISE6     403
#define S_FATT_RAISE7     404
#define S_FATT_RAISE8     405
#define S_CPOS_STND       406
#define S_CPOS_STND2      407
#define S_CPOS_RUN1       408
#define S_CPOS_RUN2       409
#define S_CPOS_RUN3       410
#define S_CPOS_RUN4       411
#define S_CPOS_RUN5       412
#define S_CPOS_RUN6       413
#define S_CPOS_RUN7       414
#define S_CPOS_RUN8       415
#define S_CPOS_ATK1       416
#define S_CPOS_ATK2       417
#define S_CPOS_ATK3       418
#define S_CPOS_ATK4       419
#define S_CPOS_PAIN       420
#define S_CPOS_PAIN2      421
#define S_CPOS_DIE1       422
#define S_CPOS_DIE2       423
#define S_CPOS_DIE3       424
#define S_CPOS_DIE4       425
#define S_CPOS_DIE5       426
#define S_CPOS_DIE6       427
#define S_CPOS_DIE7       428
#define S_CPOS_XDIE1      429
#define S_CPOS_XDIE2      430
#define S_CPOS_XDIE3      431
#define S_CPOS_XDIE4      432
#define S_CPOS_XDIE5      433
#define S_CPOS_XDIE6      434
#define S_CPOS_RAISE1     435
#define S_CPOS_RAISE2     436
#define S_CPOS_RAISE3     437
#define S_CPOS_RAISE4     438
#define S_CPOS_RAISE5     439
#define S_CPOS_RAISE6     440
#define S_CPOS_RAISE7     441
#define S_TROO_STND       442
#define S_TROO_STND2      443
#define S_TROO_RUN1       444
#define S_TROO_RUN2       445
#define S_TROO_RUN3       446
#define S_TROO_RUN4       447
#define S_TROO_RUN5       448
#define S_TROO_RUN6       449
#define S_TROO_RUN7       450
#define S_TROO_RUN8       451
#define S_TROO_ATK1       452
#define S_TROO_ATK2       453
#define S_TROO_ATK3       454
#define S_TROO_PAIN       455
#define S_TROO_PAIN2      456
#define S_TROO_DIE1       457
#define S_TROO_DIE2       458
#define S_TROO_DIE3       459
#define S_TROO_DIE4       460
#define S_TROO_DIE5       461
#define S_TROO_XDIE1      462
#define S_TROO_XDIE2      463
#define S_TROO_XDIE3      464
#define S_TROO_XDIE4      465
#define S_TROO_XDIE5      466
#define S_TROO_XDIE6      467
#define S_TROO_XDIE7      468
#define S_TROO_XDIE8      469
#define S_TROO_RAISE1     470
#define S_TROO_RAISE2     471
#define S_TROO_RAISE3     472
#define S_TROO_RAISE4     473
#define S_TROO_RAISE5     474
#define S_SARG_STND       475
#define S_SARG_STND2      476
#define S_SARG_RUN1       477
#define S_SARG_RUN2       478
#define S_SARG_RUN3       479
#define S_SARG_RUN4       480
#define S_SARG_RUN5       481
#define S_SARG_RUN6       482
#define S_SARG_RUN7       483
#define S_SARG_RUN8       484
#define S_SARG_ATK1       485
#define S_SARG_ATK2       486
#define S_SARG_ATK3       487
#define S_SARG_PAIN       488
#define S_SARG_PAIN2      489
#define S_SARG_DIE1       490
#define S_SARG_DIE2       491
#define S_SARG_DIE3       492
#define S_SARG_DIE4       493
#define S_SARG_DIE5       494
#define S_SARG_DIE6       495
#define S_SARG_RAISE1     496
#define S_SARG_RAISE2     497
#define S_SARG_RAISE3     498
#define S_SARG_RAISE4     499
#define S_SARG_RAISE5     500
#define S_SARG_RAISE6     501
#define S_HEAD_STND       502
#define S_HEAD_RUN1       503
#define S_HEAD_ATK1       504
#define S_HEAD_ATK2       505
#define S_HEAD_ATK3       506
#define S_HEAD_PAIN       507
#define S_HEAD_PAIN2      508
#define S_HEAD_PAIN3      509
#define S_HEAD_DIE1       510
#define S_HEAD_DIE2       511
#define S_HEAD_DIE3       512
#define S_HEAD_DIE4       513
#define S_HEAD_DIE5       514
#define S_HEAD_DIE6       515
#define S_HEAD_RAISE1     516
#define S_HEAD_RAISE2     517
#define S_HEAD_RAISE3     518
#define S_HEAD_RAISE4     519
#define S_HEAD_RAISE5     520
#define S_HEAD_RAISE6     521
#define S_BRBALL1         522
#define S_BRBALL2         523
#define S_BRBALLX1        524
#define S_BRBALLX2        525
#define S_BRBALLX3        526
#define S_BOSS_STND       527
#define S_BOSS_STND2      528
#define S_BOSS_RUN1       529
#define S_BOSS_RUN2       530
#define S_BOSS_RUN3       531
#define S_BOSS_RUN4       532
#define S_BOSS_RUN5       533
#define S_BOSS_RUN6       534
#define S_BOSS_RUN7       535
#define S_BOSS_RUN8       536
#define S_BOSS_ATK1       537
#define S_BOSS_ATK2       538
#define S_BOSS_ATK3       539
#define S_BOSS_PAIN       540
#define S_BOSS_PAIN2      541
#define S_BOSS_DIE1       542
#define S_BOSS_DIE2       543
#define S_BOSS_DIE3       544
#define S_BOSS_DIE4       545
#define S_BOSS_DIE5       546
#define S_BOSS_DIE6       547
#define S_BOSS_DIE7       548
#define S_BOSS_RAISE1     549
#define S_BOSS_RAISE2     550
#define S_BOSS_RAISE3     551
#define S_BOSS_RAISE4     552
#define S_BOSS_RAISE5     553
#define S_BOSS_RAISE6     554
#define S_BOSS_RAISE7     555
#define S_BOS2_STND       556
#define S_BOS2_STND2      557
#define S_BOS2_RUN1       558
#define S_BOS2_RUN2       559
#define S_BOS2_RUN3       560
#define S_BOS2_RUN4       561
#define S_BOS2_RUN5       562
#define S_BOS2_RUN6       563
#define S_BOS2_RUN7       564
#define S_BOS2_RUN8       565
#define S_BOS2_ATK1       566
#define S_BOS2_ATK2       567
#define S_BOS2_ATK3       568
#define S_BOS2_PAIN       569
#define S_BOS2_PAIN2      570
#define S_BOS2_DIE1       571
#define S_BOS2_DIE2       572
#define S_BOS2_DIE3       573
#define S_BOS2_DIE4       574
#define S_BOS2_DIE5       575
#define S_BOS2_DIE6       576
#define S_BOS2_DIE7       577
#define S_BOS2_RAISE1     578
#define S_BOS2_RAISE2     579
#define S_BOS2_RAISE3     580
#define S_BOS2_RAISE4     581
#define S_BOS2_RAISE5     582
#define S_BOS2_RAISE6     583
#define S_BOS2_RAISE7     584
#define S_SKULL_STND      585
#define S_SKULL_STND2     586
#define S_SKULL_RUN1      587
#define S_SKULL_RUN2      588
#define S_SKULL_ATK1      589
#define S_SKULL_ATK2      590
#define S_SKULL_ATK3      591
#define S_SKULL_ATK4      592
#define S_SKULL_PAIN      593
#define S_SKULL_PAIN2     594
#define S_SKULL_DIE1      595
#define S_SKULL_DIE2      596
#define S_SKULL_DIE3      597
#define S_SKULL_DIE4      598
#define S_SKULL_DIE5      599
#define S_SKULL_DIE6      600
#define S_SPID_STND       601
#define S_SPID_STND2      602
#define S_SPID_RUN1       603
#define S_SPID_RUN2       604
#define S_SPID_RUN3       605
#define S_SPID_RUN4       606
#define S_SPID_RUN5       607
#define S_SPID_RUN6       608
#define S_SPID_RUN7       609
#define S_SPID_RUN8       610
#define S_SPID_RUN9       611
#define S_SPID_RUN10      612
#define S_SPID_RUN11      613
#define S_SPID_RUN12      614
#define S_SPID_ATK1       615
#define S_SPID_ATK2       616
#define S_SPID_ATK3       617
#define S_SPID_ATK4       618
#define S_SPID_PAIN       619
#define S_SPID_PAIN2      620
#define S_SPID_DIE1       621
#define S_SPID_DIE2       622
#define S_SPID_DIE3       623
#define S_SPID_DIE4       624
#define S_SPID_DIE5       625
#define S_SPID_DIE6       626
#define S_SPID_DIE7       627
#define S_SPID_DIE8       628
#define S_SPID_DIE9       629
#define S_SPID_DIE10      630
#define S_SPID_DIE11      631
#define S_BSPI_STND       632
#define S_BSPI_STND2      633
#define S_BSPI_SIGHT      634
#define S_BSPI_RUN1       635
#define S_BSPI_RUN2       636
#define S_BSPI_RUN3       637
#define S_BSPI_RUN4       638
#define S_BSPI_RUN5       639
#define S_BSPI_RUN6       640
#define S_BSPI_RUN7       641
#define S_BSPI_RUN8       642
#define S_BSPI_RUN9       643
#define S_BSPI_RUN10      644
#define S_BSPI_RUN11      645
#define S_BSPI_RUN12      646
#define S_BSPI_ATK1       647
#define S_BSPI_ATK2       648
#define S_BSPI_ATK3       649
#define S_BSPI_ATK4       650
#define S_BSPI_PAIN       651
#define S_BSPI_PAIN2      652
#define S_BSPI_DIE1       653
#define S_BSPI_DIE2       654
#define S_BSPI_DIE3       655
#define S_BSPI_DIE4       656
#define S_BSPI_DIE5       657
#define S_BSPI_DIE6       658
#define S_BSPI_DIE7       659
#define S_BSPI_RAISE1     660
#define S_BSPI_RAISE2     661
#define S_BSPI_RAISE3     662
#define S_BSPI_RAISE4     663
#define S_BSPI_RAISE5     664
#define S_BSPI_RAISE6     665
#define S_BSPI_RAISE7     666
#define S_ARACH_PLAZ      667
#define S_ARACH_PLAZ2     668
#define S_ARACH_PLEX      669
#define S_ARACH_PLEX2     670
#define S_ARACH_PLEX3     671
#define S_ARACH_PLEX4     672
#define S_ARACH_PLEX5     673
#define S_CYBER_STND      674
#define S_CYBER_STND2     675
#define S_CYBER_RUN1      676
#define S_CYBER_RUN2      677
#define S_CYBER_RUN3      678
#define S_CYBER_RUN4      679
#define S_CYBER_RUN5      680
#define S_CYBER_RUN6      681
#define S_CYBER_RUN7      682
#define S_CYBER_RUN8      683
#define S_CYBER_ATK1      684
#define S_CYBER_ATK2      685
#define S_CYBER_ATK3      686
#define S_CYBER_ATK4      687
#define S_CYBER_ATK5      688
#define S_CYBER_ATK6      689
#define S_CYBER_PAIN      690
#define S_CYBER_DIE1      691
#define S_CYBER_DIE2      692
#define S_CYBER_DIE3      693
#define S_CYBER_DIE4      694
#define S_CYBER_DIE5      695
#define S_CYBER_DIE6      696
#define S_CYBER_DIE7      697
#define S_CYBER_DIE8      698
#define S_CYBER_DIE9      699
#define S_CYBER_DIE10     700
#define S_PAIN_STND       701
#define S_PAIN_RUN1       702
#define S_PAIN_RUN2       703
#define S_PAIN_RUN3       704
#define S_PAIN_RUN4       705
#define S_PAIN_RUN5       706
#define S_PAIN_RUN6       707
#define S_PAIN_ATK1       708
#define S_PAIN_ATK2       709
#define S_PAIN_ATK3       710
#define S_PAIN_ATK4       711
#define S_PAIN_PAIN       712
#define S_PAIN_PAIN2      713
#define S_PAIN_DIE1       714
#define S_PAIN_DIE2       715
#define S_PAIN_DIE3       716
#define S_PAIN_DIE4       717
#define S_PAIN_DIE5       718
#define S_PAIN_DIE6       719
#define S_PAIN_RAISE1     720
#define S_PAIN_RAISE2     721
#define S_PAIN_RAISE3     722
#define S_PAIN_RAISE4     723
#define S_PAIN_RAISE5     724
#define S_PAIN_RAISE6     725
#define S_SSWV_STND       726
#define S_SSWV_STND2      727
#define S_SSWV_RUN1       728
#define S_SSWV_RUN2       729
#define S_SSWV_RUN3       730
#define S_SSWV_RUN4       731
#define S_SSWV_RUN5       732
#define S_SSWV_RUN6       733
#define S_SSWV_RUN7       734
#define S_SSWV_RUN8       735
#define S_SSWV_ATK1       736
#define S_SSWV_ATK2       737
#define S_SSWV_ATK3       738
#define S_SSWV_ATK4       739
#define S_SSWV_ATK5       740
#define S_SSWV_ATK6       741
#define S_SSWV_PAIN       742
#define S_SSWV_PAIN2      743
#define S_SSWV_DIE1       744
#define S_SSWV_DIE2       745
#define S_SSWV_DIE3       746
#define S_SSWV_DIE4       747
#define S_SSWV_DIE5       748
#define S_SSWV_XDIE1      749
#define S_SSWV_XDIE2      750
#define S_SSWV_XDIE3      751
#define S_SSWV_XDIE4      752
#define S_SSWV_XDIE5      753
#define S_SSWV_XDIE6      754
#define S_SSWV_XDIE7      755
#define S_SSWV_XDIE8      756
#define S_SSWV_XDIE9      757
#define S_SSWV_RAISE1     758
#define S_SSWV_RAISE2     759
#define S_SSWV_RAISE3     760
#define S_SSWV_RAISE4     761
#define S_SSWV_RAISE5     762
#define S_KEENSTND        763
#define S_COMMKEEN        764
#define S_COMMKEEN2       765
#define S_COMMKEEN3       766
#define S_COMMKEEN4       767
#define S_COMMKEEN5       768
#define S_COMMKEEN6       769
#define S_COMMKEEN7       770
#define S_COMMKEEN8       771
#define S_COMMKEEN9       772
#define S_COMMKEEN10      773
#define S_COMMKEEN11      774
#define S_COMMKEEN12      775
#define S_KEENPAIN        776
#define S_KEENPAIN2       777
#define S_BRAIN           778
#define S_BRAIN_PAIN      779
#define S_BRAIN_DIE1      780
#define S_BRAIN_DIE2      781
#define S_BRAIN_DIE3      782
#define S_BRAIN_DIE4      783
#define S_BRAINEYE        784
#define S_BRAINEYESEE     785
#define S_BRAINEYE1       786
#define S_SPAWN1          787
#define S_SPAWN2          788
#define S_SPAWN3          789
#define S_SPAWN4          790
#define S_SPAWNFIRE1      791
#define S_SPAWNFIRE2      792
#define S_SPAWNFIRE3      793
#define S_SPAWNFIRE4      794
#define S_SPAWNFIRE5      795
#define S_SPAWNFIRE6      796
#define S_SPAWNFIRE7      797
#define S_SPAWNFIRE8      798
#define S_BRAINEXPLODE1   799
#define S_BRAINEXPLODE2   800
#define S_BRAINEXPLODE3   801
#define S_ARM1            802
#define S_ARM1A           803
#define S_ARM2            804
#define S_ARM2A           805
#define S_BAR1            806
#define S_BAR2            807
#define S_BEXP            808
#define S_BEXP2           809
#define S_BEXP3           810
#define S_BEXP4           811
#define S_BEXP5           812
#define S_BBAR1           813
#define S_BBAR2           814
#define S_BBAR3           815
#define S_BON1            816
#define S_BON1A           817
#define S_BON1B           818
#define S_BON1C           819
#define S_BON1D           820
#define S_BON1E           821
#define S_BON2            822
#define S_BON2A           823
#define S_BON2B           824
#define S_BON2C           825
#define S_BON2D           826
#define S_BON2E           827
#define S_BKEY            828
#define S_BKEY2           829
#define S_RKEY            830
#define S_RKEY2           831
#define S_YKEY            832
#define S_YKEY2           833
#define S_BSKULL          834
#define S_BSKULL2         835
#define S_RSKULL          836
#define S_RSKULL2         837
#define S_YSKULL          838
#define S_YSKULL2         839
#define S_STIM            840
#define S_MEDI            841
#define S_SOUL            842
#define S_SOUL2           843
#define S_SOUL3           844
#define S_SOUL4           845
#define S_SOUL5           846
#define S_SOUL6           847
#define S_PINV            848
#define S_PINV2           849
#define S_PINV3           850
#define S_PINV4           851
#define S_PSTR            852
#define S_PINS            853
#define S_PINS2           854
#define S_PINS3           855
#define S_PINS4           856
#define S_MEGA            857
#define S_MEGA2           858
#define S_MEGA3           859
#define S_MEGA4           860
#define S_SUIT            861
#define S_PMAP            862
#define S_PMAP2           863
#define S_PMAP3           864
#define S_PMAP4           865
#define S_PMAP5           866
#define S_PMAP6           867
#define S_PVIS            868
#define S_PVIS2           869
#define S_CLIP            870
#define S_AMMO            871
#define S_ROCK            872
#define S_BROK            873
#define S_CELL            874
#define S_CELP            875
#define S_SHEL            876
#define S_SBOX            877
#define S_BPAK            878
#define S_BFUG            879
#define S_MGUN            880
#define S_CSAW            881
#define S_LAUN            882
#define S_PLAS            883
#define S_SHOT            884
#define S_SHOT2           885
#define S_COLU            886
#define S_STALAG          887
#define S_BLOODYTWITCH    888
#define S_BLOODYTWITCH2   889
#define S_BLOODYTWITCH3   890
#define S_BLOODYTWITCH4   891
#define S_DEADTORSO       892
#define S_DEADBOTTOM      893
#define S_HEADSONSTICK    894
#define S_GIBS            895
#define S_HEADONASTICK    896
#define S_HEADCANDLES     897
#define S_HEADCANDLES2    898
#define S_DEADSTICK       899
#define S_LIVESTICK       900
#define S_LIVESTICK2      901
#define S_MEAT2           902
#define S_MEAT3           903
#define S_MEAT4           904
#define S_MEAT5           905
#define S_STALAGTITE      906
#define S_TALLGRNCOL      907
#define S_SHRTGRNCOL      908
#define S_TALLREDCOL      909
#define S_SHRTREDCOL      910
#define S_CANDLESTIK      911
#define S_CANDELABRA      912
#define S_SKULLCOL        913
#define S_TORCHTREE       914
#define S_BIGTREE         915
#define S_TECHPILLAR      916
#define S_EVILEYE         917
#define S_EVILEYE2        918
#define S_EVILEYE3        919
#define S_EVILEYE4        920
#define S_FLOATSKULL      921
#define S_FLOATSKULL2     922
#define S_FLOATSKULL3     923
#define S_HEARTCOL        924
#define S_HEARTCOL2       925
#define S_BLUETORCH       926
#define S_BLUETORCH2      927
#define S_BLUETORCH3      928
#define S_BLUETORCH4      929
#define S_GREENTORCH      930
#define S_GREENTORCH2     931
#define S_GREENTORCH3     932
#define S_GREENTORCH4     933
#define S_REDTORCH        934
#define S_REDTORCH2       935
#define S_REDTORCH3       936
#define S_REDTORCH4       937
#define S_BTORCHSHRT      938
#define S_BTORCHSHRT2     939
#define S_BTORCHSHRT3     940
#define S_BTORCHSHRT4     941
#define S_GTORCHSHRT      942
#define S_GTORCHSHRT2     943
#define S_GTORCHSHRT3     944
#define S_GTORCHSHRT4     945
#define S_RTORCHSHRT      946
#define S_RTORCHSHRT2     947
#define S_RTORCHSHRT3     948
#define S_RTORCHSHRT4     949
#define S_HANGNOGUTS      950
#define S_HANGBNOBRAIN    951
#define S_HANGTLOOKDN     952
#define S_HANGTSKULL      953
#define S_HANGTLOOKUP     954
#define S_HANGTNOBRAIN    955
#define S_COLONGIBS       956
#define S_SMALLPOOL       957
#define S_BRAINSTEM       958
#define S_TECHLAMP        959
#define S_TECHLAMP2       960
#define S_TECHLAMP3       961
#define S_TECHLAMP4       962
#define S_TECH2LAMP       963
#define S_TECH2LAMP2      964
#define S_TECH2LAMP3      965
#define S_TECH2LAMP4      966
#define NUMSTATES       967

typedef uint16_t statenum_t;

typedef struct state_s
{
  spritenum_t	sprite;
  spriteframenum_t	frame;
  int8_t			tics;
  // void		(*action) ();
  //actionf_t			action;
  ENEMYTHINKFUNCTION   action;
  statenum_t			nextstate; // todo i think this can be a int8_t diff that adds to current state.
  //int32_t			misc1, misc2;
} state_t;

//extern state_t* states;
#define size_states size_tantoangle + sizeof(state_t) * NUMSTATES

#define states ((state_t far*) (0x50000000 + size_tantoangle))


#define MT_PLAYER 0
#define MT_POSSESSED 1
#define MT_SHOTGUY 2
#define MT_VILE 3
#define MT_FIRE 4
#define MT_UNDEAD 5
#define MT_TRACER 6
#define MT_SMOKE 7
#define MT_FATSO 8
#define MT_FATSHOT 9
#define MT_CHAINGUY 10
#define MT_TROOP 11
#define MT_SERGEANT 12
#define MT_SHADOWS 13
#define MT_HEAD 14
#define MT_BRUISER 15
#define MT_BRUISERSHOT 16
#define MT_KNIGHT 17
#define MT_SKULL 18
#define MT_SPIDER 19
#define MT_BABY 20
#define MT_CYBORG 21
#define MT_PAIN 22
#define MT_WOLFSS 23
#define MT_KEEN 24
#define MT_BOSSBRAIN 25
#define MT_BOSSSPIT 26
#define MT_BOSSTARGET 27
#define MT_SPAWNSHOT 28
#define MT_SPAWNFIRE 29
#define MT_BARREL 30
#define MT_TROOPSHOT 31
#define MT_HEADSHOT 32
#define MT_ROCKET 33
#define MT_PLASMA 34
#define MT_BFG 35
#define MT_ARACHPLAZ 36
#define MT_PUFF 37
#define MT_BLOOD 38
#define MT_TFOG 39
#define MT_IFOG 40
#define MT_TELEPORTMAN 41
#define MT_EXTRABFG 42
#define MT_MISC0 43
#define MT_MISC1 44
#define MT_MISC2 45
#define MT_MISC3 46
#define MT_MISC4 47
#define MT_MISC5 48
#define MT_MISC6 49
#define MT_MISC7 50
#define MT_MISC8 51
#define MT_MISC9 52
#define MT_MISC10 53
#define MT_MISC11 54
#define MT_MISC12 55
#define MT_INV 56
#define MT_MISC13 57
#define MT_INS 58
#define MT_MISC14 59
#define MT_MISC15 60
#define MT_MISC16 61
#define MT_MEGA 62
#define MT_CLIP 63
#define MT_MISC17 64
#define MT_MISC18 65
#define MT_MISC19 66
#define MT_MISC20 67
#define MT_MISC21 68
#define MT_MISC22 69
#define MT_MISC23 70
#define MT_MISC24 71
#define MT_MISC25 72
#define MT_CHAINGUN 73
#define MT_MISC26 74
#define MT_MISC27 75
#define MT_MISC28 76
#define MT_SHOTGUN 77
#define MT_SUPERSHOTGUN 78
#define MT_MISC29 79
#define MT_MISC30 80
#define MT_MISC31 81
#define MT_MISC32 82
#define MT_MISC33 83
#define MT_MISC34 84
#define MT_MISC35 85
#define MT_MISC36 86
#define MT_MISC37 87
#define MT_MISC38 88
#define MT_MISC39 89
#define MT_MISC40 90
#define MT_MISC41 91
#define MT_MISC42 92
#define MT_MISC43 93
#define MT_MISC44 94
#define MT_MISC45 95
#define MT_MISC46 96
#define MT_MISC47 97
#define MT_MISC48 98
#define MT_MISC49 99
#define MT_MISC50 100
#define MT_MISC51 101
#define MT_MISC52 102
#define MT_MISC53 103
#define MT_MISC54 104
#define MT_MISC55 105
#define MT_MISC56 106
#define MT_MISC57 107
#define MT_MISC58 108
#define MT_MISC59 109
#define MT_MISC60 110
#define MT_MISC61 111
#define MT_MISC62 112
#define MT_MISC63 113
#define MT_MISC64 114
#define MT_MISC65 115
#define MT_MISC66 116
#define MT_MISC67 117
#define MT_MISC68 118
#define MT_MISC69 119
#define MT_MISC70 120
#define MT_MISC71 121
#define MT_MISC72 122
#define MT_MISC73 123
#define MT_MISC74 124
#define MT_MISC75 125
#define MT_MISC76 126
#define MT_MISC77 127
#define MT_MISC78 128
#define MT_MISC79 129
#define MT_MISC80 130
#define MT_MISC81 131
#define MT_MISC82 132
#define MT_MISC83 133
#define MT_MISC84 134
#define MT_MISC85 135
#define MT_MISC86 136
#define NUMMOBJTYPES 137

typedef uint8_t mobjtype_t;
typedef uint8_t sfxenum_t;


#define HIGHBIT 0x80
#define HIGHBITMASK 0x7F
#define MAKESPEED(x) x & HIGHBIT ? ((fixed_t)(x & HIGHBITMASK)) << FRACBITS : (fixed_t)x


 // most states and sfx fields could probably save space(?) in a getter. most of these arent oft used
typedef struct
{
    //int16_t	doomednum;  // moved into code in overlay
	statenum_t	spawnstate;	// pretty much  every value different, probably cant remove
	//int8_t	spawnhealth;  // can probably divide by 10 and store as int8_t and mult by 10 on init
	//statenum_t	seestate; // replaced with getter saved 80 bytes
	sfxenum_t	seesound;	// cant remove, net plus 80 bytes
    //int32_t	reactiontime;     always 8 except 0 for player. this can be abstracted in code
	//sfxenum_t	attacksound; // replaced with getter saved 80 bytes
	//statenum_t	painstate; // replaced with getter saved 96 bytes
    // uint16_t	painchance; // replaced with getter
	//sfxenum_t	painsound; // replaced with getter  saved 16 bytes
	// statenum_t	meleestate; 
	//statenum_t	missilestate;	// replaced with getter saved 96 bytes
	//statenum_t	deathstate; // replaced with getter  saved 16 bytes
	//statenum_t	xdeathstate;
	sfxenum_t	deathsound;	// cant remove, net plus 48 bytes
    uint8_t	speed;	// we're encoding this in a funky way to save space already - leave it alone
    uint8_t	radius; // too much variety to compact
    uint8_t	height;// too much variety to compact. low and high bit are unused.
    //int32_t	mass;
    //uint8_t	damage;	// replaced with getter saved 64 bytes
	// sfxenum_t	activesound;   replaced with getter  saved 48 bytes
    int32_t	flags;
	// statenum_t	raisestate;  replaced with getter

} mobjinfo_t;

#define size_thinkerlist				(sizeof(thinker_t) * MAX_THINKERS)
#define size_mobjinfo size_thinkerlist + sizeof(mobjinfo_t) * NUMMOBJTYPES

#define mobjinfo ((mobjinfo_t far *) (0x90000000 + size_thinkerlist))

 
extern int32_t getMobjMass(uint8_t id);
extern int16_t getPainChance(uint8_t id);
extern int16_t getRaiseState(uint8_t id);
extern int16_t getMeleeState(uint8_t id);
extern int16_t getXDeathState(uint8_t id);
extern sfxenum_t getActiveSound(uint8_t id);
extern sfxenum_t getPainSound(uint8_t id);
extern sfxenum_t getAttackSound(uint8_t id);
extern uint8_t getDamage(uint8_t id);
extern statenum_t getSeeState(uint8_t id);
extern statenum_t getMissileState(uint8_t id);
extern statenum_t getDeathState(uint8_t id);
extern statenum_t getPainState(uint8_t id);
extern int16_t getSpawnHealth(uint8_t id);

#endif
