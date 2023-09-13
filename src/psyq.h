#ifndef _PSYQ_H_
#define _PSYQ_H_

#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBSPU.H>
#include <LIBCD.H>
#include <LIBPAD.H>
#include <MEMORY.H>

// TODO: drop the addr suffixes for all psyq funcs in .s then just use psyq headers?

void                 SpuSetCommonAttr_80097038(SpuCommonAttr *attr);
extern unsigned long SpuSetReverbVoice_80096858(long on_off, unsigned long voice_bit);
extern void          SpuSetVoiceAttr_80097518(SpuVoiceAttr *arg);

DISPENV           *PutDispEnv_80090094(DISPENV *env);
MATRIX            *MulMatrix0_80092A48(MATRIX *m0, MATRIX *m1, MATRIX *m2);
MATRIX            *MulMatrix2_80092F68(MATRIX *m0, MATRIX *m1);
MATRIX            *MulRotMatrix_80092B58(MATRIX *m0);
MATRIX            *ScaleMatrix_800930D8(MATRIX *m, VECTOR *v);
MATRIX            *RotMatrixYXZ_80093798(SVECTOR *r, MATRIX *m);
MATRIX            *RotMatrixYXZ_gte_80094108(SVECTOR *r, MATRIX *m);
MATRIX            *RotMatrixZYX_gte_80093F08(SVECTOR *r, MATRIX *m);
MATRIX            *RotMatrixX_80093A28(long r, MATRIX *m);
MATRIX            *RotMatrixY_80093BC8(long r, MATRIX *m);
MATRIX            *RotMatrixZ_80093D68(long r, MATRIX *m);
VECTOR            *ApplyRotMatrix_80092DA8(SVECTOR *v0, VECTOR *v1);
VECTOR            *ApplyRotMatrixLV_80092E28(VECTOR *v0, VECTOR *v1);
VECTOR            *ApplyMatrixLV_80092C48(MATRIX *m, VECTOR *v0, VECTOR *v1);
SVECTOR           *ApplyMatrixSV_80093078(MATRIX *m, SVECTOR *v0, SVECTOR *v1);
long               RotTransPers3_800934A8(SVECTOR *vecs, SVECTOR *v1, SVECTOR *v2, long *sxy0, long *sxy1, long *sxy2, long *p, long *flag);
void               SetTransMatrix_80093248(MATRIX *);
char              *strcpy(char *dest, const char *src);
int                ChangeTh(int thread);
int                LoadImage2_80091FB0(RECT *, u_long *);
int                ResetCallback_80098318(void);
int                Square0_80093340(VECTOR *, VECTOR *);
int                SquareRoot0_80092708(int);
int                StoreImage2_8009209C(RECT *, u_long *);
int                StoreImage_8008FB70(RECT *, u_long *);
int                VSync_80098108(int);
void               VSyncCallback_800983A8(void (*func)());
int                rand(void);
void               srand(int s);
int                ratan2_80094308(int, int);
int                rcos_800925D8(int a1);
int                rsin_80092508(int a1);
int                sprintf(char *buffer, const char *format, ...);
int                strcmp(const char *str1, const char *str2);
long               EnableEvent(long);
long               OpenEvent(unsigned long desc, long spec, long mode, void (*func)());
long               OpenTh(long (*func)(), unsigned long, unsigned long);
long               SetConf_800997BC(unsigned long, unsigned long, unsigned long);
long               StartCARD_800990F8(void);
long               TestEvent(long);
u_long            *ClearOTagR_8008FD50(u_long *ot, int n);
unsigned int       GetRCnt_800996E8(unsigned int rcnt);
unsigned short     GetTPage_80092418(int tp, int abr, int x, int y);
void               SetDrawTPage_800924A8(DR_TPAGE *p, int dfe, int dtd, int tpage);
void               SetDrawStp_800924D8(DR_STP *p, int pbw);
void               SetTexWindow_800905F0(DR_TWIN *p, RECT *tw);
void               CloseEvent(int event);
void               DrawOTag_8008FE58(unsigned int *pOt);
void               DrawPrim_8008FDFC(void *p);
void               DrawSyncCallback_8008F76C(void *);
void               EnterCriticalSection(void);
void               ExitCriticalSection(void);
void               InitCARD_8009908C(long val);
void               LoadImage_8008FB10(RECT *, unsigned char *);
void               OuterProduct12_800933AC(VECTOR *v0, VECTOR *v1, VECTOR *v2);
void               PutDrawEnv_8008FEC8(DRAWENV *);
void               ReadRotMatrix_80092DD8(MATRIX *m);
void               SetDrawEnv_800906B0(DR_ENV *dr_env, DRAWENV *env);
void               SetMem(int);
void               SetRotMatrix_80093218(MATRIX *m);
void               StopCARD_80099130(void);
void               SwEnterCriticalSection(void);
void               SwExitCriticalSection(void);
void               VectorNormal_80092838(VECTOR *v0, VECTOR *v1);
long               VectorNormalSS_80092868(SVECTOR *v0, SVECTOR *v1);
//void              *memset_8008E688(void *pSrc, int value, int len);
void               SetDispMask_8008F7CC(int);
void               InitGeom_80092680(void);
void               PadInitDirect_8009C6CC(unsigned char *pad1, unsigned char *pad2);
void               PadSetAct_8009A678(int port, unsigned char *data, int len);
void               PadStartCom_8009A22C(void);
void               StopPAD_80099F08(void);
void               ChangeClearPAD(long val);
int                erase(char *);
long               card_read_8009901C(long chan, long block, unsigned char *buf);
void               bu_init_80098FEC(void);
int                ResetGraph(int mode);
void               SpuSetKey_80096C18(int, int);
long               RotTransPers_80093478(SVECTOR *v0, long *sxy, long *p, long *flag);
long               LoadExec(char *, unsigned long, unsigned long);
void               SpuGetAllKeysStatus_8009748C(char *status);
void               _96_init(void);
void               _96_remove(void);
void               StopCallback_8009840C(void);
long               SpuSetIRQ_80096A28(long on_off);
void               StrFadeIn_800822C8(unsigned int a1);
int                StrFadeOutStop_80082380(unsigned int fadeSpeed);
int                SePlay_800888F8(int a1);
int                StrFadeOut_80082310(unsigned int a1);
int                CdInit(void);
void               SpuInit_80094568(void);
void               PadStopCom_8009A24C(void);
long               SpuGetKeyStatus_80096DD8(unsigned long voice_bit);
unsigned long      SpuSetIRQAddr_80096B68(unsigned long);
unsigned long      SpuSetTransferStartAddr_80096EC8(unsigned long addr);
long               SpuInitMalloc_800952D8(long num, char *top);
unsigned long      SpuSetPitchLFOVoice(long on_off, unsigned long voice_bit);
long               SpuMalloc_80095338(long size);
unsigned long      SpuWrite_80096E68(unsigned char *addr, unsigned long size);
unsigned long      SpuSetPitchLFOVoice_80097008(long on_off, unsigned long voice_bit);
unsigned long      SpuSetNoiseVoice_80095908(long on_off, unsigned long voice_bit);
long               SpuSetReverb_80095BF8(long on_off);
long               SpuSetTransferMode_80096F28(long mode);
long               SpuReserveReverbWorkArea_80096788(long on_off);
long               SpuClearReverbWorkArea_80096888(long mode);
SpuIRQCallbackProc SpuSetIRQCallback_80096BA8(SpuIRQCallbackProc);
long               SpuSetReverbModeParam_80095DD8(SpuReverbAttr *attr);
long               SpuSetReverbDepth_800967D8(SpuReverbAttr *attr);
void               SpuGetVoiceAttr_80097D18(SpuVoiceAttr *arg);
int                MoveImage_8008FBD0(RECT *rect, int x, int y);
void               SpuQuit_80095258(void);
int                strncmp(const char *s1, const char *s2, int size);
void               SetSemiTrans_80092458(void *p, int abe);
void               SetPolyF4_80092488(void *p);
unsigned long      GetSp(void);
unsigned long      GetGp(void);
int                PadInfoAct_8009A47C(int port, int actno, int term);
int                PadSetActAlign_8009A5F8(int port, char *data);
int                PadGetState_8009A2B8(int port);
int                open(char *devname, int flag);
int                close(int fd);
unsigned long      lseek(int fd, unsigned int offset, int flag);
int                write(int fd, char* buf, int n);
long               format(char *fs);
struct DIRENTRY   *firstfile_80099AEC(char *, struct DIRENTRY *);
struct DIRENTRY   *nextfile(struct DIRENTRY *);
long               CloseTh(unsigned long thread);
long               card_info_80098FFC(long chan);
long               SpuIsTransferCompleted_80096F58(long flag);
int                PCinit_80014B14(void);
int                PCopen_80014B1C(char *name, int flags, int perms);
int                PClseek_80098E48(int fd, int offset, int mode);
int                PCread_80014B24(int fd, char *buff, int len);
int                PCclose_80014B2C(int fd);
long               card_clear_8009902C(long chan);
long               card_load_8009900C(long chan);

#endif // _PSYQ_H_
