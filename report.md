# EECS678-Project1 - Report
[back](https://github.com/BenSokol/EECS678-Project1/blob/master/FEATURES.md) to README<br>

## Description
The quash shell we built attempts to mimic bash while still fulfilling the required features as listed in the project page. We first tokenize the input for keywords, and pipes, and use the tokens to receive the command and its args to execute.

### Run executables without arguments.
  * Running executables without arguments is done mainly within QUASH_tokenizer, QUASH_process, and QUASH_main.
  * Running "ls" for example, will list the files and folders within the current working directory.
  * Within QUASH_process, if the executable, located in the first token from the tokenizer, is not a key word (such as cd or kill), then it will create a fork and run that executable in a child process, while the parent waits for the child process to finish.

### Run executables with arguments.
  * Running executables with arguments is done mainly within QUASH_tokenizer, QUASH_process, and QUASH_main.
  * Running "ls" + directory name, will list the files within that directory.
  * Within QUASH_process, if the executable exists, located in the first token from the tokenizer, is not a builtin function (such as cd or kill), then it will create a fork and run that executable in a child process, while the parent waits for the child process to finish. The arguments for that executable will be located in the later tokens and executed along with the executable within the execvpe (linux) or execvP (macOS) call.

### set for HOME and PATH work properly
  * Both set for HOME and PATH work properly. Setting home sets the variable "HOME", and now whenever HOME is called, it will use the new location. PATH also will update correctly. This is done in QUASH_process. It uses the setenv() function to change the variables. It also updates the mEnv variable (char **envp from main).

### exit and quit work properly
  * Running "exit" or "quit" will exit QUASH as expected. This is done similar to running executables without arguments, however instead of executing anything, a status is passed into QUASH_main and main handles that signal to exit QUASH.

### cd (with and without arguments) works properly
  * cd changes the pwd (present working directory). This is done mostly within QUASH_cd.
    * When there are no arguments to cd, the HOME directory is used and the pwd is changed to that.
    * When there are arguments to cd that start with (/), we attempt to change directories to that absolute path.
    * When there are arguments to cd that do not start with (/), we attempt to change directories to that relative path using the pwd.
  * We utilize the chdir() function to perform the actual change.

### PATH works properly. Give error messages when the executable is not found
  * Quash will make sure that the executable is present within the PATH, at an absolute path, or in the local directory and give an error if it isn't.
  * Within QUASH_process, using the std::filesystem class, we search if the executable is in the PATH before running it, and it will return an error if it does not exist.

### Child processes inherit the environment
  * Using the fork() command, the child process will receive the environment of the parent when running its process.

### Allow background/foreground execution (\&)
  * When creating a process, we check to see if we run the process asynchronously (foreground), or in the background. If it is to be run in the background, we create a thread to run the process and add it to a list of "processes". We check this list of processes using "jobs" to see if the process is still running in the background or it is finished.
  * If it is finished, it gets removed from the list of processes.

### Printing/reporting of background processes, (including the jobs command)
  * Using the "jobs" command, we see the list of current background processes. When a background process finishes, we report that to the screen when the next command (or just enter) is entered.
  * When starting a background process, we list the one that is starting along with the ones that are currently running.
  * This is done mostly in QUASH_process and QUASH_main using the checkJobStatus function.

### Allow file redirection (> and <)
  * We did not get this working successfully.

### Allow (1) pipe (|)
  * Within QUASH_process, we check the tokens returned from the tokenizer to see if there is a "|" listed within. If so, we create a pipe, and this pipe is put onto a list of pipes (in case we have multiple). We then take the currentCommand (all the tokens up to the "|"), and run that while we pipe the output from that into the input of the next one.
  * For example, "ls | less", should work as intended.

### Supports reading commands from prompt and from file
  * This was implemented to imitate bash. Quash does NOT print a prompt when reading in from file (<), so only output from commands are printed.

### Support multiple pipes in one command.
  * Using the logic from QUASH_process, multiple pipes should be supported, however, it has not been fully tested yet.

### kill command delivers signals to background processes. The kill command has the format:
  * kill SIGNUM JOBID, where SIGNUM is an integer specifying the signal number, and JOBID is an integer that specifies the job that should receive the signal.
  * Kill is handled in QUASH_process. We check to see if the jobid given in within the scope of our list of processes, and if so, we send the signal to that process. It then stops the process and deletes it from the list of processes.
  * "kill 2 9" sends SIGKILL the second process running in the background.

## Testing
For testing, we used 2 different methods.
1. We used a test suite ([Catch2](https://github.com/catchorg/Catch2)).
  * This can be run by using the following commands:

    ```bash
    # Compile tests
    make test

    # Run
    ./quash-test

    # Alternatively, this will compile tests, run them, and run cppcheck (a static analyzer).
    make run-tests
    ```

  * This fully tests the tokenizer, cd, set (for HOME, not PATH), home, pwd, ps1, and whoami
2. Manual testing. This was done to test all internal functions, and a variety of external arguments such as "ls /usr/bin | less", which opens less with the output of /usr/bin correctly.
