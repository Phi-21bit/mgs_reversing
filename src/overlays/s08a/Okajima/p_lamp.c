#include "libgv/libgv.h"
#include "libgcl/libgcl.h"
#include "Game/target.h"
#include "Game/game.h"
#include "Game/linkvarbuf.h"
#include "Okajima/spark.h"
#include "Anime/animeconv/anime.h"
#include "Bullet/blast.h"

typedef struct PLampWork
{
    GV_ACT   actor;
    DG_PRIM *prims[32];
    TARGET  *target;
    SVECTOR  verts[32];
    SVECTOR  center;
    SVECTOR  eye;
    char     pad1B4[0x8];
    int      field_1BC;
    int      field_1C0;
    int      field_1C4;
    int      field_1C8;
    int      field_1CC;
    char     pad1D0[4];
    int      where;
    int      field_1D8;
    char     pad1DC[0x18];
    int      proc_id;
    int      has_prims;
    GV_ACT  *cinema_screen;
} PLampWork;

#define EXEC_LEVEL 4

SVECTOR p_lamp_target_svec_800C353C = {5, 5, 5};
RECT    p_lamp_prim_rect_800C3544 = {100, 100, 200, 200};

GV_ACT *NewCinemaScreen_800DE434(int, int);
int     NewCinemaScreenClose_800DE4CC(GV_ACT *work);
GV_ACT *NewSpark2_800CA714(MATRIX *world);

extern int     GM_CurrentMap_800AB9B0;
extern int     GM_GameStatus_800AB3CC;
extern int     GM_GameOverTimer_800AB3D4;
extern int     GM_PadVibration_800ABA3C;
extern int     GM_PadVibration2_800ABA54;
extern MATRIX  DG_ZeroMatrix_8009D430;
extern int     GV_Time_800AB330;
extern DG_CHNL DG_Chanls_800B1800[3];

void PLampLookAt_800CC9F4(PLampWork *work, SVECTOR *eye, SVECTOR *center)
{
    if (work->field_1CC > 0)
    {
        work->field_1CC--;
    }
    else
    {
        work->field_1CC = GV_RandU_80017090(32) + 10;
    }

    eye->vx += rsin(GV_Time_800AB330 * 920) * work->field_1CC / 512;
    eye->vy += rsin(GV_Time_800AB330 * 822) * work->field_1CC / 512;
    eye->vz += rsin(GV_Time_800AB330 * 603) * work->field_1CC / 512;

    GM_PadVibration_800ABA3C = GV_RandU_80017090(2);
    GM_PadVibration2_800ABA54 = work->field_1CC * 255 / 42;
    DG_LookAt_800172D0(&DG_Chanls_800B1800[1], eye, center, 320);
}

void PLamp_800CCBA8(POLY_FT4 *poly, DG_TEX *tex, int r, int g, int b)
{
    int x, y, w, h;

    tex = DG_GetTexture_8001D830(GV_StrCode_80016CCC("rcm_l"));

    setPolyFT4(poly);
    setSemiTrans(poly, 1);
    setRGB0(poly, r, g, b);

    x = tex->off_x;
    w = tex->w;
    y = tex->off_y;
    h = tex->h;

    poly->u0 = x;
    poly->v0 = y;
    poly->u1 = x + w;
    poly->v1 = y;
    poly->u2 = x;
    poly->v2 = y + h;
    poly->u3 = x + w;
    poly->v3 = y + h;

    poly->tpage = tex->tpage;
    poly->clut = tex->clut;
}

void PLampFreePrims_800CCC6C(PLampWork *work, int count)
{
    int      i;
    DG_PRIM *prim;

    for (i = 0; i < count; i++)
    {
        prim = work->prims[i];
        if (prim != NULL)
        {
            DG_DequeuePrim_800182E0(prim);
            DG_FreePrim_8001BC04(prim);
        }
    }
}

void PLampDie_800CCCE0(PLampWork *work)
{
    if (work->cinema_screen)
    {
        GM_GameStatus_800AB3CC &= ~(GAME_FLAG_BIT_23 | GAME_FLAG_BIT_20 | GAME_FLAG_BIT_18);
        NewCinemaScreenClose_800DE4CC(work->cinema_screen);
    }

    if (work->has_prims == 1)
    {
        PLampFreePrims_800CCC6C(work, 32);
    }
}

// Duplicate of UjiGetSvecs_800C39E8
int PLampGetSvecs_800CCD44(char *opt, SVECTOR *svec)
{
    int   count;
    char *result;

    count = 0;

    while ((result = GCL_Get_Param_Result_80020AA4()) != NULL)
    {
        GCL_StrToSV_80020A14(result, svec);

        svec++;
        count++;
    }

    return count;
}

void s08a_p_lamp_800CCD98(PLampWork *work)
{
    int i;

    switch (work->field_1C0)
    {
    case 0:
        work->field_1C4 = 0;
        work->field_1C0 = 1;
        /* fallthrough */

    case 1:
        for (i = 15; i > 0; i--)
        {
            work->verts[i] = work->verts[i - 1];
        }

        work->field_1C4++;

        if (work->field_1C4 < 13)
        {
            work->verts[0].vx += 63;
        }
        else if (work->field_1C4 >= 29)
        {
            if (work->field_1C4 < 41)
            {
                work->verts[0].vx -= 63;
            }
            else if (work->field_1C4 >= 53)
            {
                work->field_1C4 = 0;
            }
        }
        break;
    }
}

void PLamp_800CCE6C(PLampWork *work)
{
    SVECTOR  svec1;
    SVECTOR  svec2;
    MATRIX   mat;
    DG_PRIM *prim;
    int      i;
    int      mult;

    switch (work->field_1C0)
    {
    case 0:
        if (GM_SnakeCurrentHealth == 0 || GM_GameOverTimer_800AB3D4 != 0)
        {
            break;
        }

        GM_GameStatus_800AB3CC |= GAME_FLAG_BIT_23 | GAME_FLAG_BIT_20 | GAME_FLAG_BIT_18;
        work->cinema_screen = NewCinemaScreen_800DE434(2000000000, 0);
        work->field_1C4 = 0;
        work->field_1C8 = 0;
        work->field_1C0 = 1;
        work->has_prims = 0;
        GM_GameStatus_800AB3CC |= GAME_FLAG_BIT_29;

        for (i = 0; i < 32; i++)
        {
            prim = work->prims[i];
            if (prim != NULL)
            {
                DG_DequeuePrim_800182E0(prim);
                DG_FreePrim_8001BC04(prim);
            }
        }

        work->center.vx = -4700;
        work->center.vy = 1000;
        work->center.vz = -13300;

    case 1:
        i = work->field_1C4++;
        if (i < 80)
        {
            svec1.vx = GV_RandU_80017090(4096);
            svec1.vy = GV_RandU_80017090(4096);
            svec1.vz = GV_RandU_80017090(4096);
            RotMatrix_gte(&svec1, &mat);
            mat.t[0] = GV_RandS_800170BC(512) - 4800;
            mat.t[1] = GV_RandU_80017090(2048) + 1000;
            mat.t[2] = -12300;
            if (i == 70)
            {
                GCL_ExecProc_8001FF2C(work->proc_id, NULL);
            }
        }
        else if (i < 240 && i % 6 == 0)
        {
            svec2.vx = GV_RandS_800170BC(512) - 4800;
            svec2.vy = GV_RandU_80017090(2048) + 1000;
            svec2.vz = GV_RandS_800170BC(512) - 12300;
            AN_Blast_Mini_8006E2A8(&svec2);
            switch (GV_RandU_80017090(4))
            {
            case 0:
                GM_SeSet2_80032968(0, 0x7F, 0xB2);
                break;
            case 1:
                GM_SeSet2_80032968(0, 0x7F, 0xB8);
                break;
            case 2:
                GM_SeSet2_80032968(0, 0x7F, 0xB9);
                break;
            case 3:
                GM_SeSet2_80032968(0, 0x7F, 0xBA);
                break;
            }
        }
        else if (i == 240)
        {
            svec2.vx = -4900;
            svec2.vy = 1000;
            svec2.vz = -12300;
            AN_Blast_Single_8006E224(&svec2);
            GM_SeSet2_80032968(0, 0x7F, 0xB1);
        }
        else if (i == 250 || i == 255 || i == 260 || i == 265)
        {
            svec2.vx = -5000;
            svec2.vy = 1500;
            svec2.vz = -12300;
            anime_create_8005E090(&svec2);
        }
        else if (i > 170)
        {
            work->field_1C0 = 2;
            work->field_1C4 = 0;
            GM_PadVibration2_800ABA54 = 0xFF;
        }

        svec1.vx = GV_RandU_80017090(4096);
        svec1.vy = GV_RandU_80017090(4096);
        svec1.vz = 0;
        RotMatrix(&svec1, &mat);
        mat = DG_ZeroMatrix_8009D430;
        if (i < 12)
        {
            work->center.vy += 200;
        }
        else if (i < 20)
        {
            work->center.vx += 200;
        }
        else if (i < 44)
        {
            work->center.vz += 200;
        }
        else if (i < 57)
        {
            work->center.vx += 200;
        }
        else if (i < 78)
        {
            work->center.vz += 200;
        }
        else if (i < 93)
        {
            work->center.vy -= 200;
        }
        else if (i < 94)
        {
            work->center.vx += 200;
        }

        if (i == 0)
        {
            work->eye.vx = -1379;
            work->eye.vy = 8064;
            work->eye.vz = -4220;
        }
        else if (i < 65)
        {
            PLampLookAt_800CC9F4(work, &work->eye, &work->center);
            if (!(i & 3))
            {
                GM_SeSet2_80032968(0, 140 - i, 0xB3);
            }
        }
        else if (i < 140)
        {
            mult = i - 65;
            work->eye.vx = (work->eye.vx * 15 + 1000) / 16;
            work->eye.vy = (work->eye.vy * 15 + 5000 - mult * 50) / 16;
            work->eye.vz = (work->eye.vz * 15 - 4000 + mult * 300) / 16;
            PLampLookAt_800CC9F4(work, &work->eye, &work->center);
            if (!(i & 3))
            {
                GM_SeSet2_80032968(0, 140 - i, 0xB3);
            }
        }
        else
        {
            work->eye.vx = 1645;
            work->eye.vy = 1000;
            work->eye.vz = -10000;
            svec2.vx = -4600;
            svec2.vy = 2000;
            svec2.vz = -12500;
            PLampLookAt_800CC9F4(work, &work->eye, &svec2);
        }

        if (i >= 11 && i < 140)
        {
            mat.t[0] = work->center.vx;
            mat.t[1] = work->center.vy;
            mat.t[2] = work->center.vz;
            if (!(i & 7))
            {
                NewSpark2_800CA714(&mat);
            }
            NewSpark_80074564(&mat, 0);
        }
        break;

    case 2:
        anime_create_8005E090(&work->center);
        mat = DG_ZeroMatrix_8009D430;
        mat.t[0] = work->center.vx;
        mat.t[1] = work->center.vy;
        mat.t[2] = work->center.vz;
        NewSpark2_800CA714(&mat);
        NewSpark2_800CA714(&mat);
        GM_SeSet2_80032968(0, 0x3F, 0xB5);
        work->field_1C0 = 3;
        GM_PadVibration_800ABA3C = 0;
        work->field_1CC = 128;

    case 3:
        if (work->field_1CC <= 0)
        {
            GV_DestroyActor_800151C8(&work->actor);
        }
        GM_PadVibration2_800ABA54 = work->field_1CC = work->field_1CC * 63 / 64 - 1;
        break;
    }
}

void PLamp_800CD570(PLampWork *work)
{
    switch (work->field_1BC)
    {
    case 0:
        s08a_p_lamp_800CCD98(work);
        break;
    case 1:
        PLamp_800CCE6C(work);
        break;
    }
}

void PLampAct_800CD5C0(PLampWork *work)
{
    GM_CurrentMap_800AB9B0 = work->where;
    PLamp_800CD570(work);
    if (work->field_1D8 == 0 && (work->target->damaged & TARGET_POWER))
    {
        GM_SeSet2_80032968(0, 0x7F, 0xB1);
        work->field_1D8 = 1;
        work->field_1BC = 1;
        work->field_1C0 = 0;
        GM_FreeTarget_8002D4B0(work->target);
    }
}

void PLampInitTarget_800CD640(PLampWork *work)
{
    SVECTOR svec;
    TARGET *target;

    target = GM_AllocTarget_8002D400();
    work->target = target;
    GM_SetTarget_8002DC74(target, 4, 0, &p_lamp_target_svec_800C353C);
    svec.vx = -4800;
    svec.vy = 1470;
    svec.vz = -13800;
    GM_MoveTarget_8002D500(target, &svec);
}

int PLampGetResources2_800CD6B0(PLampWork *work, int name, int map)
{
    work->field_1BC = 0;
    work->field_1C0 = 0;
    PLampInitTarget_800CD640(work);
    work->field_1D8 = 0;
    return 0;
}

int PLampGetResources_800CD6E4(PLampWork *work, int map, int n_verts)
{
    int      i;
    DG_PRIM *prim;
    DG_TEX  *tex;
    int      shade;
    char    *opt;

    work->has_prims = 0;
    work->where = map;

    GM_CurrentMap_800AB9B0 = map;

    for (i = 0; i < 16; i++)
    {
        if (i < n_verts)
        {
            work->verts[i + 16] = work->verts[i];
        }
        else
        {
            work->verts[i + 16] = work->verts[0];
        }
    }

    for (i = 1; i < 16; i++)
    {
        work->verts[i] = work->verts[0];
    }

    for (i = 0; i < 32; i++)
    {
        work->prims[i] = prim = DG_GetPrim(0x412, 1, 0, &work->verts[i], &p_lamp_prim_rect_800C3544);
        if (prim == NULL)
        {
            if (i != 0)
            {
                PLampFreePrims_800CCC6C(work, i);
            }

            return -1;
        }

        prim->field_2E_k500 = 1000;

        tex = DG_GetTexture_8001D830(GV_StrCode_80016CCC("rcm_l"));
        if (tex == NULL)
        {
            if (i != 0)
            {
                PLampFreePrims_800CCC6C(work, i - 1);
            }

            return -1;
        }

        if (i < 16)
        {
            shade = (16 - i) * 8;
            PLamp_800CCBA8(&prim->packs[0]->poly_ft4, tex, shade, shade, shade);
            PLamp_800CCBA8(&prim->packs[1]->poly_ft4, tex, shade, shade, shade);
        }
        else
        {
            PLamp_800CCBA8(&prim->packs[0]->poly_ft4, tex, 0, 72, 128);
            PLamp_800CCBA8(&prim->packs[1]->poly_ft4, tex, 0, 0, 128);
        }
    }

    opt = GCL_GetOption_80020968('e');
    if (opt != NULL)
    {
        work->proc_id = GCL_StrToInt_800209E8(opt);
    }

    work->field_1CC = 0;
    work->has_prims = 1;
    return 0;
}

GV_ACT *NewPLamp_800CD948(int name, int where)
{
    PLampWork *work;
    int        n_verts;

    work = (PLampWork *)GV_NewActor_800150E4(EXEC_LEVEL, sizeof(PLampWork));
    if (work != NULL)
    {
        GV_SetNamedActor_8001514C(&work->actor,
                                  (TActorFunction)PLampAct_800CD5C0,
                                  (TActorFunction)PLampDie_800CCCE0,
                                  "p_lamp.c");

        n_verts = PLampGetSvecs_800CCD44(GCL_GetOption_80020968('p'), work->verts);
        if (PLampGetResources_800CD6E4(work, where, n_verts) < 0)
        {
            GV_DestroyActor_800151C8(&work->actor);
            return NULL;
        }

        PLampGetResources2_800CD6B0(work, name, where);
    }

    return &work->actor;
}
