#ifdef __cplusplus

extern "C" {

#endif

#include "do_posix_spawn.h"

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


   do_posix_spawn( char *const argv[], 
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
                   char *const forstderr)
{
    pid_t pid;
    int status;
    int numredir;
    char * xstdin;
    char * xstdout;
    char * xstderr;
    posix_spawn_file_actions_t fd_actions;
    posix_spawn_file_actions_t* pfd_actions;
    numredir = 0;

    if (forstdin[0]==0 || forstdin[0]=='-'&& forstdin[1]==0) {
      xstdin = NULL;
    } else {
      xstdin = forstdin;
      numredir++;
    } 

    if (forstdout[0]==0 || forstdout[0]=='-'&& forstdout[1]==0) {
      xstdout = NULL;
    } else {
      xstdout = forstdout;
      numredir++;
    } 

    if (forstderr[0]==0 || forstderr[0]=='-'&&forstderr[1]==0) {
      xstderr = NULL;
    } else {
      xstderr = forstderr;
      numredir++;
    } 

    if (numredir == 0) {
      pfd_actions = NULL;
    } else {
      pfd_actions = &fd_actions;
      if ((status = posix_spawn_file_actions_init (pfd_actions))) {
        perror ("failed posix_spawn_file_actions_init");
        exit(status);
      }
      if (xstdin) {
        if ((status=posix_spawn_file_actions_addopen (
                      pfd_actions, 0, xstdin,
                      O_RDONLY, 0))){
          perror ("failed posix_spawn_file_actions_addopen for stdin");
            exit(status);
        }
      }
      if (xstdout) {
        if ((status=posix_spawn_file_actions_addopen (
                      pfd_actions, 1, xstdout,
                      O_WRONLY | O_CREAT | O_TRUNC, 0644))){
          perror ("failed posix_spawn_file_actions_addopen for stdout");
            exit(status);
        }
      }
      if (xstderr) {
        if ((status=posix_spawn_file_actions_addopen (
                      pfd_actions, 2, xstderr,
                      O_WRONLY | O_CREAT | O_TRUNC, 0644))){
          perror ("failed posix_spawn_file_actions_addopen for stderr");
            exit(status);
        }
      }
    }

    status = posix_spawn(&pid, argv[0], pfd_actions, NULL, argv, envp);
    if (status == 0) {
        printf("Child pid: %i\n", pid);
        do {
          if (waitpid(pid, &status, 0) != -1) {
            printf("Child status %d\n", WEXITSTATUS(status));
          } else {
            perror("waitpid");
            exit(1);
          }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } else {
        printf("posix_spawn: %s\n", strerror(status));
    }
}

int do_posix_spawnp(char *const argv[],
                   char *const envp[], 
                   char *const forstdin,
                   char *const forstdout,
                   char *const forstderr)
{
    pid_t pid;
    int status;
    int numredir;
    char * xstdin;
    char * xstdout;
    char * xstderr;
    posix_spawn_file_actions_t fd_actions;
    posix_spawn_file_actions_t* pfd_actions;
    numredir = 0;

    if (forstdin[0]==0 || forstdin[0]=='-'&& forstdin[1]==0) {
      xstdin = NULL;
    } else {
      xstdin = forstdin;
      numredir++;
    } 

    if (forstdout[0]==0 || forstdout[0]=='-'&& forstdout[1]==0) {
      xstdout = NULL;
    } else {
      xstdout = forstdout;
      numredir++;
    } 

    if (forstderr[0]==0 || forstderr[0]=='-'&&forstderr[1]==0) {
      xstderr = NULL;
    } else {
      xstderr = forstderr;
      numredir++;
    } 

    if (numredir == 0) {
      pfd_actions = NULL;
    } else {
      pfd_actions = &fd_actions;
      if ((status = posix_spawn_file_actions_init (pfd_actions))) {
        perror ("failed posix_spawn_file_actions_init");
        exit(status);
      }
      if (xstdin) {
        if ((status=posix_spawn_file_actions_addopen (
                      pfd_actions, 0, xstdin,
                      O_RDONLY, 0))){
          perror ("failed posix_spawn_file_actions_addopen for stdin");
            exit(status);
        }
      }
      if (xstdout) {
        if ((status=posix_spawn_file_actions_addopen (
                      pfd_actions, 1, xstdout,
                      O_WRONLY | O_CREAT | O_TRUNC, 0644))){
          perror ("failed posix_spawn_file_actions_addopen for stdout");
            exit(status);
        }
      }
      if (xstderr) {
        if ((status=posix_spawn_file_actions_addopen (
                      pfd_actions, 2, xstderr,
                      O_WRONLY | O_CREAT | O_TRUNC, 0644))){
          perror ("failed posix_spawn_file_actions_addopen for stderr");
            exit(status);
        }
      }
    }

    status = posix_spawnp(&pid, argv[0], pfd_actions, NULL, argv, envp);
    if (status == 0) {
        printf("Child pid: %i\n", pid);
        do {
          if (waitpid(pid, &status, 0) != -1) {
            printf("Child status %d\n", WEXITSTATUS(status));
          } else {
            perror("waitpid");
            exit(1);
          }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } else {
        printf("posix_spawnp: %s\n", strerror(status));
    }
}

/* do_exec_to_buffer -- a pipe from exec of a command
   to a limited size buffer */

int do_exec_to_buffer(const char * cmd, char * buf, size_t bufsize ) {
    FILE* pipe = popen(cmd,"r");
    size_t cread;
    if (!pipe) {fprintf(stderr,"do_exec_to_buffer of %s failed to open read pipe\n",cmd); return -1;}
    while ((cread=fread(buf,1L,bufsize-1,pipe))<=0) {
      if (feof(pipe)) {
        pclose(pipe);
        return(-1);
      }
      sleep(3);
    } 
    buf[cread]='\0';
    pclose(pipe);
    return 0;
}

/* do_exec_from_buffer -- a pipe from a buffer to exec of a command */

int do_exec_from_buffer(const char * cmd, const char * buf, size_t bufsize ) {
    FILE* pipe = popen(cmd,"w");
    size_t cwrite;
    fprintf(stderr,"do_exec_from_buffer %s\n",buf);
    if (!pipe) {fprintf(stderr,"do_exec_from_buffer of %s failed to open write pipe\n",cmd); return -1;}
    if ((cwrite=fwrite(buf,1L,bufsize,pipe))<bufsize) {
      fprintf(stderr,"do_exec_from_buffer of %s failed to deliver output\n",cmd); return -1;}
    fflush(pipe);
    pclose(pipe);
    return 0;
}

/* write string to file */

int string_to_file(const char * at, const char * path, const char *str) {
    char buf[2048];
    char* cwd=getcwd(buf,2048L);
    int cwdnok=chdir(at);
    if (cwdnok) {fprintf(stderr,"at %s failed\n",at ); return -1;}
    FILE* file = fopen(path,"w");
    size_t cwrite;
    if (!file) {fprintf(stderr,"string %s to file %s failed\n",str,path ); return -1;}
    fprintf(stdout,"<tr><td colspan=3>string_to_file '%s' to '%s'</td></tr>\n",str,path);
    if (fprintf(file,"%s",str)<0){
      fprintf(stdout,"<tr><td colspan=3>FAILED</td></tr>\n"); 
    }
    fflush(file);
    fclose(file);
    chdir(cwd);
    return 0;
}

#ifdef __cplusplus

}

#endif
