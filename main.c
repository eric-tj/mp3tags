#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "mp3tag.h"

static int display_info(const char *fpath, const struct stat *sb,
                int tflag, struct FTW *ftwbuf)
{
        printf("%-3s %2d %7jd   %-40s %d %s\n",
                        (tflag == FTW_D) ?   "d"   : (tflag == FTW_DNR) ? "dnr" :
                        (tflag == FTW_DP) ?  "dp"  : (tflag == FTW_F) ?   "f" :
                        (tflag == FTW_NS) ?  "ns"  : (tflag == FTW_SL) ?  "sl" :
                        (tflag == FTW_SLN) ? "sln" : "???",
                        ftwbuf->level, (intmax_t) sb->st_size,
                        fpath, ftwbuf->base, fpath + ftwbuf->base);
        return 0;           /* To tell nftw() to continue */
}

static int __check_mp3_file(const char *fpath, const struct stat *sb,
                int tflag, struct FTW *ftwbuf)
{
	char *ptr, c = '.';

	if ((sb->st_mode & S_IFMT) != S_IFREG)
		return 0;
	ptr = strrchr(fpath + ftwbuf->base, c);
	if (!ptr)
		return 0;
	if (strncmp(ptr, ".mp3", 4))
		return 0;
        return 1;
}

static int process_file(const char *fpath, const struct stat *sb,
                int tflag, struct FTW *ftwbuf)
{
	struct ID3Info_s ID3Info;
	/* check whether mp3 file */
	if (__check_mp3_file(fpath, sb ,tflag, ftwbuf) == 0)
		return 0;

	/* extract mp3 tag info */
	if (readID3FromMP3(fpath, &ID3Info))
		return 0;
	printID3Tag(&ID3Info);

	/* insert or modify database */
	if (database_chk_ins_mp3info(&ID3Info, fpath))
		return -1;

	return 0;
}

int main(int argc, char *argv[])
{
        int flags = 0;
	int ret = EXIT_SUCCESS;

        if (argc > 2 && strchr(argv[2], 'd') != NULL)
                flags |= FTW_DEPTH;
        if (argc > 2 && strchr(argv[2], 'p') != NULL)
                flags |= FTW_PHYS;
	if (database_init()) {
		ret = EXIT_FAILURE;
		goto end;
	}

        if (nftw((argc < 2) ? "." : argv[1], process_file, 20, flags)
                        == -1) {
                perror("nftw");
		ret = EXIT_FAILURE;
		goto end;
        }
end:
	database_uninit();
        exit(ret);
}
