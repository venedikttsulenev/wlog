# wlog
Simple command-line time tracking utility

## COMMANDS
      t timer <task>             - Start timer for 'task'. There can only be one active timer.
     br break                    - Pause timer
     co continue                 - Resume timer
    cur current                  - Print active timer info
      l log <task> <time>        - Log time
      u unlog <task> <time>      - Unlog time
     rn rename <task> <task2>    - Rename 'task' to 'task2'
     mr merge <task> <task2>     - Merge 'task' into 'task2': delete 'task' and add time logged in it to 'task2'.
    del delete <task>            - Delete 'task' log. If there was a timer for 'task', it gets discarded.
    rep report                   - Print logged time summary
     cl clear                    - Print summary and clear worklog
     st stop                     - Print summary and quit
      q quit                     - Quit
      h help                     - Print help
      v version                  - Print version

## ARGUMENTS
Time is passed like **\***, **\*h\*m**, **\*h**, or **\*m**, where **\*** is a non-negative number    
Task is a string containing max 31 characters from following list: **abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-/\:.**

To leave a comment use '#' symbol:    

    > # Comment
