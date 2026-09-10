
#include <string.h>
#include <stdbool.h>
#include "raylib.h"

int sign(int x)
{
	return x == 0 ? 0 : x > 0 ? 1 : -1;
}

void strlower(char* str)
{
	for(int i = 0; i < strlen(str); ++i)
	{
		char c = str[i];
		if (c >= 65 && c <= 90)
		{
			str[i] += 32; // difference between A and a
		}
	}
}

void strupper(char* str)
{
	for(int i = 0; i < strlen(str); ++i)
	{
		char c = str[i];
		if (c >= 97 && c <= 122)
		{
			str[i] -= 32; // difference between A and a
		}
	}
}

bool strmatch(char* cmd, char* input)
{
	return strncmp(cmd, input, strlen(cmd)) == 0;
}

bool IsControlDown()
{
	return ( IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL) );
}
