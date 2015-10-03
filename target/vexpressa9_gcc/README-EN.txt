
                        README
           FMP porting for the QEMU Vexpress-A9 target

                2012 (C) Daniel Sangorrin
                daniel.sangorrin@gmail.com

--------
Overview
--------

TOPPERS/FMP (http://www.toppers.jp/en/fmp-kernel.html) is an RTOS with
support for multi-core processors. It is developed by the TOPPERS
project (http://www.toppers.jp/en/index.html) and distributed as open
source code through the TOPPERS license
(http://www.toppers.jp/en/license.html).

FMP follows the "TOPPERS New Generation Kernels" specification
(http://www.toppers.jp/documents.html). Contact me for an English
translation of the specification. A good way to understand FMP's API is
by looking at the sample program in the "fmp/sample" folder,
or the header files in "fmp/kernel/". (Hint: it is based on the uITRON4.0
interface).

The main characteristics of FMP are:

- It supports SMP and AMP configurations (however, in the case of AMP
all cores should be able to execute the same instruction set).
- Kernel and applications are linked in a single binary without
memory protection.
- Tasks are assigned to processor cores during the design phase. This is
done through a configuration file (.cfg) as shown in the sample program.
- The kernel will not automatically migrate tasks (i.e., load balancing)
during the execution of the system.
- However, the API allows migrating a task to a different processor core
at run time.
- The execution of FMP can be traced and displayed graphically through
the TLV tracelog visualizer (http://www.toppers.jp/tlv.html)
- Several target boards supported (ARM Cortex-A9, NIOS2, SH2A, SH4A)

This guide provides a quickstart for running FMP on the QEMU Vexpress-A9
target (a model of the ARM Versatile Express A9 board that includes
4 ARM Cortex-A9 cores).

THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.

----------------
Host environment
----------------

You need a UNIX-based execution environment for building FMP. For
example:

        - Ubuntu12.04LTS
        - Windows7+Cygwin
        - MacOSX.

The rest of the instructions will assume that you are using Ubuntu12.04LTS.

Applications and libraries that must be installed:

        - libboost libraries (>1.46)
        sudo apt-get install libboost libboost-regex-dev libboost-system-dev \
        libboost-filesystem-dev libboost-program-options-dev

        - ARM cross-compiler
        arm-none-eabi-gcc (Sourcery CodeBench Lite 2012.03-56) 4.6.3
        (http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/editions/lite-edition/)
        Note: other compilers are supported.

        - Essential software for building applications (make, etc.)
        sudo apt-get install build-essential

------------------
Build instructions
------------------

Download the FMP package for Versatile Express A9x4 and uncompress it:

$ tar zxvf fmp-vexpressa9.tar.gz
$ ls fmp/
arch  configure  include  library   obj   sample  target  ttsp
cfg   doc        kernel   MANIFEST  perf  README.txt     syssvc  test    utils

The folder "target" should contain the VEXPRESSa9-dependent code (BSP):

$ ls fmp/target/
vexpressa9_gcc

In order to build an application we first have to build the configuration
tool for FMP. This tool depends on the C++ Boost library so you will have
to install it too.

sudo apt-get install libboost libboost-regex-dev libboost-system-dev \
libboost-filesystem-dev libboost-program-options-dev

$ cd cfg/
$ ./configure
$ make depend
$ make

Note: contact me for an already built binary if you have difficulties
compiling the configurator.

Now we will create an application in the folder "obj/obj_sample" by using
the application template at "fmp/sample":

$ mkdir -p obj/obj_sample
$ cd obj/obj_sample
$ perl ../../configure -T vexpressa9_gcc -f

You can also specify the number of processors (by default 4, but you can
also use only 1 core) and other options in obj_sample/Makefile.

Now, we will build the "obj/obj_sample/" application.

$ cd obj/obj_sample/
$ make realclean
$ make ENABLE_G_SYSLOG=false PRC_NUM=4 KERNEL_FUNCOBJS=
...
./kernel/kernel_api.csv --cfg1-def-table ../../kernel/kernel_def.csv  --cfg1-def-table ../../arch/arm_gcc/mpcore/chip_def.csv --cfg1-def-table ../../arch/arm_gcc/common/core_def.csv sample1.cfg
check complete
arm-none-eabi-objcopy -O binary -S fmp

The resulting ELF binary (fmp) contains the kernel and the application
linked together. The next step is to load and run it on QEMU.

-----------------
Load instructions
-----------------

The current QEMU source code (7-dec-2012) contains a bug in the ARM GIC
model which can be fixed with the patch at target/vexpressa9_gcc.

wget http://wiki.qemu-project.org/download/qemu-1.3.0.tar.bz2
tar jxvf qemu-1.3.0.tar.bz2
cd qemu-1.3.0/
patch -p1 < ../fmp/target/vexpressa9_gcc/0002-ARM-GIC-bug-fixes-for-arm_gic.c.patch
./configure --target-list="arm-softmmu" --disable-strip --disable-xen --disable-kvm --disable-user --disable-docs --enable-debug --enable-debug-tcg
make
make install

For newer versions, it will hopefully be fixed at mainstream QEMU and then
you would just need to install QEMU as usual. For example, in Ubuntu use:

        $ sudo apt-get install qemu

Check that you have support for the Vexpress-A9 board:

$ qemu-system-arm -M ?
vexpress-a9          ARM Versatile Express for Cortex-A9

Finally, run the FMP binary on QEMU (tune the -smp option depending on
the application Makefile settings):

qemu-system-arm -cpu cortex-a9 -M vexpress-a9 -smp 4 -serial vc:80Cx40C -serial vc:80Cx40C -serial vc:80Cx40C -serial vc:80Cx40C -no-reboot -icount auto -m 1024M -kernel fmp

Then, use Ctrl+Alt+4,5,6,7 for switching between each virtual console.

---------------------------------------------
Load instructions using U-BOOT (experimental)
---------------------------------------------

If you prefer, you can load FMP from U-BOOT as well. However you need
to patch U-BOOT with the patch at target/vexpressa9_gcc so that other
cores wait in a holding pen:

tar zxvf u-boot-linaro-stable-5b752a1.tar.gz
cd u-boot-linaro-stable-5b752a1/
patch -p1 < 0001-VexpressA9-QEMU-modifications-for-loading-u-boot.patch
make distclean
make vexpress_ca9x4_config
make

We will download FMP from U-BOOT using TFTP, so you need to prepare
TFTP as well. For example, in Ubuntu 12.04 use:

sudo apt-get install tftpd-hpa
cat /etc/default/tftpd-hpa
# /etc/default/tftpd-hpa
TFTP_USERNAME="tftp"
TFTP_DIRECTORY="/var/lib/tftpboot"
TFTP_ADDRESS="0.0.0.0:69"
TFTP_OPTIONS="--secure"
sudo service tftpd-hpa restart

Compile an FMP application and set the flag ENABLE_UBOOT to true (as well
as ENABLE_QEMU):

cd fmp/obj_sample/
make TARGET=vexpressa9_gcc ENABLE_QEMU=true ENABLE_UBOOT=true PRC_NUM=4 ENABLE_G_SYSLOG=false fmp.bin
sudo cp fmp.bin /var/lib/tftpboot/

Finally, run U-BOOT and execute FMP:

qemu-system-arm -cpu cortex-a9 -M vexpress-a9 -smp 4 -serial vc:80Cx40C -serial vc:80Cx40C -serial vc:80Cx40C -serial vc:80Cx40C -no-reboot -icount auto -m 1024M -kernel u-boot -tftp /var/lib/tftpboot/
Ctrl+Alt+5

VExpress# tftp 0x60000000 fmp.bin
go 0x60000000

-----------------------
Pass the TTSP test suite
------------------------

TTSP is a test suite developed for easying the verification of
TOPPERS kernels (http://www.toppers.jp/en/ttsp.html). Download
TTSP and uncompress it inside the FMP directory:

$ tar zxvf ttsp.tar.gz
$ ls
api_test  configure.sh  docs  library  scripts  sil_test  tools  ttb.sh  user.txt

The TTSP package should come with support for the Vexpress-A9 target:

$ ls library/FMP/target/
vexpressa9_gcc

(Note: if your TTSP version does not contain the vexpressa9_gcc folder,
please contact with me.)

The main modifications to the TTSP that were carried for the
VexpressA9 are contained in the following files:

fmp/ttsp/library/FMP/arch
fmp/ttsp/library/FMP/target/vexpressa9_gcc/

Make sure that TARGET_NAME="vexpressa9_gcc" and PROFILE_NAME="FMP" in
fmp/ttsp/configure.sh; and that in fmp/target/vexpressa9_gcc/Makefile.target
the Cortex-A9 private timers are used (instead of the watchdog timers):

        SYSTIMER_TYPE = CA9_PRIVATE_TIMER
        KERNEL_COBJS := $(KERNEL_COBJS) chip_timer.o

You need to install ruby in order to run the TTSP tools.
The steps for running the API tests are:

$ ./ttb.sh
        - Select 1 (API tests)
        - Select 1 (Auto-code tests)
        - Select 1 (Generate a MANIFEST file for all TESRYs)
        - Select 4 (Build all program files)

This will generate a big fmp.bin inside the fmp/ttsp/obj/ folder
that you have to run with u-boot. When the program is run it
will display which tests are passed.

Note 1: you can do the tests in parts by
using the "Generate a MANIFEST file divided by specified number"
or a "MANIFEST file for each functions".

Note 2: you can also do tests for the SIL or the common functions
for timer, interrupts and exceptions.
