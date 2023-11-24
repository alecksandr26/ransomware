#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <string.h>

#define LOG_MSG_SIZE 1024

#define LOG(msg, ...) do {						\
		char msg_out[LOG_MSG_SIZE];				\
		memset(msg_out, 0, LOG_MSG_SIZE);			\
		strcat(msg_out, "[INFO]: ");				\
		strcat(msg_out, msg);					\
		fprintf(stdout, msg_out __VA_OPT__(, __VA_ARGS__));	\
	} while (0)

#define LOG_ERROR(msg, ...) do {						\
		char msg_out[LOG_MSG_SIZE];				\
		memset(msg_out, 0, LOG_MSG_SIZE);			\
		strcat(msg_out, "[ERROR]: ");				\
		strcat(msg_out, msg);					\
		fprintf(stderr, msg_out __VA_OPT__(, __VA_ARGS__));	\
	} while (0)

#endif




