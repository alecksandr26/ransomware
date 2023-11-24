#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <errno.h>

#include "../include/file_encipher.h"
#include "../include/encipher.h"
#include "../include/log.h"

#define BUFF_SIZE 4096
#define FILE_NAME_SIZE 1024

void get_file_name(const char *file, char *file_name)
{
	assert(file != NULL && file_name != NULL && "Can't receive null arguments");
	
	int pos = strlen(file);
	int lenght = pos;
	
	while (pos > 0 && file[--pos] != '\\')
		;
	
	if (pos > 0)
		pos++;
	
	while (pos < lenght)
		*(file_name++) = file[pos++];
}

void get_path_from_file(const char *file, char *path)
{
	assert(file != NULL && path != NULL && "Can't receive null arguments");
	
	int pos = strlen(file);
	
	while (pos > 0 && file[--pos] != '\\')
		;
	
	for (int i = 0; i < pos; i++)
		*(path++) = file[i];
}

void get_encrypted_file_name(const char *encrypted_file, char *file_name)
{
	assert(encrypted_file != NULL && file_name != NULL && "Can't receive null arguments");

	int pos = strlen(encrypted_file), pos_point;

	pos_point = pos;
	while (pos > 0 && encrypted_file[--pos] != '\\')
		;

	if (pos > 0)
		pos++;
	
	while (pos_point > 0 && encrypted_file[--pos_point] != '.')
		;
	
	while (pos < pos_point)
		*(file_name++) = encrypted_file[pos++];
}

void file_encipher(const char *file, const char *str_key)
{
	assert(file != NULL && str_key != NULL && "Can't receive null arguments");
	
	size_t key_size = strlen(str_key);
	char file_name[FILE_NAME_SIZE], file_path[FILE_NAME_SIZE];
	char ciphered_file[FILE_NAME_SIZE], ciphered_file_name[FILE_NAME_SIZE];

	memset(file_name, 0, FILE_NAME_SIZE);
	memset(file_path, 0, FILE_NAME_SIZE);
	memset(ciphered_file, 0, FILE_NAME_SIZE);
	memset(ciphered_file_name, 0, FILE_NAME_SIZE);
	
	get_file_name(file, file_name);
	get_path_from_file(file, file_path);

	if (file_path[0] != 0) {
		strcat(ciphered_file, file_path);
		strcat(ciphered_file, "\\");
	}
	
	strcat(ciphered_file, file_name);
	strcat(ciphered_file, ".encrypted");
	
	strcat(ciphered_file_name, file_name);
	strcat(ciphered_file_name, ".encrypted");

	LOG("Encrypting %s --> %s\n", file_name, ciphered_file_name);
	
	/* Open and create the ciphered file */
	FILE *file_ptr = fopen(file, "rb");
	
	if (file_ptr == NULL) {
		LOG_ERROR("Error openning the file %s: %s\n", file, strerror(errno));
		return;
	}

	FILE *ciphered_file_ptr = fopen(ciphered_file, "wb");

	if (ciphered_file_ptr == NULL) {
		LOG_ERROR("Error creating the file %s: %s\n", ciphered_file, strerror(errno));
		return;
	}

	char buff[BUFF_SIZE], ciphered_buff[BUFF_SIZE];
	
	memset(buff, 0, BUFF_SIZE);
	memset(ciphered_buff, 0, BUFF_SIZE);

	int lenght = 0;
	while ((lenght = fread(buff, sizeof(char), BUFF_SIZE, file_ptr)) > 0) {
		encipher((const unsigned char *) buff, (unsigned char *) ciphered_buff,
			 lenght, str_key, key_size);

		fwrite(ciphered_buff, sizeof(char), lenght, ciphered_file_ptr);
		
		memset(buff, 0, lenght);
		memset(ciphered_buff, 0, lenght);
	}
	
	fclose(file_ptr);
	fclose(ciphered_file_ptr);

	/* Delete the file */
	DeleteFile(file);
}

void file_decipher(const char *ciphered_file, const char *str_key)
{
	assert(ciphered_file != NULL && str_key != NULL && "Can't receive null arguments");

	size_t key_size = strlen(str_key);
	char file_name[FILE_NAME_SIZE], ciphered_file_path[FILE_NAME_SIZE];
	char file[FILE_NAME_SIZE], file_ciphered_name[FILE_NAME_SIZE];

	memset(ciphered_file_path, 0, FILE_NAME_SIZE);
	memset(file_name, 0, FILE_NAME_SIZE);
	memset(file, 0, FILE_NAME_SIZE);
	memset(file_ciphered_name, 0, FILE_NAME_SIZE);
	
	get_encrypted_file_name(ciphered_file, file_name);
	get_file_name(ciphered_file, file_ciphered_name);
	get_path_from_file(ciphered_file, ciphered_file_path);


	if (ciphered_file_path[0] != 0) {
		strcat(file, ciphered_file_path);
		strcat(file, "\\");
	}
	
	strcat(file, file_name);
	
	LOG("Decrypting %s --> %s\n", file_ciphered_name, file_name);
	
	/* Open and create the ciphered file */
	FILE *ciphered_file_ptr = fopen(ciphered_file, "rb");
	
	if (ciphered_file_ptr == NULL) {
		LOG_ERROR("Error openning the file %s: %s\n", file, strerror(errno));
		return;
	}

	FILE *file_ptr = fopen(file, "wb");

	if (file_ptr == NULL) {
		LOG_ERROR("Error creating the file %s: %s\n", ciphered_file, strerror(errno));
		return;
	}

	char ciphered_buff[BUFF_SIZE], buff[BUFF_SIZE];
	
	memset(buff, 0, BUFF_SIZE);
	memset(ciphered_buff, 0, BUFF_SIZE);

	int lenght = 0;
	while ((lenght = fread(ciphered_buff, sizeof(char), BUFF_SIZE, ciphered_file_ptr)) > 0) {
		decipher((const unsigned char *) ciphered_buff, (unsigned char *) buff,
			 lenght, str_key, key_size);

		fwrite(buff, sizeof(char), lenght, file_ptr);
		
		memset(buff, 0, lenght);
		memset(ciphered_buff, 0, lenght);
	}
	
	fclose(file_ptr);
	fclose(ciphered_file_ptr);

	/* Delete the file */
	DeleteFile(ciphered_file);
}




