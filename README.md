# Pestilence

# Sumary
- I. Usage.
- II. Description.
- III. How it works ?
- IV. Backdoor.

# This project has educationals purposes only
This is a self replicating virus with a non malicious code source, but malicous code can be added to Pestilence.
This project was done for educational purposes only, i disclaim the responsability if it used for malicous purposes.

# I. Usage:
TO DO

# II. Description:
This project is the following part of a group of projects of 42 schools:
- Woody_Woodpacker: an elf packer.
- Famine: a simple virus that infect elf files in a target directories.
- Pestilence: Famine's virus adding ofuscation methods.

When pestilence will be end, the next step will be code War a polymorphic virus and finally the last challenge is called Death, a virus that regroups all the properties of his ancestors. But adding the capacity to have a metamorphic code. But for the momment the goal of this project is to discover and understand the differents ways of ofuscation.

# III. How it works ?
(for the moment i ve only coded Famine, i haven t nothing about pestilence i m just using the readme for organise my future work on Pestilence)

  # a) Infection:
  - Pestilence will infect binaries using the basic method of change the entry point.
  - The virus will be placed in the padding after .text section (Have to try EPO tecnique for War seems better)

  # b) Ofuscation:
  For the moment all ofuscation methods will be anti-debug methods:
  - Anti-ptrace:
      Durring the infection routine a child process will be created with fork(). Use ptrace to attach this process to the parent
      process, if that fail it means the virus process (the process who have to be the parent) is not the parent. So virus could be
      launched into a debugger and we don't want that so we stop the process.
    
 - Specific process identification:
      If there are specific process runing the virus process will be stoped.
      
 - Hide process in task manager:
      I have to improve my knowless about that but in theory i can create a fork processs, setsid it for creaate a orphan process
      and not a zombie process. This orphan process will execute a normal process be for example a watcher and if i do it well i
      can use a pthread to execute the virus but in the task manager only the normal process (the watcher for example) will be seen.
    
   Can i add something than encrypts and decrips too i think.
    
# IV.Backdoor:
For the moment i have no idea how to do that but i will do it
