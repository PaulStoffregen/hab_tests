#ifndef CLI_C
#define CLI_C
#endif /* CLI_C */

/*lint -esym(960,17.4) pointer arithmetic by increment or decrement used*/
//#define ENABLE_DEBUG 1

#include "bsp.h"
//#include "trace.h"
#include "cli.h"

/*
 ******************************************************************************
 * Function: cli_parseCmd()
 ******************************************************************************
 * @par Description:
 *   This function traverses the given command table for a command string
 *   matching the string given as command. The matching stops ii either of both
 *   strings ends or differ in a single character before they end.
 *   So, in order to sucessfully being recognized as valid command, the string
 *   given as command must match a whole reference string in the given command
 *   table. If the string given as command contains further characters starting
 *   with a parameter delimiter (space or equal-sign) the remaining part is
 *   interpeted as command arguments. 
 *   If successfully matched, the function returns the pointer to the commands
 *   callback and if arguments where detected, the part of the string
 *   containing those arguments.
 *   The exact interpretation and parsing of those arguments shall be done
 *   by the command callback function.
 *
 * @param cmdTable [IN] - Command table to be used
 * @param tblSize [IN] - Size of the command table in terms of entries
 * @param stopChars [IN] - An array of characters which shall be interpreted
 *   as stop characters during matching procedure.
 * @param cmdLine [IN] - The string containing the command line to be prsed.
 *
 * @return ID of the table entry containing the recognized command or -1 in
 *   thr field tblPos and the actual length of the command in field cmdLen.
 *
 * @retval 0..tblSize-1 in case of success
 * @retval -1 in case of unrecognized command
 * @retval actual length of the command
 *
 ******************************************************************************
 */

int cli_parseCmd(const T_CLI_ENTRY* cmdTable, const char* cmd, const char** argStr)
{
   sint16 result;
   uint8  tblPos;
   uint16 cmdLen;

   /* Initialize result */
   result = -1;
   tblPos = 0;
   while(cmdTable[tblPos].cmdString != NULL)
   {
      const char* refStr = cmdTable[tblPos].cmdString;
      const char* cmdStr = cmd;

#if (ENABLE_DEBUG == STD_ON)
      TRACE_INFO("Ref String: N=%d >%s<\n", tblPos, refStr);
#endif /* (ENABLE_DEBUG == STD_ON) */

      if('\0' == *refStr)
      {
         /* Reference string is empty, so continue with next table entry */
      }
      else if(0 == cli_matchWildcard(refStr, cmdStr, &cmdLen) )
      {
         /* Command string doesn't match, so continue with next table entry */
      }
      else
      {
         /* Command string matched */
         result = tblPos;
         *argStr = cmd + cmdLen;
         break;
      }
      tblPos++;
   }
   return result;
}


sint8 cli_matchWildcard(const char* refStr, const char* cmdStr, uint16* cmdLen)
{
   sint8 result = 0;
   const char* starStr = NULL;
   const char* cmdLine = cmdStr;

   /* Traverse and through the reference string and
    * compare each character with command string.
    */
#if (ENABLE_DEBUG == STD_ON)
   TRACE_INFO("CmdLine: >%s<\n", cmdLine);
#endif /* (ENABLE_DEBUG == STD_ON) */
   
   while('\0' != *refStr)
   {
#if (ENABLE_DEBUG == STD_ON)
      TRACE_INFO("R # C ... %c # %c\n", *refStr, *cmdStr);
#endif /* (ENABLE_DEBUG == STD_ON) */

      if('\0' == *cmdStr)
      {
         /* The command sting reached the terminating character,
          * so break the loop
          */
         break;
      }
      else if(' ' == *cmdStr)
      {
         /* The command string reached the argument delimiter,
          * so break the loop.
          * The reference string's current char might be anything
          * especialy:
          * 1. '\0' - Which means even the reference string ended
          * 2. '\\' - Which 
          */
         break;
      }

      if('\\' == *refStr)
      {
         /* Reference string contains an escape character, so handle
          * the escape sequence.
          * We expect a special character to follow the escape character,
          * thus eat the escape character and check the escaped one.
          */
         refStr++;
         
         /* The escaped character can be one of the following 3:
          * 1. The escape character itself, to indicate that the 
          *    command string should contain this character.
          * 2. The special character matching any single charater
          *    in the command string.
          * 3. The special character matching any sequence of
          *    characters in the command string.
          * In any case we expect at least one single further character
          * in the command string which is not the argument delimiter.
          */
         if('\\' == *refStr)
         {
            /* The escaped character is the escape charater itself,
             * so proceed with string matching below
             */
         }
         else if('.' == *refStr)
         {
            /* The escaped character is the special character for matching
             * any single character, so we need to eat a single character
             * from both, the command string and the reference string and
             * continue with the next character.
             */
            cmdStr++;
            refStr++;
            continue;
         }
         else if('*' == *refStr)
         {
            /* The escaped character is the special character for matching
             * any sequence of characters, so we need to eat a single
             * character of the reference string then proceed with string
             * matching below. We need to remember that new position of the
             * reference string for later rewind if necessary. By the fact
             * that the star string is no more empty from now on we have
             * also an indication that we are in star-matching. 
             */
            refStr++;
            starStr = refStr;
         }
         else
         {
            /* The reference string contains an invalid escape sequence,
             * so interpret the escaped character as the escape character
             * itself, thus rewind the reference string one character
             * and proceed with string matching below.
             */
            refStr--;
         }
      }

      if(*refStr == *cmdStr)
      {
         /* Eat a character of both, command string and
          * reference string, and continue with next
          * character.
          */
         refStr++;
         cmdStr++;
      }
      else if(starStr != NULL)
      {
         /* The characters don't match, but we are in star matching,
          * thus eat a single character of the of the command string
          * and rewind the reference string to the recent star position
          */
         cmdStr++;
         refStr = starStr;
      }
      else /* if(*refStr != *cmdStr) */
      {
         /* Mismatch! */
         result = 0;
         break;
      }
   }

   /* When execution comes to this location, the following could
    * have happend:
    * 1. The whole reference string has been processed and matched
    * the command string up to it's end or the argument delimiter
    * 
    * 2. The whole reference string has been processed and matched
    * the command string up to the current character which is neither
    * the terminating character nor the argument delimiter
    *
    * 3. 
    * So test whether also the command string finished by testing
    * it's next character.
    */

   if('\0' != *refStr)
   {
      /* Reference string still contains characters, so command string
       * doesn't match. Continue with next table entry
       */
   }
   else if(' ' == *cmdStr)
   {
      /* The command string's next character is the argument delimiter,
       * indicating that parameters may follow, so a matching command was
       * found in the table.
       */
      result = 1;
#if (ENABLE_DEBUG == STD_ON)
      TRACE_INFO("CMD:>%s< ARG:>%s<\n", refStr, cmdStr);
#endif /* (ENABLE_DEBUG == STD_ON) */
   }
   else if('=' == *cmdStr)
   {
      /* The command string's next character is the argument delimiter,
       * indicating that parameters may follow, so a matching command was
       * found in the table.
       */
      result = 1;
#if (ENABLE_DEBUG == STD_ON)
      TRACE_INFO("CMD:>%s< ARG:>%s<\n", refStr, cmdStr);
#endif /* (ENABLE_DEBUG == STD_ON) */
   }
   else if('\0' == *cmdStr)
   {
      /* The command string's next character is the terminating character
       * so a matching command was found in the table, but no arguments
       * follow
       */
      result = 1;
   }
   else if(NULL != starStr)
   {
      /* The command string was not yet processed completely, but we are
       * in star matching (star at the end of the reference string),
       * so eat every character of the command srting up to the argument
       * delimiter or the terminating character, whatever comes first
       */
      while( ('\0' != *cmdStr) && (' ' != *cmdStr) )
      {
         cmdStr++;
      }
      result = 1;
   }
   else
   {
      /* The command string was not processed completely,
       * so it doesn't match
       */
   }
   *cmdLen = cmdStr - cmdLine;
   return result;
}


/*
 ******************************************************************************
 * Function: cli_execCmd()
 ******************************************************************************
 * @par Description:
 *   This function, depending on its arguments executes the given callback
 *   or just returns.
 *
 * @param cmdCallBack [IN] - Pointer to the callback function to be executed
 * @param argStr [IN] - Ponter to the argument string to be delivered to the
 *   callback
 *
 * @return void
 *
 ******************************************************************************
 */

sint8 cli_execCmd(T_CLI_ENTRY* cmdTbl, sint16 tblPos, const char* argStr)
{
   T_CLI_COMMAND callBack;
   sint8 result = CLI_ERROR_INVALID_CMD;

   if(tblPos >= 0)
   {
#if (ENABLE_DEBUG == STD_ON)
      TRACE_INFO("CMD decoded\n");
#endif /* (ENABLE_DEBUG == STD_ON) */
      callBack = cmdTbl[tblPos].cmdCallBack;
      result = callBack(argStr);
   }
   else
   {
#if (ENABLE_DEBUG == STD_ON)
      TRACE_INFO("CMD unknown\n");
#endif /* (ENABLE_DEBUG == STD_ON) */
   }
   return result;
}


