#!/usr/bin/perl -w

#####################################################################
#
#####################################################################

use strict;
use File::Basename;
use Switch;

# Create command arguments from arguments given to this scipt and undefine
# the script's arguments
my @cmdArgs = @ARGV;
undef @ARGV;

# Definition of all functions used by this script


my $HAB_TAG_IVT = 0xd1;       #/*!< Image Vector Table */
my $HAB_TAG_DCD = 0xd2;       #/*!< Device Configuration Data */
my $HAB_TAG_CSF = 0xd4;       #/*!< Command Sequence File */
my $HAB_TAG_CRT = 0xd7;       #/*!< Certificate */
my $HAB_TAG_SIG = 0xd8;       #/*!< Signature */
my $HAB_TAG_EVT = 0xdb;       #/*!< Event */
my $HAB_TAG_RVT = 0xdd;       #/*!< ROM Vector Table */
my $HAB_TAG_WRP = 0x81;       #/*!< Wrapped Key */
my $HAB_TAG_MAC = 0xac;       #/*!< Message Authentication Code */

my $HAB_CMD_SET_ITEM = 0xb1;  #/**< Set Item */
my $HAB_CMD_INS_KEY = 0xbe;   #/**< Install Key */
my $HAB_CMD_AUTH_DATA = 0xca; #/**< Authenticate Data */
my $HAB_CMD_WRT_DATA = 0xcc;  #/**< Write Data */
my $HAB_CMD_CHK_DATA = 0xcf;  #/**< Check Data */
my $HAB_CMD_NOP = 0xc0;       #/**< No Operation */
my $HAB_CMD_INIT = 0xb4;      #/**< Initialize */
my $HAB_CMD_UNLOCK = 0xb2;    #/**< Unlock */

my $HAB_PCL_SRK = 0x03;
my $HAB_PCL_X509 = 0x09;
my $HAB_PCL_CMS = 0xc5;
my $HAB_PCL_BLOB = 0xbb;
my $HAB_PCL_AEAD = 0xa3;

sub TRACE_INFO
{
  printf("");
}


###############################################################################
#
###############################################################################
#
#
###############################################################################

sub hab_getTagStr
{
  my $tag = $_[0];

  my $tagStr;  
  switch($tag)
  {
  case ($HAB_TAG_IVT) {$tagStr = "IVT";}
  case ($HAB_TAG_DCD) {$tagStr = "DCD";}
  case ($HAB_TAG_CSF) {$tagStr = "CSF";}
  case ($HAB_TAG_CRT) {$tagStr = "CRT";}
  case ($HAB_TAG_SIG) {$tagStr = "SIG";}
  case ($HAB_TAG_EVT) {$tagStr = "EVT";}
  case ($HAB_TAG_RVT) {$tagStr = "RVT";}
  case ($HAB_TAG_WRP) {$tagStr = "WRP";}
  case ($HAB_TAG_MAC) {$tagStr = "MAC";}
  else                {$tagStr = "Unknown!";}
  }
  return $tagStr;
}


###############################################################################
#
###############################################################################
#
#
###############################################################################

sub hab_getCmdStr
{
  my $cmdTag = $_[0];

  my $cmdStr;
  switch($cmdTag)
  {
  case ($HAB_CMD_SET_ITEM)  {$cmdStr = "SET_ITEM";}
  case ($HAB_CMD_INS_KEY)   {$cmdStr = "INS_KEY";}
  case ($HAB_CMD_AUTH_DATA) {$cmdStr = "AUTH_DATA";}
  case ($HAB_CMD_WRT_DATA)  {$cmdStr = "WRT_DATA";}
  case ($HAB_CMD_CHK_DATA)  {$cmdStr = "CHK_DATA";}
  case ($HAB_CMD_NOP)       {$cmdStr = "NOP";}
  case ($HAB_CMD_INIT)      {$cmdStr = "INIT";}
  case ($HAB_CMD_UNLOCK)    {$cmdStr = "UNLOCK";}
  else                      {$cmdStr = "Unknown!";}
  }
  return $cmdStr;
}


###############################################################################
#
###############################################################################
#
#
###############################################################################

sub hab_getProtocolStr
{
  my $pclTag = $_[0];

  my $protocolStr;
  switch($pclTag)
  {
  case ($HAB_PCL_SRK)  {$protocolStr = "SRK";}
  case ($HAB_PCL_X509) {$protocolStr = "X509";}
  case ($HAB_PCL_CMS)  {$protocolStr = "CMS";}
  case ($HAB_PCL_BLOB) {$protocolStr = "BLOB";}
  case ($HAB_PCL_AEAD) {$protocolStr = "AEAD";}
  else                 {$protocolStr = "Unknown!";}
  }
  return $protocolStr;
}


###############################################################################
#
###############################################################################
#
#
###############################################################################

sub dumpCsf
{
  my $buf;
  my $csfData;
  my $csf = {};
  my $cmd = {};
  # Extract header value
  $csf->{"header"} = unpack("V", $buf);

  # Extract tag, length and version as "U8:U16be:U8"
  ($csf->{"tag"}, $csf->{"size"}, $csf->{"ver"}) = unpack("C n C", $buf);

  # Gather major and minor version
  $csf->{"majVer"} = $csf->{"ver"} >> 4;
  $csf->{"minVer"} = $csf->{"ver"} & 0x0F;

  # Check validity of CSF
  if($csf->{"tag"} != 0xD4)
  {
    # Invalid CSF tag
    print("Invalid CSF header (TAG)\n");
  }
  elsif($csf->{"majVer"} != 4)
  {
    # Invalid major version
    print("Invalid CSF header (VER)\n");
  }
  else
  {
    printf("Valid CSF with size: 0x%08X (%d Bytes)\n", $csf->{"size"}, $csf->{"size"});
  }

  # Extract CSF commands
  my $offs = 4;
  # Extract header value
  ($cmd->{"header"}) = unpack("x".$offs." V", $csfData);
  # Extract tag, length and param as "U8:U16be:U8"
  ($cmd->{"tag"}, $cmd->{"size"}, $cmd->{"flags"}) = unpack("x".$offs."C n C", $csfData);

  printf("Command:\n");
  printf(" Tag:   0x%02X -> %s\n", $cmd->{"tag"}, hab_getCmdStr($cmd->{"tag"}));
  printf(" Size:  0x%04X -> %d Bytes\n", $cmd->{"size"}, $cmd->{"size"});
  printf(" Flags: 0x%02X\n", $cmd->{"flags"});

  $offs += 4;
  ($cmd->{"pcl"}, $cmd->{"alg"}, $cmd->{"src"}, $cmd->{"tgt"}, $cmd->{"keyAddr"}) = unpack("x".$offs."C C C C N", $csfData);
  printf(" Proto:   0x%02X -> %s\n", $cmd->{"pcl"}, hab_getProtocolStr($cmd->{"pcl"}));
  printf(" Algo:    0x%02X\n", $cmd->{"alg"});
  printf(" Src:     0x%02X\n", $cmd->{"src"});
  printf(" Tgt:     0x%02X\n", $cmd->{"tgt"});
  printf(" KeyAddr: 0x%08X\n", $cmd->{"keyAddr"});
}


###############################################################################
#
###############################################################################
#
#
###############################################################################

sub getCwd
{
  my @result;
  my $cmdStr = "pwd";
  my $pipe;
  my $line;
  my $pid;
  
  $pid = open($pipe, $cmdStr." |");
  while($line = <$pipe>)
  {
    # remove newline
    chomp($line);
    push(@result, $line);
  }
  close($pipe);
  return @result;
}


###############################################################################
#
###############################################################################
#
#
###############################################################################

sub signImage
{
  my @result;
  my $cstProc;
  my $exit;
  my $line;
  my $pid;
  my $dir;
  
  
  @result = getCwd();
  $dir = $result[0];
  chdir($ENV{'CST_HOME'}."/production");
  @result = getCwd();
  print("Executing CST-Tool from within ".$result[0]."\n");
  
  $pid = open($cstProc, "cst.exe -i cst.conf -o csf.bin |");

  while($line = <$cstProc>)
  {
    # remove newline
    chomp($line);
    # push to the result array
    push(@result, $line);
  }

  # Close the child process
  close($cstProc);

  # Examine the exit code of the child process
  $exit = $?;

  if($exit & 0xFF)
  {
    # The child process failed
    if($exit & 0x80)
    {
      warn("$0: pipe failed with core dump: exit=".($exit >> 8)." signal=".($exit & 0x7F)."\n");
    }
    else
    {
      warn("$0: pipe failed: exit=".($exit >> 8)." signal=".($exit & 0x7F)."\n");
    }

    if(@result)
    {
      die("failed with the following output:\n", join("\n", @result), "\n");
    }
    else
    {
      die("failed without any output.\n");
    }
  }
  print("Output:\n  ", join("\n  ", @result), "\n");
  chdir($dir);
}


###############################################################################
#
###############################################################################
#
#
###############################################################################

sub makeHex
{
  my $srcPath;
  my $srcImgFile;
  my $dstImgFile;
  my $entryAddr;
  my $baseAddr;
  my @result;
  my $argList;
  my $pipe;
  my $exit;
  my $line;
  my $pid;
  my $dir;
  
  ($srcPath, $srcImgFile, $dstImgFile, $entryAddr, $baseAddr) = @_;
  @result = getCwd();
  print("Executing objcpy from within ".$result[0]."\n");
  printf("  Entry:     0x%08X\n", $entryAddr);
  printf("  Base:      0x%08X\n", $baseAddr);
  $entryAddr -= $baseAddr;
  
  $argList = "-I binary -O ihex --set-start ".$entryAddr." --change-address ".$baseAddr." ".$srcPath.$srcImgFile." ".$srcPath.$dstImgFile;
  $pid = open($pipe, "objcopy ".$argList."|");

  while($line = <$pipe>)
  {
    # remove newline
    chomp($line);
    # push to the result array
    push(@result, $line);
  }

  # Close the child process
  close($pipe);
  print("\nOutput:\n  ", join("\n  ", @result), "\n\n");
}


# --ivt-offs=0x00000000
# --csf-align=0x1000
# --crc-offs=0x208
# --csf-pad=
# --srk-tbl=
# --noca-key=
# --csf-key=
# --img-key=
# --srk-idx=

# Define help message for this script
my $helpMessage = <<"END_HELP";
 --input-file=<in-file-name>
 --input-dir=<input-directory>
 --expand-file=<file-name>
 --output-file=<out-file-name>
 --output-cfg=<out-cfg-file-name>
 --image-type=MFG|MMC|BLOB
 --ivt-offs=0x00001000
 --crc-offs=<crc-offset>
 --srk-tbl=<srk-tbl-file-name>
 --srk-idx=<skr-index>
END_HELP

# The image type determines the following aspects of the image signing:
# MFG:
#   An MFG image is expected to already contain a valid IVT at the offset
#   given by --ivt-offs. The offset depends on the target processor and
#   storage media.
#   As the image is split by the MFG-tool into 2 components during download,
#   with the first one being the DCD downloaded to and executed from a
#   target processor specific address, the signing target address needs to
#   be adapted appropriately in the generated CST configuration.
#   Also in order to not execute the DCD twice, the second component is the
#   remaining image with the DCD address in the IVT set to 0, which must
#   be taken into account before signature is calculated. Thus a modified
#   binary image is created to be feed into CST, which has the DCD address
#   set to 0.
# MMC:
#   A MMC image is expected to already contain a valid IVT at the offset
#   given by --ivt-offs. The offset depends on the target processor and
#   storage media.
#   A MMC image may or may not have a DCD. Anyway, if loaded from storage
#   media, the DCD is executed from the natural place given by image. So
#   the image doesn't need a modification of neither the IVT nor the DCD
#   address.
# BLOB:
#   A BLOB image is expected to have no IVT so the script will built an
#   IVT and prepend it to the given blob.
#   A BLOB image is expected to have no DCD at all.
#   A BLOB image is expected to have a size not necessarily aligned to any
#   specific boundary. As we want to align the start of the CSF to a
#   boundary given by --csf-align the a modified binary image is created to
#   be fed into CST, which adds some padding between the actual end of the
#   input image and the wished boundary.
#   As a BLOB image doesn't have an IVT it also has no load address and thus
#   that load address must be specified via --load-addr.

# Define the script's main function
# 
sub genSignCfg
{
  # Get arguments and argument count
  my @args = @_;
  my $argc = @args;

  my $i;
  # Name of image to be signed
  my $srcImgName = "image.bin";
  # Path to image to be signed
  my $srcImgPath;
  my $expImgName = "image_exp.bin";
  my $expHexName = "image_exp.hex";
  my $expImgPath = $ENV{'CST_HOME'}."/production/";
  # Name of binary file used for signing
  my $dstImgName = "image_strip.bin";
  my $dstImgPath = $ENV{'CST_HOME'}."/production/";
  # Name of CST config file
  my $cstConf = "cst.conf";
  my $csfImgName = "csf.bin";

  my $imgType = "MMC";
  # Address of DCD in target memory
  my $dcdAddr = 0x00910000;
  # Multi-line string containing the CST config
  my $cstConfig;
  # Array getting the blocks definitions for the CST config
  my @blocks = {};

  # Size of input binary image
  my $imgSize;
  # Size of expanded output binary image
  my $expSize;
  # Size of padding to fit the alignment
  my $padSize;
  
  # Read pointer for given binary file
  my $imgPos;
  # Buffer used for read data from given binary file
  my $buf;

  # Buffer to hold read partition data
  my $partData = "";
  # Buffer to hold read, modified or generated IVT data
  my $ivtData = "";
  # Buffer to modified for signing IVT data
  my $sigIvtData = "";
  # Buffer to hold read, modified or generated boot data
  my $bootData = "";
  # Buffer to hold read DCD data
  my $dcdData = "";
  # Buffer to hold read application data
  my $appData = "";
  # Buffer to hold generated application padding data
  my $appPad = "";
  # Buffer to hold generated CSF data
  my $csfData = "";
  # Buffer to hold generated CSF padding
  my $csfPad = "";

  # Offset of the CRC within the given binary file.
  my $crcOffs;
  # Offset of the IVT within the given binary file.
  my $ivtOffs = 0x00001000;
  # Offset to the BOOT data within the given binary file.
  my $bootOffs;
  # Offset to the DCD within in the given binary file.
  my $dcdOffs;

  # Size of CRC
  my $crcSize;
  # Size of HAB header
  my $HAB_HDR_SIZE = 0x04;
  # Size of IVT data structure
  my $HAB_IVT_SIZE = 0x20;
  # Size of boot data structure
  my $HAB_BOOT_SIZE = 0x0C;
  # Size of DCD in source image if available
  my $dcdSize = 0;

  # ...
  my $blobLoadAddr = 0;
  my $bootPad = 0x04;
  my $csfAlign = 0x400;
  my $csfSize = 0x2000;

  # Struct to store IVT values
  my $ivt = {};
  # Struct to store boot values
  my $boot = {};
  # Struct to store DCD values
  my $dcd = {};

  my $srkNonCA = 0;
  my $srkTblFileName;
  my $srkIdx = 0;
  my $nocaKeyFileName;
  my $imgKeyFileName;
  my $csfKeyFileName;
  my $csfkSlot = 0;
  my $imgkSlot = 1;

  my $imgFile;
  my $expFile;
  my $ivtFile;
  my $cstFile;
  my $csfFile;

  my $file;
  my $path;
  my $suffix;

  if($argc > 0)
  {
    my $argID;
    my $argStr;
    # Parse arguments
    for($argID = 0; $argID < $argc; $argID++)
    {
      $argStr = $cmdArgs[$argID];
      if($argStr =~ m/--input-file=(.+)/)
      {
        ($file, $path, $suffix) = fileparse($1, qr/\.[^.]*/);
        $srcImgName = $file.$suffix;
        $srcImgPath = $path;
      }
      elsif($argStr =~ m/--ivt-offs=(.+)/)
      {
        my $offs = $1;
        if($offs =~ m/^0x([a-fA-F0-9]+)$/)
        {
          $ivtOffs = hex($1);
        }
        elsif($offs =~ m/^([0-9]+)$/)
        {
          $ivtOffs = $1;
        }
        else
        {
          print("Invalid format for IVT offset: ".$offs."\n");
          exit(-1);
        }
      }
      elsif($argStr =~ m/--crc-offs=(.+)/)
      {
        my $offs = $1;
        if($offs =~ m/^0x([a-fA-F0-9]+)$/)
        {
          $crcOffs = hex($1);
          $crcSize = 4;
        }
        elsif($offs =~ m/^([0-9]+)$/)
        {
          $crcOffs = $1;
          $crcSize = 4;
        }
        else
        {
          print("Invalid format for CRC offset: ".$offs."\n");
          exit(-1);
        }
      }
      elsif($argStr =~ m/--srk-tbl=(.+)/)
      {
        $srkTblFileName = $1;
      }
      elsif($argStr =~ m/--srk-idx=(.+)/)
      {
        my $idx = $1;
        if($idx =~ m/^([0-3])$/)
        {
          $srkIdx = $1;
        }
        else
        {
          print("Invalid SRK index: ".$idx."\n");
          print(" Allowed values: 0..3\n");
          exit(-1);
        }
      }
      elsif($argStr =~ m/--noca-key=(.+)/)
      {
        $nocaKeyFileName = $1;
        $srkNonCA = 1;
      }
      elsif($argStr =~ m/--csf-key=(.+)/)
      {
        $csfKeyFileName = $1;
      }
      elsif($argStr =~ m/--csfk-slot=(.+)/)
      {
        my $slot = $1;
        if($slot =~ m/^([0-3])$/)
        {
          $csfkSlot = $1;
        }
        else
        {
          print("Invalid CSFK slot: ".$slot."\n");
          exit(-1);
        }
      }
      elsif($argStr =~ m/--img-key=(.+)/)
      {
        $imgKeyFileName = $1;
      }
      elsif($argStr =~ m/--imgk-slot=(.+)/)
      {
        my $slot = $1;
        if($slot =~ m/^([2-4])$/)
        {
          $imgkSlot = $1;
        }
        else
        {
          print("Invalid IMGK slot: ".$slot."\n");
          print(" Allowed values: 2..4\n");
          exit(-1);
        }
      }
      elsif($argStr =~ m/--image-type=(.+)/)
      {
        $imgType = $1;
        if($imgType =~ m/MFG/)
        {
          $dcdAddr = 0x00910000;
        }
        elsif($imgType =~ m/MMC/)
        {
          $dcdAddr = 0x00000000;
        }
        elsif($imgType =~ m/BLOB/)
        {
          $dcdAddr = 0x00910000;
        }
      }
    }
  }
  else
  {
    print("$helpMessage\n");
    return -1;
  }
  
  # Check if source image exists
  if(-e "$srcImgPath/$srcImgName")
  {
    # Get image size
    $imgSize = (-s "$srcImgPath"."$srcImgName");
  }
  else
  {
    print("Source image ".$srcImgPath.$srcImgName." doesn't exist.\n");
    exit(-1);
  }

  # Open the binary image for read
  open($imgFile, "<", "$srcImgPath"."$srcImgName") or die "Failed to open: $srcImgPath"."$srcImgName $!";
  binmode($imgFile);

  print("\nGenerating signing request...\n");
  printf("  FileName:  %s\n", $srcImgPath.$srcImgName);
  printf("  FileSize:  0x%08X (%d Bytes)\n", $imgSize, $imgSize);


  # Preset expanded size with image size
  $expSize = $imgSize;

  # Clear image position pointer
  $imgPos = 0;
  if($imgType =~ m/BLOB/)
  {
    # Image type is BLOB
    $expSize += $HAB_IVT_SIZE;  # Need to generate and add IVT
    $expSize += $HAB_BOOT_SIZE; # Need to generate and add BOOT data
    $expSize += $bootPad;       # Add boot padding if desired
  }
  else
  {
    # Other image type
  }
  
  # Calculate space to fill before the CSF in order to fit alignment
  $padSize = ($csfAlign - ($expSize % $csfAlign)) % $csfAlign;

  # Calculate final expanded size
  $expSize += $padSize;

  if($padSize != 0)
  {
    print("\nExpanding image...\n");
    printf("  ExpSize = 0x%08X (%d Bytes)\n", $expSize, $expSize);
    # Fill the padding buffer
    for($i = 0; $i < $padSize; $i++)
    {
      $appPad .= pack("C", 0x5A);
    }
  }
  else
  {
    print("\nNo expansion necessary...\n");
  }


  if($imgType =~ m/BLOB/)
  {
    # Image type is BLOB

    # Clear out partition data
    $partData = "";

    # Create appropriate IVT content
    $ivt->{"tag"} = 0xD1;
    $ivt->{"size"} = $HAB_IVT_SIZE;
    $ivt->{"ver"} = 0x40;
    $ivt->{"header"} = unpack("V", pack("C n C", $ivt->{"tag"}, $ivt->{"size"}, $ivt->{"ver"}));
    $ivt->{"selfAddr"} = $blobLoadAddr;
    $ivt->{"csfAddr"} = $ivt->{"selfAddr"} + $expSize;
    $ivt->{"bootAddr"} = $ivt->{"selfAddr"} + $HAB_IVT_SIZE;
    $ivt->{"entryAddr"} = $ivt->{"bootAddr"} + $HAB_BOOT_SIZE + $bootPad;
    $ivt->{"dcdAddr"} = 0;

    # Print IVT info
    printf("\nGenerate image vector table (IVT)...\n");
    printf("  Header:    0x%08X (Tag=0x%02X Len=0x%04X (%d Bytes), Ver=0x%02X\n", $ivt->{"header"}, $ivt->{"tag"}, $ivt->{"size"}, $ivt->{"size"}, $ivt->{"ver"});
    printf("  Entry:     0x%08X\n", $ivt->{"entryAddr"});
    printf("  DCD:       0x%08X\n", $ivt->{"dcdAddr"});
    printf("  Boot Data: 0x%08X\n", $ivt->{"bootAddr"});
    printf("  Self:      0x%08X\n", $ivt->{"selfAddr"});
    printf("  CSF:       0x%08X\n\n", $ivt->{"csfAddr"});

    # Create appropriate boot data content
    $boot->{"startAddr"} = $blobLoadAddr;
    $boot->{"imgSize"} = $expSize + $csfSize;
    $boot->{"plugin"} = 0;
  
    # Print boot data info
    printf("\nGenerate boot data...\n");
    printf("  Start:     0x%08X\n", $boot->{"startAddr"});
    printf("  Length:    0x%08X\n", $boot->{"imgSize"});
    printf("  Plugin:    0x%08X\n\n", $boot->{"plugin"});
    
    # Clear out DCD data
    $dcdData = "";
  }
  else
  {
    # Other image type

    # Read partition data which is all data up to $ivtOffs
    # This might be nothing if $ivtOffs is 0
    $imgPos += read($imgFile, $buf, $ivtOffs);
    # Save partition data for later reference
    $partData = $buf;

    # Here we expect to read the IVT
    $imgPos += read($imgFile, $buf, $HAB_IVT_SIZE);
    # Save IVT data for later reference
    $ivtData = $buf;


    # Extract header value
    $ivt->{"header"} = unpack("V", $buf);
    # Extract tag, length and version as "U8:U16be:U8"
    ($ivt->{"tag"}, $ivt->{"size"}, $ivt->{"ver"}) = unpack("C n C", $buf);
    # Gather major and minor version
    $ivt->{"majVer"} = $ivt->{"ver"} >> 4;
    $ivt->{"minVer"} = $ivt->{"ver"} & 0x0F;

    # Check validity of IVT
    if($ivt->{"tag"} != 0xD1)
    {
      # Invalid IVT tag
      print("Invalid IVT header (TAG) at offset: ".$ivtOffs."\n");
      exit(-1);
    }
    elsif($ivt->{"size"} != $HAB_IVT_SIZE)
    {
      # Invalid IVT len
      print("Invalid IVT header (LEN) at offset: ".$ivtOffs."\n");
      exit(-1);
    }
    elsif($ivt->{"majVer"} != 4)
    {
      # Invalid major version
      print("Invalid IVT header (VER) at offset: ".$ivtOffs."\n");
      exit(-3);
    }
    else
    {
      # IVT is valid so extract IVT fields
      ($ivt->{"entryAddr"}, $ivt->{"dcdAddr"}, $ivt->{"bootAddr"}, $ivt->{"selfAddr"}, $ivt->{"csfAddr"}) = unpack("x4 V x4 V V V V x4", $ivtData);

      # Give some info about read IVT
      printf("\nFound valid image vector table (IVT) at offset 0x%08X...\n", $ivtOffs);
      printf("  Header:    0x%08X (Tag=0x%02X Len=0x%04X (%d Bytes), Ver=0x%02X\n", $ivt->{"header"}, $ivt->{"tag"}, $ivt->{"size"}, $ivt->{"size"}, $ivt->{"ver"});
      printf("  Entry:     0x%08X\n", $ivt->{"entryAddr"});
      printf("  DCD:       0x%08X\n", $ivt->{"dcdAddr"});
      printf("  Boot Data: 0x%08X\n", $ivt->{"bootAddr"});
      printf("  Self:      0x%08X\n", $ivt->{"selfAddr"});
      printf("  CSF:       0x%08X\n", $ivt->{"csfAddr"});
    }
    
    # Calculate offset within image, where to read the boot data
    $bootOffs = $ivtOffs - $ivt->{"selfAddr"} + $ivt->{"bootAddr"};
    
    # Check for and extract boot data
    if($ivt->{"bootAddr"} == 0)
    {
      # No boot data in the given binary
      print("No boot data address given in IVT.");
      exit(-1);
    }
    # Boot data is part of the binary
    elsif($bootOffs < $imgPos)
    {
      # Pointer to boot data points somewhere below the end of the IVT
      print("BOOT!!!!!!!");
      exit(-2);
    }
    else
    {
      # Read boot alignment (data between IVT and boot data)
      $imgPos += read($imgFile, $buf, $bootOffs - $imgPos);
      # Save for later reference
      $bootData .= $buf;
      # Read boot data
      $imgPos += read($imgFile, $buf, $HAB_BOOT_SIZE);
      # Save for later reference
      $bootData = $buf;


      # Extract boot fields as "U32le:U32le:U32le"
      ($boot->{"startAddr"}, $boot->{"imgSize"}, $boot->{"plugin"}) = unpack("V V V", $buf);

      # Give some info about read IVT
      printf("\nFound boot data at offset 0x%08X...\n", $bootOffs);
      printf("  Start:     0x%08X\n", $boot->{"startAddr"});
      printf("  Length:    0x%08X\n", $boot->{"imgSize"});
      printf("  Plugin:    0x%08X\n\n", $boot->{"plugin"});
    }    


    # Calculate offset within image, where to read the DCD data
    $dcdOffs =  $ivtOffs - $ivt->{"selfAddr"} + $ivt->{"dcdAddr"};
 
    # Check for and extract device configuration data
    if($ivt->{"dcdAddr"} == 0)
    {
      # Device config data is not part of the binary
      print("No DCD address given in IVT.");
    }
    # Device config data is part of the binary
    elsif($dcdOffs < $imgPos)
    {
      # Pointer to device config data points somewhere below the end of the boot data
      print("DCD!!!!!!!");
      exit(-2);
    }
    else
    {
      # Read DCD alignment
      $imgPos += read($imgFile, $buf, $dcdOffs - $imgPos);
      $dcdData = $buf;

      # Read DCD header
      $imgPos += read($imgFile, $buf, $HAB_HDR_SIZE);  
      $dcdData .= $buf;
        
      # Extract DCD header
      $dcd->{"header"} = unpack("V", $buf);

      # Extract tag, length and par
      ($dcd->{"tag"}, $dcd->{"size"}, $dcd->{"par"}) = unpack("C n C", $buf);

      printf("\nFound device configuration data (DCD) at offset 0x%08X...\n", $dcdOffs);
      printf("  Header:    0x%08X (Tag=0x%02X Len=0x%04X (%d Bytes), Par=0x%02X\n", $dcd->{"header"}, $dcd->{"tag"}, $dcd->{"size"}, $dcd->{"size"}, $dcd->{"par"});
      printf("\n");
      
      $dcdSize = $dcd->{"size"};
    }
    

    # Read remaining file
    $imgPos += read($imgFile, $buf, $imgSize - $imgPos);
    # Save for later reference
    $appData = $buf;

    # Close source image file
    close($imgFile);
  }

#  $ivt->{"csfAddr"} = $ivt->{"selfAddr"} + $expSize - $ivtOffs;
#  $boot->{"imgSize"} = $expSize + $csfSize; # TODO: is this correct?


  if($imgType =~ m/MFG/)
  {
    # As we have a target DCD address, the image is a MFG image
    # Rewrite IVT with stripped DCD address
    $ivt->{"sigDcdAddr"} = 0;
  }
  else
  {
    $ivt->{"sigDcdAddr"} = $ivt->{"dcdAddr"};
  }

  # Print IVT info
  printf("\n\nWrite image to be signed...\n");
  printf("  DstPath:   %s\n", $dstImgPath.$dstImgName);
  printf("\nImage vector table (IVT):\n");
  printf("  Header:    0x%08X (Tag=0x%02X Len=0x%04X (%d Bytes), Ver=0x%02X\n", $ivt->{"header"}, $ivt->{"tag"}, $ivt->{"size"}, $ivt->{"size"}, $ivt->{"ver"});
  printf("  Entry:     0x%08X\n", $ivt->{"entryAddr"});
  printf("  DCD:       0x%08X\n", $ivt->{"sigDcdAddr"});
  printf("  Boot Data: 0x%08X\n", $ivt->{"bootAddr"});
  printf("  Self:      0x%08X\n", $ivt->{"selfAddr"});
  printf("  CSF:       0x%08X\n", $ivt->{"csfAddr"});

  # Generate IVT
  $ivtData = pack("VVVVVVVV", $ivt->{"header"}, $ivt->{"entryAddr"}, 0, $ivt->{"sigDcdAddr"}, $ivt->{"bootAddr"}, $ivt->{"selfAddr"}, $ivt->{"csfAddr"}, 0);

  # Print boot data info
  printf("\nBoot data at offset 0x%08X:\n", $bootOffs);
  printf("  Start:     0x%08X\n", $boot->{"startAddr"});
  printf("  Length:    0x%08X\n", $boot->{"imgSize"});
  printf("  Plugin:    0x%08X\n", $boot->{"plugin"});

  # Generate boot data
#  $bootData = pack("VVVN", $boot->{"startAddr"}, $boot->{"imgSize"}, $boot->{"plugin"}, 0xAFFEDEAD);
  $bootData = pack("VVV", $boot->{"startAddr"}, $boot->{"imgSize"}, $boot->{"plugin"});

  # Create image to be fed into the CST tool
  open($ivtFile, ">", "$dstImgPath"."$dstImgName") or die "Failed to open: $!";
  binmode($ivtFile);
  print($ivtFile $partData);
  print($ivtFile $ivtData);
  print($ivtFile $bootData);
  print($ivtFile $appData);
  print($ivtFile $appPad);
  close($ivtFile);


  # Prepare block definition for signing configuration
  if( ($dcdAddr != 0) && ($dcdSize != 0) )
  {
    # The source binary contains DCD and the target type is MFG
    $blocks[0]->{"blkAddr"} = sprintf("0x%08X", $ivt->{"selfAddr"});
    $blocks[0]->{"blkOffs"} = sprintf("0x%08X", $ivtOffs);
    $blocks[0]->{"blkSize"} = sprintf("0x%08X", $ivt->{"dcdAddr"} - $ivt->{"selfAddr"});
    $blocks[0]->{"file"} = "\"$dstImgName\",\\";

    $blocks[1]->{"blkAddr"} = sprintf("0x%08X", $dcdAddr);
    $blocks[1]->{"blkOffs"} = sprintf("0x%08X", $dcdOffs);
    $blocks[1]->{"blkSize"} = sprintf("0x%08X", $dcdSize);
    $blocks[1]->{"file"} = "\"$dstImgName\",\\";

    $blocks[2]->{"blkAddr"} = sprintf("0x%08X", $ivt->{"dcdAddr"} + $dcdSize);
    $blocks[2]->{"blkOffs"} = sprintf("0x%08X", $dcdOffs + $dcdSize);
    $blocks[2]->{"blkSize"} = sprintf("0x%08X", $ivt->{"csfAddr"} - ($ivt->{"dcdAddr"} + $dcdSize));
    $blocks[2]->{"file"} = "\"$dstImgName\"";
  }
  # Either the target type is MMC or the source binary image doesn't contain DCD
  elsif(($crcSize != 0) && ($crcOffs > $ivtOffs))
  {
    # Create first chunk from IVT to CRC
    $blocks[0]->{"blkAddr"} = sprintf("0x%08X", $ivt->{"selfAddr"});
    $blocks[0]->{"blkOffs"} = sprintf("0x%08X", $ivtOffs);
    $blocks[0]->{"blkSize"} = sprintf("0x%08X", $crcOffs - $ivtOffs);
    $blocks[0]->{"file"} = "\"$dstImgName\",\\";
    
    # Create first chunk from CRC to CSF
    $blocks[1]->{"blkAddr"} = sprintf("0x%08X", $ivt->{"selfAddr"} + ($crcOffs + $crcSize - $ivtOffs));
    $blocks[1]->{"blkOffs"} = sprintf("0x%08X", $crcOffs + $crcSize);
    $blocks[1]->{"blkSize"} = sprintf("0x%08X", $ivt->{"csfAddr"} - $ivt->{"selfAddr"} - ($crcOffs + $crcSize - $ivtOffs));
    $blocks[1]->{"file"} = "\"$dstImgName\"";
  }
  else
  {
    # so we can sign the whole image in a single chunk
    if($ivt->{"selfAddr"} < $ivt->{"entryAddr"})
    {
      $blocks[0]->{"blkAddr"} = sprintf("0x%08X", $ivt->{"selfAddr"});
      $blocks[0]->{"blkOffs"} = sprintf("0x%08X", $ivtOffs);
      $blocks[0]->{"blkSize"} = sprintf("0x%08X", $ivt->{"csfAddr"} - $ivt->{"selfAddr"});
      $blocks[0]->{"file"} = "\"$dstImgName\"";
    }
    else
    {
      $blocks[0]->{"blkAddr"} = sprintf("0x%08X", $ivt->{"entryAddr"});
      $blocks[0]->{"blkOffs"} = sprintf("0x%08X", $ivtOffs - ($ivt->{"selfAddr"} - $ivt->{"entryAddr"}));
      $blocks[0]->{"blkSize"} = sprintf("0x%08X", $ivt->{"csfAddr"} - $ivt->{"entryAddr"});
      $blocks[0]->{"file"} = "\"$dstImgName\"";
    }
  }
#  printf("ExpSize: 0x%08X\n", $expSize);
#  printf("IvtOffs: 0x%08X\n", $ivtOffs);
#        UID = 0x87, 0x70, 0xFA, 0x5A, 0xB9, 0x6F, 0xE9, 0xBA
#        UID = 0xBA, 0xE9, 0x6F, 0xB9, 0x5A, 0xFA, 0x70, 0x87


  if(1 == $srkNonCA)
  {
    ($cstConfig = qq
    {      [Header]
        Version = 4.1
        Hash Algorithm = SHA256
      [Install SRK]
        File = "../crts/$srkTblFileName"
        Source Index = $srkIdx
      [Install NOCAK]
        File = "../crts/$nocaKeyFileName"
      [Authenticate CSF]
        Signature Format = CMS
      [Unlock]
        Engine = OCOTP
        Features = SCS
        UID = 0x0A, 0x05, 0x7F, 0x67, 0xD2, 0x21, 0x1D, 0x2C
      [Unlock]
        Engine = SNVS
        Features = LP SWR, ZMK WRITE
      [Authenticate Data]
        Verification Index = 0
        Blocks = \\
    }) =~ s/^ {6}//mg;
  }
  else
  {
    ($cstConfig = qq
    {      [Header]
        Version = 4.1
      [Install SRK]
        File = "../crts/$srkTblFileName"
        Source Index = $srkIdx
      [Install CSFK]
        File = "../crts/$csfKeyFileName"
      [Authenticate CSF]
        Signature Format = CMS
      [Install Key]
        File = "../crts/$imgKeyFileName"
        Verification Index = $srkIdx
        Target Index = $imgkSlot
      [Authenticate Data]
        Verification Index = $imgkSlot
        Blocks = \\
    }) =~ s/^ {6}//mg;
  }

  # Create configuration for CST tool
  print("\nWrite signing config for storage media...\n");
  printf("  DstPath: %s\n", $dstImgPath.$cstConf);
  open($cstFile, ">", "$dstImgPath"."$cstConf") or die "Failed to open: $!";
  
  # Write the general config
  print($cstFile $cstConfig);
  #print($cstConfig);
  print("  BlkAddr    BlkOffs    BlkSize    File\n");
  for($i = 0; $i < @blocks; $i++)
  {
    print($cstFile "  ".$blocks[$i]->{"blkAddr"}." ".$blocks[$i]->{"blkOffs"}." ".$blocks[$i]->{"blkSize"}." ".$blocks[$i]->{"file"}."\n");
    print("  ".$blocks[$i]->{"blkAddr"}." ".$blocks[$i]->{"blkOffs"}." ".$blocks[$i]->{"blkSize"}." ".$blocks[$i]->{"file"}."\n");
  }
  close($cstFile);

#  my @cwd = getCwd();
#  print("Cwd:\n", join("\n", @cwd), "\n");
  print("\n");
  signImage();


  # Check if source image exists
  if(-e "$dstImgPath"."$csfImgName")
  {
    print("\nCheck command sequence file...\n");
    # Get and print the CSF size
    $csfSize = (-s "$dstImgPath"."$csfImgName");
    printf("  CsfName: %s\n", $dstImgPath.$csfImgName);
    printf("  CsfSize: 0x%08X (%d Bytes)\n", $csfSize, $csfSize);
  }
  else
  {
    print("Source image ".$srcImgPath.$srcImgName." doesn't exist.\n");
    exit(-1);
  }

  $expSize = $boot->{"imgSize"} - ($ivt->{"csfAddr"} - $boot->{"startAddr"});
  $padSize = $expSize - $csfSize;
  if($padSize < 0)
  {
    print("\nInvalid image size found in IVT\n");
    exit(-1);
  }
  elsif($padSize != 0)
  {
    print("\nExpanding csf...\n");
    printf("  ExpSize = 0x%08X (%d Bytes)\n", $expSize, $expSize);
    printf("  PadSize = 0x%08X (%d Bytes)\n", $padSize, $padSize);
    # Fill the padding buffer
    for($i = 0; $i < $padSize; $i++)
    {
      $csfPad .= pack("C", 0x5A);
    }
  }
  else
  {
    print("\nNo expansion necessary...\n");
  }


  # Open the CSF binary image for read
  open($csfFile, "<", "$dstImgPath"."$csfImgName") or die "Failed to open: $dstImgPath"."$csfImgName $!";
  binmode($csfFile);

  # Read CSF file
  read($csfFile, $buf, $imgSize);
  # Save for later reference
  $csfData = $buf;

  
  # Print IVT info
  printf("\nWrite expanded signed image as follows:\n");
  printf("  ImgPath:   %s\n", $srcImgPath.$expImgName);
  printf("\nImage vector table (IVT):\n");
  printf("  Header:    0x%08X (Tag=0x%02X Len=0x%04X (%d Bytes), Ver=0x%02X\n", $ivt->{"header"}, $ivt->{"tag"}, $ivt->{"size"}, $ivt->{"size"}, $ivt->{"ver"});
  printf("  Entry:     0x%08X\n", $ivt->{"entryAddr"});
  printf("  DCD:       0x%08X\n", $ivt->{"dcdAddr"});
  printf("  Boot Data: 0x%08X\n", $ivt->{"bootAddr"});
  printf("  Self:      0x%08X\n", $ivt->{"selfAddr"});
  printf("  CSF:       0x%08X\n", $ivt->{"csfAddr"});

  # Generate IVT
  $ivtData = pack("VVVVVVVV", $ivt->{"header"}, $ivt->{"entryAddr"}, 0, $ivt->{"dcdAddr"}, $ivt->{"bootAddr"}, $ivt->{"selfAddr"}, $ivt->{"csfAddr"}, 0);

  # Print boot data info
  printf("\nBoot data at offset 0x%08X:\n", $bootOffs);
  printf("  Start:     0x%08X\n", $boot->{"startAddr"});
  printf("  Length:    0x%08X\n", $boot->{"imgSize"});
  printf("  Plugin:    0x%08X\n\n", $boot->{"plugin"});

  # Generate boot data
#  $bootData = pack("VVVN", $boot->{"startAddr"}, $boot->{"imgSize"}, $boot->{"plugin"}, 0xAFFEDEAD);
  $bootData = pack("VVV", $boot->{"startAddr"}, $boot->{"imgSize"}, $boot->{"plugin"});

  # Create expanded signed image
  open($expFile, ">", "$srcImgPath"."$expImgName") or die "Failed to open: $!";
  binmode($expFile);
  print($expFile $partData);
  print($expFile $ivtData);
  print($expFile $bootData);
  print($expFile $appData);
  print($expFile $appPad);
  print($expFile $csfData);
  print($expFile $csfPad);
  close($expFile);

  makeHex($srcImgPath, $expImgName, $expHexName, $ivt->{"entryAddr"}, $boot->{"startAddr"});
  
  return 0;
}

# This function is called, when the perl script is executed.
genSignCfg(@cmdArgs);

