#Testing HAB on Teensy 4.0

High Assurange Boot (HAB) is not yet supported on Teensy 4.x.  This repository
is a collection of experiments to hopefully get it working.  Discussion is on
this forum thread.

https://forum.pjrc.com/threads/57173-Teensy-4-0-code-security

##Initial Setup

Download CST 3.3.0 or 3.3.1 from NXP's website.  Registration is required, but
they do not require a non-disclosure agreement to download CST.

Copy these files to the {CST}/keys folder

    SRK1_sha256_2048_65537_v3_usr_key.pem
    key_pass.txt
    SRK_fuse.bin
    SRK_table.bin

Copy these files to the {CST}/crts folder

    SRK1_sha256_2048_65537_v3_usr_crt.pem

These key files were created with the following commands.

    cd cst/keys
    ./hab4_pki_tree.sh -existing-ca n -use-ecc n -kl 2048 -duration 250 -num-srk 1 -srk-ca n
    ../linux64/bin/srktool -h 4 -t SRK_table.bin -e SRK_fuse.bin -d sha256 -c ../crts/SRK1_sha256_2048_65537_v3_usr_crt.pem

However, for the sake of testing and sharing results on the forum, please use the
generated keys from this repository so we all can get the same results.  Of course
for a real project you would want to generate your own encryption keys and never
share the SRK1_sha256_2048_65537_v3_usr_key.pem file with anyone!

Run "make" to build hex2bin, bin2hex, srkfuse

Run the teensy4_hab_write_srk.ino sketch on your Teensy 4.0 or 4.1 to write the
SRK key's hash into fuse memory.

##Usage

Compile the teensy4_hab_logfile sketch

Edit teensy4_hab.csf to experiment with Blocks setting.  See CST_UG.pdf section 5.2.7
on page 44-46 (54-56th pages of PDF) for details.

    ./hex2bin teensy4_hab_logfile.ino.hex
    ./cst/linux64/bin/cst -i teensy4_hab.csf -o hab_signature.bin
    cat teensy4_hab.ino.bin hab_signature.bin > teensy4_hab.bin
    ./bin2hex teensy4_hab.bin

Open teensy4_hab.hex with Teensy Loader and program it onto your Teensy 4.0 or 4.1.

Experiment with settings!  If you find any way to get HAB to report success, please
post the teensy4_hab.csf file and commands you used.
