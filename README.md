# ICT374 Assignment 2

[Link to assignment details](https://www.it.murdoch.edu.au/~S900432D/oli1l1hsu_x3X64dg72kf7Th973yihbkys9M10in0x/assignments/a2/shell.shtml)

### Contributors
***
- Ben Hawkins
- Michaela Angus

### How to Build 
***
1. Run 'make master'
2. Run 'make clean'

### To do 
***

|     Tasks     |    Assigned   |    Status     |
| ------------- | ------------- | ------------- |
| Reconfigurable Shell Prompt  | Michaela  | Completed |
| Built-in command `pwd` | Michaela | Completed |
| Directory walk `cd` | Ben | Completed | 
| Wild card characters | Ben | Review | 
| Standard input & output redirection `<` and `>` | Michaela / Ben | Completed |
| Shell pipeline `\|` | Ben | incomplete | 
| Background job execution `&` | Ben | incomplete | 
| The shell environment | | incomplete |
| Built-in command `exit` | Michaela | Completed |


### Bug Tracker
***
| Issue | Assigned | Status | 
| ----- | -------- | ------ |
| prompt function breaks when: prompt set then a command is issued with two arguments | Michaela | Completed |
| pwd function will break when the directory changes | | Incomplete |
| Stdin < redirection results in a segmentation fault | Ben | Complete |
| Stdin < redirection has issues with newly created files | Michaela | Complete |
| prompt function will break with a command like this: `ls -l` wont break on `blah blah` prompt function without placeholder will break on both: suggestion fix prompt without placeholder first, likely related | | Incomplete |
| (For branch Separtors) Command `ls &` will result with two outputs --> called when it finishes and when we check if it finishes using SIGCHLD | | Incomplete | 
