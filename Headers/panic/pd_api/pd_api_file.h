//
//  pdext_file.h
//  Playdate Simulator
//
//  Created by Dave Hayden on 10/6/17.
//  Copyright © 2017 Panic, Inc. All rights reserved.
//

#ifndef pdext_file_h
#define pdext_file_h

#if TARGET_EXTENSION

typedef void SDFile;

typedef enum
{
	kFileRead		= (1<<0),
	kFileReadData	= (1<<1),
	kFileWrite		= (1<<2),
	kFileAppend		= (2<<2)
} FileOptions;

typedef struct
{
	int isdir;
	unsigned int size;
	int m_year;
	int m_month;
	int m_day;
	int m_hour;
	int m_minute;
	int m_second;
} FileStat;

#endif

#if !defined(SEEK_SET)
#  define SEEK_SET        0       /* Seek from beginning of file.  */
#  define SEEK_CUR        1       /* Seek from current position.  */
#  define SEEK_END        2       /* Set file pointer to EOF plus "offset" */
#endif

struct playdate_file
{
	const char* (*geterr)(void);

	int		(*listfiles)(const char* path, void (*callback)(const char* path, void* userdata), void* userdata);
	int		(*stat)(const char* path, FileStat* stat);
	int		(*mkdir)(const char* path);
	int		(*unlink)(const char* name, int recursive);
	int		(*rename)(const char* from, const char* to);
	
	SDFile*	(*open)(const char* name, FileOptions mode);
	int		(*close)(SDFile* file);
	int		(*read)(SDFile* file, void* buf, unsigned int len);
	int		(*write)(SDFile* file, const void* buf, unsigned int len);
	int		(*flush)(SDFile* file);
	int		(*tell)(SDFile* file);
	int		(*seek)(SDFile* file, int pos, int whence);
};

#endif /* pdext_file_h */
