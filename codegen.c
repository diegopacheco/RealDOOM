
#include "m_memory.h"
#include "m_near.h"

#define CONSTANTS_COUNT 88

char* CONSTANTS[CONSTANTS_COUNT] = {
    "SECTORS_SEGMENT",
    "VERTEXES_SEGMENT",
    "SIDES_SEGMENT",
    "LINES_SEGMENT",
    "LINEFLAGSLIST_SEGMENT",
    "SEENLINES_SEGMENT",
    "SUBSECTORS_SEGMENT",
    "SUBSECTOR_LINES_SEGMENT",
    "NODES_SEGMENT",
    "NODE_CHILDREN_SEGMENT",
    "SEG_LINDEDEFS_SEGMENT",
    "SEG_SIDES_SEGMENT",


    "FINESINE_SEGMENT",
    "FINECOSINE_SEGMENT",
    "FINETANGENTINNER_SEGMENT",
    "STATES_SEGMENT",
    "EVENTS_SEGMENT",
    "FLATTRANSLATION_SEGMENT",
    "TEXTURETRANSLATION_SEGMENT",
    "TEXTUREHEIGHTS_SEGMENT",
    "SCANTOKEY_SEGMENT",
    "RNGTABLE_SEGMENT",
    "SPRITECACHE_NODES_SEGMENT",
    "FLATCACHE_NODES_SEGMENT",
    "PATCHCACHE_NODES_SEGMENT",
    "TEXTURECACHE_NODES_SEGMENT",

    "SEGS_PHYSICS_SEGMENT",
    "DISKGRAPHICBYTES_SEGMENT",



    "THINKERLIST_SEGMENT",
    "MOBJINFO_SEGMENT",
    "LINEBUFFER_SEGMENT",
    "SECTORS_PHYSICS_SEGMENT",
    "SECTORS_SOUNDORGS_SEGMENT",
    "SECTOR_SOUNDTRAVERSED_SEGMENT",

    "INTERCEPTS_SEGMENT",
    "AMMNUMPATCHBYTES_SEGMENT",
    "AMMNUMPATCHOFFSETS_SEGMENT",
    "DOOMEDNUM_SEGMENT",
    "LINESPECIALLIST_SEGMENT",


    "SCREEN0_SEGMENT",
    "GAMMATABLE_SEGMENT",
    "MENUOFFSETS_SEGMENT",

    "LINES_PHYSICS_SEGMENT",
    "BLOCKMAPLUMP_SEGMENT",

    "COLORMAPS_SEGMENT",
    "COLFUNC_JUMP_LOOKUP_SEGMENT",
    "DC_YL_LOOKUP_SEGMENT",
    "COLFUNC_FUNCTION_AREA_SEGMENT",
    "MOBJPOSLIST_SEGMENT",

    "MASKEDPOSTDATA_SEGMENT",
    "SPRITEPOSTDATASIZES_SEGMENT",
    "SPRITETOTALDATASIZES_SEGMENT",
    "MASKEDPOSTDATAOFS_SEGMENT",
    "MASKEDPIXELDATAOFS_SEGMENT",
    "DRAWFUZZCOL_AREA_SEGMENT",

    "CACHEDHEIGHT_SEGMENT",
    "YSLOPE_SEGMENT",
    "CACHEDDISTANCE_SEGMENT",
    "CACHEDXSTEP_SEGMENT",
    "CACHEDYSTEP_SEGMENT",
    "SPANSTART_SEGMENT",
    "DISTSCALE_SEGMENT",

    "OPENINGS_SEGMENT",
    "NEGONEARRAY_SEGMENT",
    "SCREENHEIGHTARRAY_SEGMENT",
    "FLOORCLIP_SEGMENT",
    "CEILINGCLIP_SEGMENT",

    "TEXTUREWIDTHMASKS_SEGMENT",
    "ZLIGHT_SEGMENT",
    "XTOVIEWANGLE_SEGMENT",
    "SPRITEOFFSETS_SEGMENT",
    "PATCHPAGE_SEGMENT",
    "PATCHOFFSET_SEGMENT",


    "SEGS_RENDER_SEGMENT",
    "SEG_NORMALANGLES_SEGMENT",
    "SIDES_RENDER_SEGMENT",
    "VISSPRITES_SEGMENT",
    "PLAYER_VISSPRITES_SEGMENT",
    "TEXTUREPATCHLUMP_OFFSET_SEGMENT",
    "VISPLANEHEADERS_SEGMENT",
    "VISPLANEPICLIGHTS_SEGMENT",
    "FUZZOFFSET_SEGMENT",
    "SCALELIGHTFIXED_SEGMENT",
    "SCALELIGHT_SEGMENT",
    "PATCH_SIZES_SEGMENT",
    "VIEWANGLETOX_SEGMENT",
    "FLATINDEX_SEGMENT",

    "SKYTEXTURE_TEXTURE_SEGMENT"



};

segment_t SEGMENTS[CONSTANTS_COUNT] = {
    sectors_segment,
    vertexes_segment, 
    sides_segment, 
    lines_segment, 
    lineflagslist_segment ,
    seenlines_segment, 
    subsectors_segment, 
    subsector_lines_segment, 
    nodes_segment, 
    node_children_segment, 
    seg_linedefs_segment, 
    seg_sides_segment,

    finesine_segment,
    finecosine_segment,
    finetangentinner_segment,
    states_segment,
    events_segment,
    flattranslation_segment,
    texturetranslation_segment,
    textureheights_segment,
    scantokey_segment,
    rndtable_segment,
    spritecache_nodes_segment,
    flatcache_nodes_segment,
    patchcache_nodes_segment,
    texturecache_nodes_segment,

    segs_physics_segment,
    diskgraphicbytes_segment,



    thinkerlist_segment,
    mobjinfo_segment,
    linebuffer_segment,
    sectors_physics_segment,
    sectors_soundorgs_segment,
    sector_soundtraversed_segment,
    intercepts_segment,
    ammnumpatchbytes_segment,
    ammnumpatchoffsets_segment,
    doomednum_segment,
    linespeciallist_segment,

    screen0_segment,
    gammatable_segment,
    menuoffsets_segment,

    lines_physics_segment,
    blockmaplump_segment,

    colormaps_segment,
    colfunc_jump_lookup_segment,
    dc_yl_lookup_segment,
    colfunc_function_area_segment,
    mobjposlist_segment,

    maskedpostdata_segment,
    spritepostdatasizes_segment,
    spritetotaldatasizes_segment,
    maskedpostdataofs_segment,
    maskedpixeldataofs_segment,
    drawfuzzcol_area_segment,


    cachedheight_segment,
    yslope_segment,
    cacheddistance_segment,
    cachedxstep_segment,
    cachedystep_segment,
    spanstart_segment,
    distscale_segment,

    openings_segment,
    negonearray_segment,
    screenheightarray_segment,
    floorclip_segment,
    ceilingclip_segment,

    texturewidthmasks_segment,
    zlight_segment,
    xtoviewangle_segment,
    spriteoffsets_segment,
    patchpage_segment,
    patchoffset_segment,



    segs_render_segment,
    seg_normalangles_segment,
    sides_render_segment,
    vissprites_segment,
    player_vissprites_segment,
    texturepatchlump_offset_segment,
    visplaneheaders_segment,
    visplanepiclights_segment,
    fuzzoffset_segment,
    scalelightfixed_segment,
    scalelight_segment,
    patch_sizes_segment,
    viewangletox_segment,
    flatindex_segment,

    skytexture_texture_segment
    
};

int16_t main ( int16_t argc,int8_t** argv )  { 
    
    // Export .inc file with segment values, etc from the c coe
    FILE* fp = fopen("constant.inc", "w");
    char* varname;
    segment_t segment;
    int16_t i;

    for (i = 0; i < CONSTANTS_COUNT; i++){
        varname = CONSTANTS[i];
        segment = SEGMENTS[i];
        fprintf(fp, "%s = 0%Xh\n", varname, segment);

    }

    fclose(fp);

    printf("Generated constant.inc file");
    
    return 0;
} 
