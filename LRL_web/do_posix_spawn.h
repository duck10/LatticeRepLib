#ifndef DO_POSIX_SPAWN_H
#define DO_POSIX_SPAWN_H
#ifdef __cplusplus

extern "C" {

#endif

#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>

extern char **environ;

/* do_posix_spawn( char *const argv[], 
                   char *const envp[],
                   const char * forstdin, 
                   const char * forstdout,
                   const char * forstderr)

   executes the program argv[0] with the null-terminated argument list
   list argv substituting the paths given by forstdin, forstdout,  and
   forstderr for stdin, stdout and stderr, provided the path
   given is neither the string "-" nor empty. envp is the null-terminated
   list of environment strings.  

   do_posix_spawnp( char *const argv[], 
                   char *const envp[],
                   const char * forstdin, 
                   const char * forstdout,
                   const char * forstderr)

   executes the file argv[0] with the null-terminated argument list
   list argv substituting the paths given by forstdin, forstdout,  and
   forstderr for stdin, stdout and stderr, provided the path
   given is neither the string "-" nor empty. envp is the null-terminated
   list of environment strings.  

 */ 

int do_posix_spawn(char *const argv[],
                   char *const envp[], 
                   char *const forstdin,
                   char *const forstdout,
                   char *const forstderr);

int do_posix_spawnp(char *const argv[],
                   char *const envp[], 
                   char *const forstdin,
                   char *const forstdout,
                   char *const forstderr);

/* do_exec_to_buffer -- a pipe from exec of a command
   to a limited size buffer */

int do_exec_to_buffer(const char * cmd, char * buf, size_t bufsize );

/* do_exec_from_buffer -- a pipe from a buffer to exec of a command */

int do_exec_from_buffer(const char * cmd, const char * buf, size_t bufsize );
/* do_exec_to_buffer -- a little bootstrap pipe from exec of a command
   to a limited size buffer */

int do_exec_to_buffer(const char * cmd, char * buf, size_t bufsize );

int string_to_file(const char * at, const char * path, const char *str);

#ifdef __cplusplus

}

#endif
#endif
