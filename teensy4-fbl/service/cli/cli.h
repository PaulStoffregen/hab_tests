#ifndef CLI_H
#define CLI_H


#define CLI_OK                  0
#define CLI_PENDING             1
#define CLI_ERROR_INVALID_CMD  -1
#define CLI_ERROR_INVALID_ARGS -2

#define CLI_CMD_END 13
#define MAX_CMD_LEN 12


/*
 * Function prototype of command
 */
typedef int (*T_CLI_COMMAND)(const char* argStr);


/*
 * Structure defining a command table entry
 */
typedef struct T_CLI_ENTRY
{
  const char*   cmdString;
  T_CLI_COMMAND cmdCallBack;
}T_CLI_ENTRY;


int   cli_parseCmd(const T_CLI_ENTRY* cmdTable, const char* cmd, const char** argStr);
sint8 cli_execCmd(T_CLI_ENTRY* cmdTbl, sint16 tblPos, const char* argStr);
sint8 cli_matchWildcard(const char* refStr, const char* cmdStr, uint16* cmdLen);

#endif /* CLI_H */
