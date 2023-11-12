#include "Game/game.h"
#include "libdg/libdg.h"

typedef struct _RippleWork
{
    GV_ACT   actor;
    int      map;     //0x20
    DG_PRIM *prim;    //0x24
    SVECTOR  pos[32]; //0x28
    MATRIX   mat;     //0x128
    int      timer;   //0x148
} RippleWork;

extern MATRIX  DG_ZeroMatrix_8009D430;
extern SVECTOR DG_ZeroVector_800AB39C;

void s00a_ripple_800D7AC0( SVECTOR* pos, int n_vec, int scale )
{
    SVECTOR wave_pos[4];
    MATRIX mat;
    SVECTOR rot;

    setVector(&wave_pos[1], -0x393, 0, 0x834);
    setVector(&wave_pos[3],  0x393, 0, 0x834);
    setVector(&wave_pos[0], -0x393, 0, 0x6D4);
    setVector(&wave_pos[2],  0x393, 0, 0x6D4);

    {/* 輪の大きさを変更する */
        mat = DG_ZeroMatrix_8009D430 ;
        mat.m[0][0] = scale ;
        mat.m[1][1] = scale ;
        mat.m[2][2] = scale ;
        DG_SetPos_8001BC44( &mat );
        DG_RotVector_8001BE98( wave_pos, wave_pos, 4 );
    }

    rot = DG_ZeroVector_800AB39C ;
    while ( -- n_vec >= 0 ) {
        RotMatrixYXZ( &rot, &mat ) ;
        DG_SetPos_8001BC44( &mat ) ;
        
        /* 頂点データ生成 */
        DG_RotVector_8001BE98( wave_pos, pos, 4 ) ;
        
        pos += 4 ;
        rot.vy += 512 ;
    }
}

void RippleInitPacks_800D7C14( POLY_FT4 *packs0, POLY_FT4 *packs1, int n_packs, DG_TEX *tex )
{
    while ( -- n_packs >= 0 ) {
        setPolyFT4( packs0 ) ;
        setSemiTrans( packs0, 1 ) ;
        setPolyFT4( packs1 ) ;
        setSemiTrans( packs1, 1 ) ;
        DG_SetPacketTexture4( packs0, tex ) ;
        DG_SetPacketTexture4( packs1, tex ) ;
        packs0 ++ ;
        packs1 ++ ;
    }
}

void RippleShadePacks_800D7CEC( POLY_FT4 *packs0, POLY_FT4 *packs1, int n_packs, short bright )
{
    while ( -- n_packs >= 0 )
    {
        setRGB0( packs0, bright, bright, bright ) ;
        setRGB0( packs1, bright, bright, bright ) ;
        packs0++;
        packs1++;
    }
}

void RippleAct_800D7D2C( RippleWork *work )
{
    int x;
    int v0;
    short temp;
    DG_PRIM *prim;
    
    GM_SetCurrentMap( work->map );
    x = work->timer - 1;
    temp = x;

    work->timer = x;
    if ( temp <= 0 )
    {
        GV_DestroyActor_800151C8( &( work->actor ) );
        return;       
    }

    v0 = ( 80 - temp ) * 32;
    work->mat.m[0][0] = v0;
    work->mat.m[1][1] = v0;
    work->mat.m[2][2] = v0;
    DG_SetPos_8001BC44( &work->mat );
    DG_PutPrim_8001BE00( (MATRIX*)work->prim );
    prim = work->prim;
    RippleShadePacks_800D7CEC( &prim->field_40_pBuffers[0]->poly_ft4, &prim->field_40_pBuffers[1]->poly_ft4, 8, temp / 2 );
}

void RippleDie_800D7DDC( RippleWork *work )
{
    GM_FreePrim( work->prim );
}

int RippleGetResources_800D7E18( RippleWork *work, MATRIX* mat, int scale )
{
    DG_PRIM *prim;
    DG_TEX  *tex;

    work->map = GM_GetCurrentMap();

    s00a_ripple_800D7AC0( work->pos, 8, scale );
    prim = work->prim = DG_GetPrim( 0x12, 8, 0, work->pos, NULL );

    if ( prim == NULL )
    {
        return -1;
    }

    tex = DG_GetTexture_8001D830( GV_StrCode_80016CCC( "ripple" ) );
    
    if ( tex == NULL )
    {
        return -1;
    }

    RippleInitPacks_800D7C14( &prim->field_40_pBuffers[0]->poly_ft4, &prim->field_40_pBuffers[1]->poly_ft4, 8, tex );
    work->mat = *mat;
    work->timer = 64;

    return 0;
}

void* NewRipple_800D7F30( MATRIX* mat, int scale, int argc, char **argv )
{
    RippleWork *work ;

    work = (RippleWork *)GV_NewActor_800150E4( 5, sizeof( RippleWork ) ) ;
    if ( work != NULL ) {
        GV_SetNamedActor_8001514C( &( work->actor ), ( TActorFunction )RippleAct_800D7D2C, ( TActorFunction )RippleDie_800D7DDC, "ripple.c" );
        if ( RippleGetResources_800D7E18( work, mat, scale ) < 0 )
        {
            GV_DestroyActor_800151C8( &( work->actor ) );
            return NULL;
        }
    }
    return (void *)work ;
}
