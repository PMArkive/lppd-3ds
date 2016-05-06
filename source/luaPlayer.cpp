/*----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#------  This File is Part Of : ----------------------------------------------------------------------------------------#
#------- _  -------------------  ______   _   --------------------------------------------------------------------------#
#------ | | ------------------- (_____ \ | |  --------------------------------------------------------------------------#
#------ | | ---  _   _   ____    _____) )| |  ____  _   _   ____   ____   ----------------------------------------------#
#------ | | --- | | | | / _  |  |  ____/ | | / _  || | | | / _  ) / ___)  ----------------------------------------------#
#------ | |_____| |_| |( ( | |  | |      | |( ( | || |_| |( (/ / | |  --------------------------------------------------#
#------ |_______)\____| \_||_|  |_|      |_| \_||_| \__  | \____)|_|  --------------------------------------------------#
#------------------------------------------------- (____/  -------------------------------------------------------------#
#------------------------   ______   _   -------------------------------------------------------------------------------#
#------------------------  (_____ \ | |  -------------------------------------------------------------------------------#
#------------------------   _____) )| | _   _   ___   ------------------------------------------------------------------#
#------------------------  |  ____/ | || | | | /___)  ------------------------------------------------------------------#
#------------------------  | |      | || |_| ||___ |  ------------------------------------------------------------------#
#------------------------  |_|      |_| \____|(___/   ------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Licensed under the GPL License --------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Copyright (c) Nanni <lpp.nanni@gmail.com> ---------------------------------------------------------------------------#
#- Copyright (c) Rinnegatamante <rinnegatamante@gmail.com> -------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Credits : -----------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Smealum for ctrulib and ftpony src ----------------------------------------------------------------------------------#
#- StapleButter for debug font -----------------------------------------------------------------------------------------#
#- Lode Vandevenne for lodepng -----------------------------------------------------------------------------------------#
#- Jean-loup Gailly and Mark Adler for zlib ----------------------------------------------------------------------------#
#- Special thanks to Aurelio for testing, bug-fixing and various help with codes and implementations -------------------#
#-----------------------------------------------------------------------------------------------------------------------*/

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "include/luaplayer.h"

static lua_State *L;
extern void drawError(char* cmd, char* format, ...);

const char *runScript(const char* script, bool isStringBuffer)
{
	L = luaL_newstate();
	
	// Standard libraries
	luaL_openlibs(L);
	
	// Modules
	luaSystem_init(L);
	luaScreen_init(L);
	luaControls_init(L);
	luaTimer_init(L);
	
	int s = 0;
	const char *errMsg = NULL;
	
	//Patching dofile function & I/O module
	/*char* patch = "dofile = System.dofile\n\
			 io.open = System.openFile\n\
			 io.write = System.writeFile\n\
			 io.close = System.closeFile\n\
			 io.read = System.readFile\n\
			 io.size = System.getFileSize";
	luaL_loadbuffer(L, patch, strlen(patch), NULL); 
	lua_KFunction dofilecont = (lua_KFunction)(lua_gettop(L) - 1);
	lua_callk(L, 0, LUA_MULTRET, 0, dofilecont);*/
	
	if(!isStringBuffer) 
		s = luaL_loadfile(L, script);
	else 
		s = luaL_loadbuffer(L, script, strlen(script), NULL);
		
	if (s == 0) 
	{
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
	}
	if (s) 
	{
		errMsg = lua_tostring(L, -1);
		lua_pop(L, 1); // remove error message
	}
	lua_close(L);
	
	return errMsg;
}
