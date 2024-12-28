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
	size_t  length;
	uint64_t  crc32;
};

int zip_open(const char *path);
void zip_close(void);

int zip_findfirst(struct zip_find_t *file);
int zip_findnext(struct zip_find_t *file);

int64_t zopen(const char *filename);
size_t zread(int64_t fd, void *buf, size_t size);
int zgetc(int64_t fd);
int zclose(int64_t fd);
size_t zsize(int64_t fd);
#if (EMU_SYSTEM == NCDZ)
int zlength(const char *filename);
#endif

#endif /* ZFILE_H */
