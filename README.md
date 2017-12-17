=====================================
# COPY AT YOUR OWN RISK !           
=====================================

### NACHOS-3 Commit Message
---

##### Assignment 3 : Complete

Memory Manager is completely re-written.

All the work has been done in "userprog" excepet for a tweak in "machine/translate.cc" in order to implement LRU page replacement scheme.

One important thing to note is that while reading in from the executable file for the first time in Task 2 (Demand Paging), it is not necessary to explicitly handle the six cases described in the guidlines. The "ReadAt" method takes care of all the cases.

I intend to upload a well-documented compilation of all the assignments (1, 2, 3) with problem descriptions. The chances are very slim, though; because I am lazy, VERY lazy.

If you are reading this commit message, chances are high that I have not uploaded a compilation of all the assignments. If that's the case, see commit:5 for assignment 1 only, and commit:7 for assignment 2 only.

My NACHOS Project (or assignment, or offlines, or... whatever) for Level-3, Term-2 is hereby, complete.

### NACHOS-2 Commit Message
---
##### Assignment 2 : Complete (with bonus task)

(Note : Some methods in "threads" folder (from assignment 1) have been renamed (for maintaining consistency). Not a big deal.

Task 1 : Address Space : Virtual Memory Support : Complete
Task 2 : System Call: Exec and Exit : Complete
Task 3 : System Call: Read and Write : Complete
Task 4 : User Exception Handling (Bonus) : Complete
I was too lazy to complete the "extra bonus task" (who cares, anyway?).

Almost all the work has been done in the "userprog" directory, with just a single modification in "threads/thread.h" (added field : threadID).

Circular dependency was a real pain in the ass to deal with during this assignment.

##### Note: After making any change, execute "rebuild". To run a program, execute "run program_name". Both are located in the "code" directory.


### NACHOS-1 Commit Message
---

##### NACHOS offline 1 : COMPLETE

Lock : Okay
Condition : Okay
Producer / Consumer : Okay

Next push will be for offline 2.