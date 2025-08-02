# Installation

This file contains quick instructions for getting Weenix to run on
Redhat-derived or Debian-derived Linux flavors. If you're using a virtual machine with the Weenix Vagrantfile, the dependencies should be installed automatically when the machine is provisioned.

See also [Getting Started with Weenix](https://github.com/CMPS-6770/handout/wiki/Getting-Started-with-Weenix) for more thorough documentation.

1. Download and install dependencies.

   On recent versions of Ubuntu or Debian, you can simply run:

   ```bash
   $ sudo apt-get install gcc gdb qemu-system make python3 python3-pyelftools cscope xterm bash grub-pc-bin xorriso mtools
   ```

   or on Redhat:

   ```bash
   $ sudo yum install gcc gdb qemu-system make python3 python3-pyelftools cscope xterm bash grub-pc-bin xorriso mtools
   ```

2. Compile Weenix:

   ```bash
   $ make
   ```

3. Invoke Weenix:

   ```bash
   $ ./weenix -n
   ```

   or, to run Weenix under gdb, run:

   ```bash
   $ ./weenix -n -d gdb
   ```
