/******************************************************************************

	video.c

	PS2�r�f�I����֐�

******************************************************************************/

#include "emumain.h"

#include <stdlib.h>

typedef struct x86_64_video {
} x86_64_video_t;

/******************************************************************************
	�O���[�o���֐�
******************************************************************************/

static void x86_64_start(void *data) {
	x86_64_video_t *x86_64 = (x86_64_video_t*)data;

	ui_init();
}

static void *x86_64_init(void)
{
	x86_64_video_t *x86_64 = (x86_64_video_t*)calloc(1, sizeof(x86_64_video_t));

	x86_64_start(x86_64);
	return x86_64;
}


/*--------------------------------------------------------
	�r�f�I�����I��(����)
--------------------------------------------------------*/

static void x86_64_exit(x86_64_video_t *x86_64) {
}

static void x86_64_free(void *data)
{
	x86_64_video_t *x86_64 = (x86_64_video_t*)data;
	
	x86_64_exit(x86_64);
	free(x86_64);
}

/*--------------------------------------------------------
	�r�f�I���[�h�ݒ�
--------------------------------------------------------*/


static void x86_64_setMode(void *data, int mode)
{
	x86_64_video_t *x86_64 = (x86_64_video_t*)data;
#if VIDEO_32BPP
	if (video_mode != mode)
	{
		x86_64_exit(x86_64);
		video_mode = mode;

		x86_64_start(data);
	}
#endif
}

/*--------------------------------------------------------
	VSYNC��҂�
--------------------------------------------------------*/

static void x86_64_waitVsync(void *data)
{
}


/*--------------------------------------------------------
	�X�N���[�����t���b�v
--------------------------------------------------------*/

static void x86_64_flipScreen(void *data, bool vsync)
{
}


/*--------------------------------------------------------
	VRAM�̃A�h���X���擾
--------------------------------------------------------*/

static void *x86_64_frameAddr(void *data, void *frame, int x, int y)
{
	return NULL;
}

static void *x86_64_workFrame(void *data, enum WorkBuffer buffer)
{
	return NULL;
}


/*--------------------------------------------------------
	�`��/�\���t���[�����N���A
--------------------------------------------------------*/

static void x86_64_clearScreen(void *data)
{
}

/*--------------------------------------------------------
	�w�肵���t���[�����N���A
--------------------------------------------------------*/

static void x86_64_clearFrame(void *data, void *frame)
{
}


/*--------------------------------------------------------
	�w�肵���t���[����h��Ԃ�
--------------------------------------------------------*/

static void x86_64_fillFrame(void *data, void *frame, uint32_t color)
{
}


/*--------------------------------------------------------
	��`�͈͂��R�s�[
--------------------------------------------------------*/

static void x86_64_transferWorkFrame(void *data, RECT *src_rect, RECT *dst_rect)
{
}

static void x86_64_copyRect(void *data, void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
}


/*--------------------------------------------------------
	��`�͈͂����E���]���ăR�s�[
--------------------------------------------------------*/

static void x86_64_copyRectFlip(void *data, void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
}


/*--------------------------------------------------------
	��`�͈͂�270�x��]���ăR�s�[
--------------------------------------------------------*/

static void x86_64_copyRectRotate(void *data, void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
}


/*--------------------------------------------------------
	�e�N�X�`������`�͈͂��w�肵�ĕ`��
--------------------------------------------------------*/

static void x86_64_drawTexture(void *data, uint32_t src_fmt, uint32_t dst_fmt, void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
}

static void *x86_64_getNativeObjects(void *data, int index) {
	return NULL;
}

static void *x86_64_getTexture(void *data, enum WorkBuffer buffer) {
	return NULL;
}

static void x86_64_blitFinishFix(void *data, enum WorkBuffer buffer, void *clut, uint32_t vertices_count, void *vertices) {
}


video_driver_t video_x86_64 = {
	"x86_64",
	x86_64_init,
	x86_64_free,
	x86_64_setMode,
	x86_64_waitVsync,
	x86_64_flipScreen,
	x86_64_frameAddr,
	x86_64_workFrame,
	x86_64_clearScreen,
	x86_64_clearFrame,
	x86_64_fillFrame,
	x86_64_transferWorkFrame,
	x86_64_copyRect,
	x86_64_copyRectFlip,
	x86_64_copyRectRotate,
	x86_64_drawTexture,
	x86_64_getNativeObjects,
	x86_64_blitFinishFix,
};