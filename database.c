#include <my_global.h>
#include <mysql.h>
#include "database.h"

void __finish_with_error(MYSQL *con)
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
}

static MYSQL *con;
#define DATABASE_NAME	"test"
#define TABLE_NAME	"mp3_info"
#define HOST_NAME	"localhost"
#define USER_NAME	"root"
#define PASSWORD	"eric"

#define FIELD_FILE_PATH	"file_path"

int database_init()
{
	con = mysql_init(NULL);
	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		return -1;
	}
	if (mysql_real_connect(con, HOST_NAME, USER_NAME, PASSWORD,
			       DATABASE_NAME, 0 , NULL, 0) == NULL)
	{
		__finish_with_error(con);
		return -1;
	}
	return 0;
}

int database_uninit()
{
	mysql_close(con);
	return 0;
}
#define MAX_QUERY_SIZE	8192
static char sql_query_buf[MAX_QUERY_SIZE];

int __ins_mp3info(struct ID3Info_s *ID3Info, const char *path)
{
	const char *title, *artist, *album, *year;

	title = ID3Info->songTitle;
	artist = ID3Info->artist;
	album = ID3Info->album;
	year = ID3Info->year;

	snprintf(sql_query_buf, MAX_QUERY_SIZE,
		 "INSERT INTO %s VALUES\(\"%s\", NULL, "
		 "\"%s\", \"%s\", \"%s\", \"%s\", "
		 "NULL, NULL, NULL)",
		 TABLE_NAME, path,
		 title, artist, album, year);
	printf("%s\n", sql_query_buf);
	if (mysql_query(con, sql_query_buf)) {
		__finish_with_error(con);
		return -1;
	}

}

int database_chk_ins_mp3info(struct ID3Info_s *ID3Info, const char *path)
{
	int ret;
	MYSQL_RES *result;
	MYSQL_ROW row;

	/* Check whether already exists */
	snprintf(sql_query_buf, MAX_QUERY_SIZE,
		 "SELECT * FROM %s WHERE %s = \"%s\"",
		 TABLE_NAME, FIELD_FILE_PATH, path);
	if (mysql_query(con, sql_query_buf)) {
		__finish_with_error(con);
		return -1;
	}
	result = mysql_store_result(con);

	row = mysql_fetch_row(result);
	if (!row) {
	/* If not insert a item */
		return __ins_mp3info(ID3Info, path);
	} else {
	/* If yes update a item */
		printf("%s: item: %s already inserted.\n", __func__, path);
	}
	return 0;
}
