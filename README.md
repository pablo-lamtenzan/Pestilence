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
- Pestilence: Famine's virus adding ofuscation anti-debug methods.

Next projects:
  - War : add poliphormic ofuscation.
  - Death : add metaphormic ofucation.

# III. How it works ?

  # a) Infection:
  - Pestilence will infect binaries using the basic method of change the entry point.
  - The virus will be placed in the padding after .text section (Have to try EPO tecnique for War seems better)

  # b) Ofuscation:
 - Anti-ptrace:
      Check using ptrace function if Pestilence is run into a debuger.
 - Specific process identification:
      Check if specific not allowed processes are runnig at the same time of Pestilence.
 - Tracer checker:
      Check calling all needed times a deamon during the execution time if there a are a tracer tracing Pestilence
 - Encrypted hexdump:
      Some tracer checker and infection will be deamons. The hexdump needed for create the deamons is encrypted since the
      begin. It will be decripted with the same key and IV during the execution.
    
# IV.Backdoor:
For the moment i have no idea how to do that but i will do it
