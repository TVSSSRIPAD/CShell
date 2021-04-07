# CShell

A Basic and Simple Shell written in C

### Process to Run the Shell:-

    1. Fork the repo & cd into the correct directory
    2. Run the command 'make' 
    3. The makefile will create the cshell executable file
    4. Run './cshell' to start the shell
    5. If you face any issues, run the command 'make clean'. 
       This removes old executables. Then run 'make' to compile all the files again 
       and finally run './cshell'
#### The Prompt

The prompt of this shell is in the following format:-

[system name]:~ [absolute path of the current directory] >>

ex: sripad:~ /home/folder1>> 


#### Builtin Commands  

1. pwd : 
&ensp;&ensp;prints current working directory                      
2. cd : For changing directory   
&ensp;&ensp;Example: `cd path`
&ensp;&ensp;This chamges the current working directory to 'path.'
3. about : 
&ensp;&ensp;Shows basic details about this shell 
4. help :   
&ensp;&ensp;shows help text about basic inbuilt commands  
5. history :   
&ensp;&ensp;Gives list of recently ran commands      
&ensp;&ensp; Note:- Recently ran commands can be ran again by entering !num. This will run the numth command from history again.  
&ensp;&ensp;Example:- Suppose output of history is as follows:-
&ensp;&ensp;1 ls  
&ensp;&ensp;2 pwd  
&ensp;&ensp;3 about  
&ensp;&ensp;4 help  
&ensp;&ensp;The `pwd` command can be ran again by running !2 in the shell
6. jobs :  
&ensp;&ensp;Lists all the processes running in the background.
&ensp;&ensp;Processes can be run in the background by adding '&' at the ending of commands.
&ensp;&ensp;There must be a white-space between '&' and the command.   
7. kill \<pid> : 
&ensp;&ensp;kills the process with pid = \<pid>    
8. exit : exits the shell cleanly 

#### User Commands  
 - This shell also supports user commands.  
 - User command can be pushed to the background by adding '&' at the end of command.
 - The stdin and stdout can also be redirected by using '> filename' and '< filename'
 - Note:- There must be a single white space between > or <  and the filename.
   Example:- ./prog > op.txt < ip.txt
    This command causes the program to take input from tmp.txt and prints the output to op.txt.


#### TODO 
- Add  '|' Pipe handling

 