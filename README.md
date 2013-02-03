CS340-Spring-2013
=================

Git repository for CS340 group during the Spring 2013 semester.


ascii-only
----------

./ascii-only ascii-only.c

<big>Compiling:</big>

Without debug symbols:
gcc -Wall -g -o ascii-only ascii-only.c

With debug symbols:
gcc -Wall -gdwarf-3 -o ascii-only ascii-only.c


Requested fixes & enhancements:
-------------------------------

<big>1) Work on MIPS!</big>

qemu-system-mips -m 256M -M malta -kernel vmlinux-2.6.26-2-4kc-malta \
    -hda debian_lenny_mips_standard.qcow2 -append "root=/dev/hda1 \
    console=ttyS0" -nographic   
                                                                                
-m         : ram                                                                
-M         : machine                                                            
-hda       : disk image                                                         
-append    : kernel command line                                                
-nographic : no graphical output                                                
                                                                                
malta refers to The MIPS Malta prototype board 

-----------------------------
<big>2) Better error messages...</big>

file not found

can't open file

usage

<big>3) Specify infile and outfile</big>

<big>4) Specify log file</big>

infile

outfile

date

line:character_position non-ascii (in hex or decimal)


<big>5) Option for skiping or replacing non-ascii</big>

can specify the replacement as char, decimal, or hex