/*
 *  Author : WangBoJing , email : 1989wangbojing@gmail.com
 * 
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Author. (C) 2016
 * 
 *
 
****       *****
  ***        *
  ***        *                         *               *
  * **       *                         *               *
  * **       *                         *               *
  *  **      *                        **              **
  *  **      *                       ***             ***
  *   **     *       ******       ***********     ***********    *****    *****
  *   **     *     **     **          **              **           **      **
  *    **    *    **       **         **              **           **      *
  *    **    *    **       **         **              **            *      *
  *     **   *    **       **         **              **            **     *
  *     **   *            ***         **              **             *    *
  *      **  *       ***** **         **              **             **   *
  *      **  *     ***     **         **              **             **   *
  *       ** *    **       **         **              **              *  *
  *       ** *   **        **         **              **              ** *
  *        ***   **        **         **              **               * *
  *        ***   **        **         **     *        **     *         **
  *         **   **        **  *      **     *        **     *         **
  *         **    **     ****  *       **   *          **   *          *
*****        *     ******   ***         ****            ****           *
                                                                       *
                                                                      *
                                                                  *****
                                                                  ****


 *
 */
 
 
#include "NattyUtils.h"
#include "NattyLog.h"
#include <stdlib.h>
#include <time.h>

#if 0
typedef struct tm TimeStamp;

TimeStamp* ntyGetSystemTime(void) {
	time_t timer;
	TimeStamp *tblock;
	timer = time(NULL);
	tblock = localtime(&timer);

	return tblock;
}

#endif

#if 1 //Rename zlog function
int nLogInit(const char *confpath) {
	return zlog_init(confpath);
}

nLogCategory* nLogGetCategory(const char *cname) {
	return zlog_get_category(cname);
}

void nLogFinally(void) {
	return zlog_fini();
}

void nLogInfoWithTime(nLogCategory *cat, const char *format, va_list *params) {

	U8 catBuffer[NATTY_LOG_BUFFER_LENGTH] = {0};
	U8 buffer[NATTY_LOG_BUFFER_LENGTH + 32] = {0};
	
	TimeStamp *tblock = ntyGetSystemTime();
	
	sprintf(catBuffer, format, params);

	sprintf(buffer, " [%d-%d-%d %d:%d:%d] --> %s", tblock->tm_year % 100 + 2000, tblock->tm_mon+1, tblock->tm_mday,
		tblock->tm_hour, tblock->tm_min, tblock->tm_sec, catBuffer);

	zlog_info(cat, "%s", buffer);
}

void nLogInfoWithTimePrefix(nLogCategory *cat, const char *format) {

	U8 buffer[NATTY_LOG_BUFFER_LENGTH + 32] = {0};
	
	TimeStamp *tblock = ntyGetSystemTime();
	
	sprintf(buffer, " [%d-%d-%d %d:%d:%d] --> %s", tblock->tm_year % 100 + 2000, tblock->tm_mon+1, tblock->tm_mday,
		tblock->tm_hour, tblock->tm_min, tblock->tm_sec, format);

	zlog_info(cat, "%s", buffer);

	//nLogFinally();
}


void nLogConsoleShow(const char *format) {
	
	TimeStamp *tblock = ntyGetSystemTime();
	fprintf(stdout, " [%d-%d-%d %d:%d:%d] --> %s", tblock->tm_year % 100 + 2000, tblock->tm_mon+1, tblock->tm_mday,
		tblock->tm_hour, tblock->tm_min, tblock->tm_sec, format);
	
}

#endif

void* ntyLogInitialize(nLog *log) {
	int rc = -1;

	rc = nLogInit(NATTY_LOG_CONF);
	if (rc) {
		printf("init failed : %d\n", rc);
		return NULL;
	}

	log->nCategory = nLogGetCategory(NATTY_LOG_CATEGORY);
	if (!log->nCategory) {
		printf("get cat failed\n");
		nLogFinally();
		return NULL;
	}

	return log;
}

void* ntyLogCtor(void *self, va_list *params) {
	return ntyLogInitialize(self);
}

void* ntyLogDtor(void *self) {
	nLogFinally();
	return NULL;
}


void ntyLogWithTime(void *self, const char *format) {
	nLog *log = self;
	
	nLogInfoWithTimePrefix(log->nCategory, format);
}

static const LogHandle ntyLogHandle = {
	sizeof(nLog),
	ntyLogCtor,
	ntyLogDtor,
	ntyLogWithTime,
};

const void *pNtyLogHandle = &ntyLogHandle;
static void *pLogHandle = NULL;

void* ntyLogInstance(void) {
	if (pLogHandle == NULL) {
		void *pLog = New(pNtyLogHandle);
		if ((unsigned long)NULL != cmpxchg((void*)(&pLogHandle), (unsigned long)NULL, (unsigned long)pLog, WORD_WIDTH)) {
			Delete(pLog);
		}
	}
	return pLogHandle;
}

void ntyLogInfo(const char *format, ...) {
	void *log = ntyLogInstance();
	U8 catBuffer[NATTY_LOG_BUFFER_LENGTH] = {0};

	va_list params;
	va_start(params, format);
	
	vsprintf(catBuffer, format, params);
	ntyLogWithTime(log, catBuffer);
	
	va_end(params);
}

void ntyLogShow(const char *format, ...) {
	U8 catBuffer[NATTY_LOG_BUFFER_LENGTH] = {0};

	va_list params;
	va_start(params, format);
	
	vsprintf(catBuffer, format, params);
	nLogConsoleShow(catBuffer);
	
	va_end(params);
}



//#define ntylog(format, ...) 		fprintf(stdout, format, ##__VA_ARGS__)

#if 0
int main (void) {
	ntyLog(" Start Main");

	ntyLog(" Start Main .........");

	ntyLog(" Start Main 12344");

	ntyLog(" Start End .........");

	nLogFinally();
}
#endif


