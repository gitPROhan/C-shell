# Description
All commands have been implemented, keeping in mind the following assumptions:- 

# Assumptions
If multiple foreground processes taking >= 2 seconds are executed together, then the latest one is shown in the prompt.
Commands separated by ';' and '&' are executed sequentially, so they are updated as different pastevents.

There should be a space before and after every ';' and '&'.

Invalid commands should be avoided to let the shell function normally. (EXCEPT the ones which have been explicitly asked to show "Invalid command" on.)