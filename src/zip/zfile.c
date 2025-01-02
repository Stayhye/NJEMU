/******************************************************************************

	zfile.c

	ZIP�t�@�C������֐�

******************************************************************************/

#include <fcntl.h>
#include <limits.h>
#include "emumain.h"
#include "zip/unzip.h"


/******************************************************************************
	���[�J���ϐ�
******************************************************************************/

static unzFile unzfile = NULL;

static char basedir[PATH_MAX];
static char *basedirend;
static char zip_cache[4096];
static size_t  zip_cached_len;
static int  zip_filepos;

// TODO: FJTRUJY The scope of this functions are wrong as they are mixing the zip and file operations.
// Additionally we have required to use int64_t as file descriptor, to be compatible with 64-bit systems.


/******************************************************************************
	�O���[�o���֐�
******************************************************************************/

/*------------------------------------------------------
	ZIP�t�@�C�����J��
------------------------------------------------------*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvoid-pointer-to-int-cast"
int zip_open(const char *path)
{
	if (unzfile != NULL) zip_close();

	if ((unzfile = unzOpen(path)) != NULL)
		return (int)unzfile;

	strcpy(basedir, path);
	strcat(basedir, "/");
	basedirend = strrchr(basedir, '/') + 1;

	return -1;
}
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

/*------------------------------------------------------
	ZIP�t�@�C�������
------------------------------------------------------*/

void zip_close(void)
{
	if (unzfile)
	{
		unzClose(unzfile);
		unzfile = NULL;
	}
}


/*------------------------------------------------------
	ZIP�t�@�C�����̃t�@�C�������� (����)
------------------------------------------------------*/

int zip_findfirst(struct zip_find_t *file)
{
	if (unzfile)
	{
		if (unzGoToFirstFile(unzfile) == UNZ_OK)
		{
			unz_file_info info;

			unzGetCurrentFileInfo(unzfile, &info, file->name, PATH_MAX);
			file->length = info.uncompressed_size;
			file->crc32 = info.crc;
			return 1;
		}
	}
	return 0;
}


/*------------------------------------------------------
	ZIP�t�@�C�����̃t�@�C�������� (2��ڈȍ~)
------------------------------------------------------*/

int zip_findnext(struct zip_find_t *file)
{
	if (unzfile)
	{
		if (unzGoToNextFile(unzfile) == UNZ_OK)
		{
			unz_file_info info;

			unzGetCurrentFileInfo(unzfile, &info, file->name, PATH_MAX);
			file->length = info.uncompressed_size;
			file->crc32 = info.crc;
			return 1;
		}
	}
	return 0;
}


/*------------------------------------------------------
	ZIP�t�@�C�����̃t�@�C�����J��
------------------------------------------------------*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvoid-pointer-to-int-cast"
int64_t zopen(const char *filename)
{
	zip_cached_len = 0;

	if (unzfile == NULL)
	{
		int32_t fd;

		strcpy(basedirend, filename);
		fd = open(basedir, O_RDONLY, 0777);
		return (fd < 0) ? -1 : (long)fd;
	}

	if (unzLocateFile(unzfile, filename) == UNZ_OK)
		if (unzOpenCurrentFile(unzfile) == UNZ_OK)
			return (long)unzfile;

	return -1;
}
#pragma clang diagnostic pop
#pragma GCC diagnostic pop


/*------------------------------------------------------
	ZIP�t�@�C�����̃t�@�C�������
------------------------------------------------------*/

int zclose(int64_t fd)
{
	zip_cached_len = 0;

	if (unzfile == NULL)
	{
		if (fd != -1) close((int32_t)fd);
		return 0;
	}
	return unzCloseCurrentFile(unzfile);
}


/*------------------------------------------------------
	ZIP�t�@�C�����̃t�@�C����ǂݍ���
------------------------------------------------------*/

size_t zread(int64_t fd, void *buf, size_t size)
{
	if (unzfile == NULL)
		return read((int32_t)fd, buf, size);

	return unzReadCurrentFile(unzfile, buf, size);
}


/*------------------------------------------------------
	ZIP�t�@�C�����̃t�@�C������1�o�C�g�ǂݍ���
------------------------------------------------------*/

int zgetc(int64_t fd)
{
	if (zip_cached_len == 0)
	{
		if (unzfile == NULL)
			zip_cached_len = read((int32_t)fd, zip_cache, 4096);
		else
			zip_cached_len = unzReadCurrentFile(unzfile, zip_cache, 4096);
		if (zip_cached_len == 0) return EOF;
		zip_filepos = 0;
	}
	zip_cached_len--;
	return zip_cache[zip_filepos++] & 0xff;
}


/*------------------------------------------------------
	ZIP�t�@�C�����̃t�@�C���̃T�C�Y���擾
------------------------------------------------------*/

size_t zsize(int64_t fd)
{
	unz_file_info info;

	if (unzfile == NULL)
	{
        off_t len, pos = lseek((int32_t)fd, 0, SEEK_CUR);

		len = lseek((int32_t)fd, 0, SEEK_END);
		lseek((int32_t)fd, pos, SEEK_CUR);

		return len;
	}

	unzGetCurrentFileInfo(unzfile, &info, NULL, 0);

	return info.uncompressed_size;
}


/*------------------------------------------------------
	ZIP�t�@�C�����̃t�@�C���̃T�C�Y���擾
	(ZIP�t�@�C�����J�����Ƀt�@�C�����w��Ŏ擾)
------------------------------------------------------*/

#if (EMU_SYSTEM == NCDZ)
int zlength(const char *filename)
{
	int64_t fd;
	int length;

	if ((fd = zopen(filename)) != -1)
	{
		length = zsize(fd);
		zclose(fd);
		return length;
	}
	return -1;
}
#endif
