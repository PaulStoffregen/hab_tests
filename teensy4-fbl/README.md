# Teensy 4.x flash bootloader (FBL)
This folder contains several different sub-projects arround the teensy 4.x flash bootloader (FBL). This was developed in preparation for a secured Teensy to become possible.

The aim of this repository is to have a point of last resort, if something went wrong during experimentation with the High Assurance Boot (HAB) features of the iMX-RT1062 processor.
It is also mentioned to be the basis of an updater for a possible secured Teensy 4.x.

## What's in the repository?

### Firmware side applications

#### The flash bootloader
The most important firmware side application is the flash bootloader itself and can be found under application/fbl.

#### A test application firmware image
The aim of the test application firmware image is to verify that the FBL together with the FBLTool is able to update the actual application firmware image. And that
the FBL can successfully run that image. The so called FlashApp can be found under driver/startup/imxrt/flash-app.

### Host side application
In order to communicate with the FBL on the Teensy there is a host application, which can be found under tools/fblTool.

### Tooling

#### Signing
For an IMXRT processor set to closed secure configuration it is necessary for at least the primary firmware image to be properly signed. To help users with the process of signing,
a perl script has been added which takes this responsibility. This scrpt can be found under tools/hab-sign.

#### Protection
As the FBL is intedend receive application firmware images via UART and write them to the external flash, it is a good idea to have some sort of protection against communication errors.
So there is a CRC32 patched into the application firmware image for that purpose, which is done by another perl script, which can be found under tools/patchCrc.

## Build environment
The environment in which all this was developed is a PC running Windows 10. For the tooling it is running a MinGW/MSys environment, a Perl installation, GIT and the
NXP code signing tools CST-3.3.1. Host application is built using the MinGW compiler and the target applications are built using a Sourcery ARM compiler of 2014.05.
In general all projects have a Makefile, defining what to include in the build. Additionally thera are cmd_shell.bat files opening a windows CLI window in the proper
working directory and setting up the necessary environment variables. There is a file mk/setenv.bat that contains the necessary environment settings which need to be
adapted. The main settings are:
- Definition of the location of the MinGW compiler and adding this location to the PATH variable
- Definition of the location of the MSys environment root and adding this locaion to the PATH variable
- Definition of the location of the GIT application folder and adding it to the PATH variable
- Definition of the location of the CST tool folder and adding it to the PATH variable

Optionally you may extend the PATH variable for the Perl installation you want to use.

The path and name prefix of the compiler used for cross compilation is given via environment variable CROSSCOMPILE.

I tried to build the firmware images also using the one which is part of the teensy-duino delivery both without success.
It always tries to link with libc.a which the linker is not able to find, even though the linker is called with -nostdlib.

## How to build
In general you navigate to the desired folder and run the batch file by double click. In the open CLI window you type your GNU make command. Afterwards the results show up
in the output folder which is in normally: ./__out__/imxrt.

### Build the FBL
Run application/fbl/cmd_shell.bat
```
$>make PLATFORM=IMXRT CROSSCOMPILE=C:\dvpt\tools\gcc-linaro-7.5.0-2019.12-i686-mingw32_arm-eabi\bin\arm-eabi- version bin postlink
```
The goal "version" will create a file version.h with up to date version information including hash, date, time and revision number of the last commit and date and time of the build.
The goal "bin" will firstly compile and link all required files into "fbl.elf" and secondly create "fbl.bin" using "objcopy".
The goal "postlink" will take "fbl.bin", expand and place it under "./production" within the CST root folder, sign the expanded image optionally further expand it and place
the the result back in the output folder as "image_exp.bin". This is then taken to be patched with a CRC32 and stored as "image_exp.fbl". In a final step that image is taken
and transformed into "image_exp.hex", which can be programmed via "teensy_loader_cli.exe".

### Build the FBLTool
Run tools/fbltool/cmd_shell.bat
```
$>make PLATFORM=MINGW all
```
The goal creates "fblTool.exe" within the output directory.
 
### Build the FlashApp
Run driver/startup/cmd_shell.bat
```
$>make -C imxrt/flash-app PLATFORM=IMXRT CROSSCOMPILE=C:\dvpt\tools\gcc-linaro-7.5.0-2019.12-i686-mingw32_arm-eabi\bin\arm-eabi- version bin postlink
```
Here again the goal "version" creates a file "version.h" with up to date version information. The goal "bin" creates "app.elf" and "app.bin". And finally goal "postlink" creates
"image_exp.fbl" which is inteded to be programmed via FBL.

## How to program

### Program the FBL
Once a signed FBL image has been created, it can be programmed using the "teensy_loader_cli.exe". Once programmed you can use the command line accessible via UART4 (mapped on
pins 16 and 17) to get target information, write the SRK Hash matching the testing keys created by Paul, which is embedded in the FBL. You can also examine the SNVS status,
stepwise disable the JTAG access and close the secure configuration. More on this in the description about the command line interface. The FBL also configures UART1 (mapped on
pads 24 and 25 for communication with the FBLTool, so on this channel you can instruct the FBL to program an application binary image. Once such an image is programmed, after
reset the FBL will wait for currently 10s to receive a command and if nothing is received it will in turn authenticate the application binary image and on success jump to this
image.

### Program the FlashApp
Once a signed FlashApp image has been ccreated, it can be programmed using the FBLTool only. So the FBLTool is running on a host PC creating a console window wher you can run
several commands. More on this in the description about the FBLTool. One of the commands is "update" which does run the whole programming sequence with the FBL running at the
Teensy downloading that image whos name was given to the executable as a parameter. 

## Using the FBLTool
The FBLTool is an executable application whos user interface is a console window. It implements the protocol layer for communication with the FBL, several commands which
define the building blocks of the update protocol and a sequencer running those commands in the proper order.
The executable has a few command line arguments which are as follows:
```
  -c<num>          use com-port <num>
  -b<baudrate>     set baudrate
  -f <image-file>  file to be programmed
```
Currently only a static baudrate of 115200 kbps is supported.

If run without any arguments only a help text will be shown before the applicaltion terminates. Once started with the proper parameters, you can see a command prompt
like this:
```
FBLTOOL>
```
Following commands are implemented:
```
General commands
  ?
  help     print help text
  open     attach to the FBL during the first 10s after reset
  info     show some information about the SW programmed to the target
Command sequence
  update   run the whole update sequence
Single commands
  inv       invalidate current application SW
  erase     erase current application SW
  program   program a few dummy blocks 
  activate  activate the new application SW
  restart   reset and restart the target
  reboot    reset and restart the target
```

## Using the FBL's commandline
The FBL itself also provides a commandlin interface via UART, so you can use a terminal to gain access to this interface. Make sure your terminal adds carridge return to every
received newline. The command prompt shows up with a short message which looks like this:
```
Hello FBL
FBL>
```

Following commands are implemented:
```
General commands
  help       print help text
  boot       stop the count down after reset and enter the FBL
Processor information commands
  cpuID      show CPU UID
  rvtInfo    show processor's RVT info
  habInfo    show processor's HAB version
Fuse information commands
  lockInfo   show lock fuse info
  otpInfo    show processor's fuses
  srkInfo    show processor's SRK hash
Firmware image information commands
  fblInfo    show FBL's SW version info
  appInfo    show FlashApp's SW version info
  ldInfo     show FBL's linkage info
  ivtInfo    show FBL's IVT info
  csfInfo    show FBL's CSF info
Boot and security related information commands
  bootInfo   show boot related info form SRC
  habCheck   show HAB status summary
  habStatus  show HAB status log
  snvsInfo   show contents SNVS registers
  scsInfo    show contents of register OCOTP.SCS
  enaJtag    re-enable JTAG access for secured or disabled JTAG
Fuse programming commands:
  wrSrkHash  burn the SRK hash fuses
  wrSecCfg   burn the SEC_CONFIG fuse
  wrSjcRsp   burn the SJC_RESPONSE fuses
  wrSecJtag  burn the JTAG_SMODE fuses to secure JTAG and the KTE fuse
  wrDisJtag  burn the JTAG_SMODE fuses to JTAG disabled
  wrDisSjc   burn the SJC_DISABLE fuse
```