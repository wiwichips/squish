# SQUISH
Superior Quality Unbeatable Interprocess (communication) Shell, or "squish", is a shell that supports running processes, piping between processes, handle redirection, variable globbing, cd and exit. 
## How to Compile and Run
```bash
make
./squish
```
## How errors are handled
* When entering a command that doesn't exist, squish will ouput "name: command not found". 
* The exit status of the command will be outputted if running a single command. 
## Piping algorithm
Processes are piped recursively backwards from the last command. The recursive base case is when there is only one command left to be exec'd.

For ex. ``p1 | p2 | p2`` can be represented as:
```
       squish
         /\
        /  \
       /\   p3
      /  \
     p1  p2
```
Pseudo-code follows as such:
```c
ipc (tokens, nTokens) 
{
  if ( nTokens == 1 ) {
    fork new process
    exec ( tokens[0] )
    return
  }

  fork new process
  if ( pid == 0 ) {
    replace stdout with write end of pipe
    ipc (tokens, --ntokens)
  }

  fork new process
  if ( pid == 0 ) {
    replace stdin with read end of pipe
    exec ( tokens[nTokens] )
  }
}
```
## File Structure
### Tokenization Files by AHW
squish_main.c
squish_run.c
squish_tokenize.c
### "Built-in" CD and exit
w_change_dir.c
w_exit.c
### Globbing
w_glob.c
### Piping and redirection
w_pipe.c
w_redirection.c
### Input parser
w_run.c
### Exec wrapper
w_run_command.c

## Works Cited
Some of the code is copied from the cis-3050 c examples from the linux.socs server. Wherever that is the case, a comment block will be included above the code snippit.
