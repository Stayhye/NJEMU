/******************************************************************************

	video.c

	PS2�r�f�I����֐�

******************************************************************************/

#include "emumain.h"

#include <stdlib.h>
#include <SDL.h>

#define OUTPUT_WIDTH 352
#define OUTPUT_HEIGHT 280

typedef struct x86_64_video {
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool draw_extra_info;
	SDL_BlendMode blendMode;

	// Original buffers containing clut indexes
	uint8_t *scrbitmap;
	uint8_t *tex_spr;
	uint8_t *tex_spr0;
	uint8_t *tex_spr1;
	uint8_t *tex_spr2;
	uint8_t *tex_fix;

	SDL_Texture *sdl_texture_scrbitmap;
	SDL_Texture *sdl_texture_tex_spr0;
	SDL_Texture *sdl_texture_tex_spr1;
	SDL_Texture *sdl_texture_tex_spr2;
	SDL_Texture *sdl_texture_tex_fix;
} x86_64_video_t;

/******************************************************************************
	�O���[�o���֐�
******************************************************************************/

static void x86_64_start(void *data) {
	x86_64_video_t *x86_64 = (x86_64_video_t*)data;

	// Original buffers containing clut indexes
	size_t scrbitmapSize = BUF_WIDTH * SCR_HEIGHT;
	size_t textureSize = BUF_WIDTH * TEXTURE_HEIGHT;
	x86_64->scrbitmap = (uint8_t*)malloc(scrbitmapSize);
	uint8_t *tex_spr = (uint8_t*)malloc(textureSize * 3);
	x86_64->tex_spr = tex_spr;
	x86_64->tex_spr0 = tex_spr;
	x86_64->tex_spr1 = tex_spr + textureSize;
	x86_64->tex_spr2 = tex_spr + textureSize * 2;
	x86_64->tex_fix = (uint8_t*)malloc(textureSize);

	// Create SDL textures
	x86_64->sdl_texture_scrbitmap = SDL_CreateTexture(x86_64->renderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, BUF_WIDTH, SCR_HEIGHT);
	if (x86_64->sdl_texture_scrbitmap == NULL) {
		printf("Could not create sdl_texture_scrbitmap: %s\n", SDL_GetError());
		return;
	}	
	x86_64->sdl_texture_tex_spr0 = SDL_CreateTexture(x86_64->renderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, BUF_WIDTH, TEXTURE_HEIGHT);
	if (x86_64->sdl_texture_tex_spr0 == NULL) {
		printf("Could not create sdl_texture_tex_spr0: %s\n", SDL_GetError());
		return;
	}
	x86_64->sdl_texture_tex_spr1 = SDL_CreateTexture(x86_64->renderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, BUF_WIDTH, TEXTURE_HEIGHT);
	if (x86_64->sdl_texture_tex_spr1 == NULL) {
		printf("Could not create sdl_texture_tex_spr1: %s\n", SDL_GetError());
		return;
	}

	x86_64->sdl_texture_tex_spr2 = SDL_CreateTexture(x86_64->renderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, BUF_WIDTH, TEXTURE_HEIGHT);
	if (x86_64->sdl_texture_tex_spr2 == NULL) {
		printf("Could not create sdl_texture_tex_spr2: %s\n", SDL_GetError());
		return;
	}

	x86_64->sdl_texture_tex_fix = SDL_CreateTexture(x86_64->renderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, BUF_WIDTH, SCR_HEIGHT);
	if (x86_64->sdl_texture_tex_fix == NULL) {
		printf("Could not create sdl_texture_tex_fix: %s\n", SDL_GetError());
		return;
	}

	SDL_SetTextureBlendMode(x86_64->sdl_texture_scrbitmap, x86_64->blendMode);
	SDL_SetTextureBlendMode(x86_64->sdl_texture_tex_spr0, x86_64->blendMode);
	SDL_SetTextureBlendMode(x86_64->sdl_texture_tex_spr1, x86_64->blendMode);
	SDL_SetTextureBlendMode(x86_64->sdl_texture_tex_spr2, x86_64->blendMode);
	SDL_SetTextureBlendMode(x86_64->sdl_texture_tex_fix, x86_64->blendMode);

	ui_init();
}

static void *x86_64_init(void)
{
	uint32_t windows_width, windows_height;
	x86_64_video_t *x86_64 = (x86_64_video_t*)calloc(1, sizeof(x86_64_video_t));
	x86_64->draw_extra_info = false;

	// Create a window (width, height, window title)
	char title[256];
	sprintf(title, "%s %s", APPNAME_STR, VERSION_STR);
	windows_width = x86_64->draw_extra_info ? BUF_WIDTH * 2 : OUTPUT_WIDTH;
	windows_height = x86_64->draw_extra_info ? TEXTURE_HEIGHT * 2 : OUTPUT_HEIGHT;


    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windows_width, windows_height, SDL_WINDOW_SHOWN);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		free(x86_64);
		return NULL;
	}

	x86_64->window = window;

	// Create a renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Check that the renderer was successfully created
	if (renderer == NULL) {
		// In the case that the renderer could not be made...
		printf("Could not create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(x86_64->window);
		free(x86_64);
		return NULL;
	}

	x86_64->renderer = renderer;

	x86_64->blendMode = SDL_ComposeCustomBlendMode(
		SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, 
		SDL_BLENDFACTOR_SRC_ALPHA, 
		SDL_BLENDOPERATION_ADD, 
		SDL_BLENDFACTOR_ZERO, 
		SDL_BLENDFACTOR_ZERO, 
		SDL_BLENDOPERATION_ADD
	);

	x86_64_start(x86_64);
	return x86_64;
}


/*--------------------------------------------------------
	�r�f�I�����I��(����)
--------------------------------------------------------*/

static void x86_64_exit(x86_64_video_t *x86_64) {
	if (x86_64->sdl_texture_scrbitmap) {
		SDL_DestroyTexture(x86_64->sdl_texture_scrbitmap);
		x86_64->sdl_texture_scrbitmap = NULL;
	}

	if (x86_64->sdl_texture_tex_spr0) {
		SDL_DestroyTexture(x86_64->sdl_texture_tex_spr0);
		x86_64->sdl_texture_tex_spr0 = NULL;
	}

	if (x86_64->sdl_texture_tex_spr1) {
		SDL_DestroyTexture(x86_64->sdl_texture_tex_spr1);
		x86_64->sdl_texture_tex_spr1 = NULL;
	}

	if (x86_64->sdl_texture_tex_spr2) {
		SDL_DestroyTexture(x86_64->sdl_texture_tex_spr2);
		x86_64->sdl_texture_tex_spr2 = NULL;
	}

	if (x86_64->sdl_texture_tex_fix) {
		SDL_DestroyTexture(x86_64->sdl_texture_tex_fix);
		x86_64->sdl_texture_tex_fix = NULL;
	}

	if (x86_64->scrbitmap) {
		free(x86_64->scrbitmap);
		x86_64->scrbitmap = NULL;
	}

	if (x86_64->tex_spr) {
		free(x86_64->tex_spr);
		x86_64->tex_spr = NULL;
		x86_64->tex_spr0 = NULL;
		x86_64->tex_spr1 = NULL;
		x86_64->tex_spr2 = NULL;
	}

	if (x86_64->tex_fix) {
		free(x86_64->tex_fix);
		x86_64->tex_fix = NULL;
	}
}

static void x86_64_free(void *data)
{
	x86_64_video_t *x86_64 = (x86_64_video_t*)data;

	SDL_DestroyRenderer(x86_64->renderer);
	SDL_DestroyWindow(x86_64->window);
	
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
	x86_64_video_t *x86_64 = (x86_64_video_t*)data;
	SDL_RenderPresent(x86_64->renderer);
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
	x86_64_video_t *x86_64 = (x86_64_video_t*)data;
	switch (buffer) {
		case SCRBITMAP:
			return x86_64->scrbitmap;
		case TEX_SPR0:
			return x86_64->tex_spr0;
		case TEX_SPR1:
			return x86_64->tex_spr1;
		case TEX_SPR2:
			return x86_64->tex_spr2;
		case TEX_FIX:
			return x86_64->tex_fix;
		default:
			return NULL;
	}
}


/*--------------------------------------------------------
	�`��/�\���t���[�����N���A
--------------------------------------------------------*/

static void x86_64_clearScreen(void *data)
{
	x86_64_video_t *x86_64 = (x86_64_video_t*)data;
	SDL_RenderClear(x86_64->renderer);
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
	x86_64_video_t *x86_64 = (x86_64_video_t*)data;

	if (!x86_64->draw_extra_info) {
		return;
	}

	// Let's print the SPR0, SPR1 SPR2 and FIX in the empty space of the screen (right size 0.5 scale)
	SDL_Rect dst_rect_spr0 = { BUF_WIDTH, 0, BUF_WIDTH / 2, TEXTURE_HEIGHT / 2 };
	SDL_Rect dst_rect_spr0_border = { dst_rect_spr0.x - 1, dst_rect_spr0.y - 1, dst_rect_spr0.w + 2, dst_rect_spr0.h + 2 };
	SDL_Rect dst_rect_spr1 = { BUF_WIDTH, dst_rect_spr0.y + dst_rect_spr0.h + 20, BUF_WIDTH / 2, TEXTURE_HEIGHT / 2 };
	SDL_Rect dst_rect_spr1_border = { dst_rect_spr1.x - 1, dst_rect_spr1.y - 1, dst_rect_spr1.w + 2, dst_rect_spr1.h + 2 };
	SDL_Rect dst_rect_spr2 = { BUF_WIDTH, dst_rect_spr1.y + dst_rect_spr1.h + 20, BUF_WIDTH / 2, TEXTURE_HEIGHT / 2 };
	SDL_Rect dst_rect_spr2_border = { dst_rect_spr2.x - 1, dst_rect_spr2.y - 1, dst_rect_spr2.w + 2, dst_rect_spr2.h + 2 };
	SDL_Rect dst_rect_fix = { BUF_WIDTH, dst_rect_spr2.y + dst_rect_spr2.h + 20, BUF_WIDTH / 2, SCR_HEIGHT / 2 };
	SDL_Rect dst_rect_fix_border = { dst_rect_fix.x - 1, dst_rect_fix.y - 1, dst_rect_fix.w + 2, dst_rect_fix.h + 2 };

	SDL_SetRenderDrawColor(x86_64->renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(x86_64->renderer, &dst_rect_spr0_border);
	SDL_RenderFillRect(x86_64->renderer, &dst_rect_spr1_border);
	SDL_RenderFillRect(x86_64->renderer, &dst_rect_spr2_border);
	SDL_RenderFillRect(x86_64->renderer, &dst_rect_fix_border);
	SDL_SetRenderDrawColor(x86_64->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(x86_64->renderer, &dst_rect_spr0);
	SDL_RenderFillRect(x86_64->renderer, &dst_rect_spr1);
	SDL_RenderFillRect(x86_64->renderer, &dst_rect_spr2);
	SDL_RenderFillRect(x86_64->renderer, &dst_rect_fix);

	SDL_RenderCopy(x86_64->renderer, x86_64->sdl_texture_tex_spr0, NULL, &dst_rect_spr0);
	SDL_RenderCopy(x86_64->renderer, x86_64->sdl_texture_tex_spr1, NULL, &dst_rect_spr1);
	SDL_RenderCopy(x86_64->renderer, x86_64->sdl_texture_tex_spr2, NULL, &dst_rect_spr2);	
	SDL_RenderCopy(x86_64->renderer, x86_64->sdl_texture_tex_fix, NULL, &dst_rect_fix);

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

static SDL_Texture *x86_64_getTexture(void *data, enum WorkBuffer buffer) {
	x86_64_video_t *x86_64 = (x86_64_video_t*)data;
	switch (buffer) {
		case SCRBITMAP:
			return x86_64->sdl_texture_scrbitmap;
		case TEX_SPR0:
			return x86_64->sdl_texture_tex_spr0;
		case TEX_SPR1:
			return x86_64->sdl_texture_tex_spr1;
		case TEX_SPR2:
			return x86_64->sdl_texture_tex_spr2;
		case TEX_FIX:
			return x86_64->sdl_texture_tex_fix;
		default:
			return NULL;
	}
}

static void x86_64_blitTexture(void *data, enum WorkBuffer buffer, void *clut, uint32_t vertices_count, void *vertices) {
	// We need to transform the texutres saved that uses clut into a SDL texture compatible format
	SDL_Point size;
	x86_64_video_t *x86_64 = (x86_64_video_t *)data;
	struct Vertex *vertexs = (struct Vertex *)vertices;
	uint16_t *clut_texture = (uint16_t *)clut;
	uint8_t *tex_fix = x86_64_workFrame(data, buffer);
	SDL_Texture *texture = x86_64_getTexture(data, buffer);
	SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

	// Lock texture
	void *pixels;
	int pitch;
	SDL_LockTexture(texture, NULL, &pixels, &pitch);

	// Obtain the color from the clut using the index and copy it to the pixels array
	for (int i = 0; i < size.y; ++i) {
		for (int j = 0; j < size.x; ++j) {
			int index = i * size.x + j;
			uint8_t pixelValue = tex_fix[index];
			uint16_t color = clut_texture[pixelValue];

			uint16_t *pixel = (uint16_t*)pixels + index;
			*pixel = color;
		}
	}

	// Unlock texture
	SDL_UnlockTexture(texture);

	// Render Geometry expect to receive a SDL_Vertex array and it is using triangles
	// however x86_64_blitTexture receives a SDL_Vertex array using 2 vertex per sprite
	// so we are going to use SDL_RenderCopy to render all the sprites
	for (int i = 0; i < vertices_count; i += 2) {
		struct Vertex *vertex1 = &vertexs[i];
		struct Vertex *vertex2 = &vertexs[i + 1];

		SDL_Rect dst_rect = { 
			vertex1->x,
			vertex1->y,
			vertex2->x - vertex1->x,
			vertex2->y - vertex1->y
		};
		SDL_Rect src_rect = { 
			vertex1->u,
			vertex1->v,
			vertex2->u - vertex1->u,
			vertex2->v - vertex1->v
		};

		// Render the sprite
		SDL_RenderCopy(x86_64->renderer, texture, &src_rect, &dst_rect);
	}
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
	x86_64_blitTexture,
};