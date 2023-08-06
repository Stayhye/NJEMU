/******************************************************************************

	zfile.c

	ZIP�t�@�C������֐�

******************************************************************************/

#ifndef ZFILE_H
#define ZFILE_H

#include <stdint.h>
#include "emucfg.h"

struct zip_find_t
{
	char name[MAX_PATH];
	uint32_t  length;
	uint32_t  crc32;
};

int zip_open(const char *path);
void zip_close(void);

int zip_findfirst(struct zip_find_t *file);
int zip_findnext(struct zip_find_t *file);

int zopen(const char *filename);
int zread(int fd, void *buf, unsigned size);
int zgetc(int fd);
int zclose(int fd);
int zsize(int fd);
#if (EMU_SYSTEM == NCDZ)
int zlength(const char *filename);
#endif

#endif /* ZFILE_H */
