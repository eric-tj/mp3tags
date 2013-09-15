#ifndef __DATABASE_H
#define __DATABASE_H

#include "mp3tag.h"

int database_init();
int database_uninit();
int database_chk_ins_mp3info(struct ID3Info_s *ID3Info, const char *path);

#endif
