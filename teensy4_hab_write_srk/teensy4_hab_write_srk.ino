// WARNING: This will permanently burn the 256 bit SRK hash into
// your Teensy 4's fuse memory.  This WRITE IS PERMANENT.  There
// is no way to erase the fuses.  If you write the wrong data,
// the only way to try again is with another Teensy!

void setup() {
  while (!Serial) ;
  delay(250);
  Serial.printf("SRK Hash: %08X %08X %08X %08X %08X %08X %08X %08X\n",
    HW_OCOTP_SRK0, HW_OCOTP_SRK1, HW_OCOTP_SRK2, HW_OCOTP_SRK3,
    HW_OCOTP_SRK4, HW_OCOTP_SRK5, HW_OCOTP_SRK6, HW_OCOTP_SRK7);

// These lines are genated with "./srkfuse cst/keys/SRK_fuse.bin".
// Hopefully I got the bit/byte/word order correct??
fuse_write(&HW_OCOTP_SRK0, 0x357F286C);
fuse_write(&HW_OCOTP_SRK1, 0x9D03F0B5);
fuse_write(&HW_OCOTP_SRK2, 0x0FD17D0B);
fuse_write(&HW_OCOTP_SRK3, 0xBC95D1AE);
fuse_write(&HW_OCOTP_SRK4, 0xA4845637);
fuse_write(&HW_OCOTP_SRK5, 0x0F8C747F);
fuse_write(&HW_OCOTP_SRK6, 0xF6E81875);
fuse_write(&HW_OCOTP_SRK7, 0xF973F092);

  Serial.printf("SRK Hash: %08X %08X %08X %08X %08X %08X %08X %08X\n",
    HW_OCOTP_SRK0, HW_OCOTP_SRK1, HW_OCOTP_SRK2, HW_OCOTP_SRK3,
    HW_OCOTP_SRK4, HW_OCOTP_SRK5, HW_OCOTP_SRK6, HW_OCOTP_SRK7);
}

void loop() {
  

}

void fuse_write(volatile uint32_t *fuses, uint32_t value)
{
        uint32_t oldvalue = *fuses;

        if ((oldvalue | value) == oldvalue) {
                Serial.println("fuse_write skipped\n");
                return; // no write needed
        }
        // page 2504

        uint32_t addr = ((uint32_t)fuses - (uint32_t)&HW_OCOTP_LOCK) >> 4;
        if (addr > 0x2F) {
                Serial.printf("fuse_write illegal address: %x\n", (uint32_t)fuses);
                return;
        }
        Serial.printf("fuse_write start, addr=%x, data=%x\n", addr, value);
        //return;

        // 1: Program the following fields with timing values to match the
        //    frequency of ipg_clk:
        //      HW_OCOTP_TIMING[STROBE_PROG]
        //      HW_OCOTP_TIMING[RELAX]
        // TODO: for now asseme default settings are ok...

        // 2: Check that HW_OCOTP_CTRL[BUSY] and HW_OCOTP_CTRL[ERROR] are
        //    clear.  Overlapped accesses are not supported by the controller.
        if (HW_OCOTP_CTRL & HW_OCOTP_CTRL_ERROR) {
                Serial.println("fuse_write prior error");
                return;
                // TODO: clear error
        }
        Serial.println("wait1");
        while (HW_OCOTP_CTRL & HW_OCOTP_CTRL_BUSY) ; // wait

        // 3: Write the requested address to HW_OCOTP_CTRL[ADDR] and program
        //    the unlock code into HW_OCOTP_CTRL[WR_UNLOCK].

        HW_OCOTP_CTRL = HW_OCOTP_CTRL_WR_UNLOCK(0x3E77) | HW_OCOTP_CTRL_ADDR(addr);

        // 4: Write the data to the HW_OCOTP_DATA register. 
        HW_OCOTP_DATA = value;

        // 5. Once complete, the controller will clear BUSY.
        Serial.println("wait2");
        while (HW_OCOTP_CTRL & HW_OCOTP_CTRL_BUSY) ; // wait

        // Due to internal electrical characteristics of the OTP during writes,
        // all OTP operations following a write must be separated by 2 us
        delayMicroseconds(2);

        // need to read it back and update shadow register
        if (HW_OCOTP_CTRL & HW_OCOTP_CTRL_ERROR) {
                Serial.println("fuse_write error\n");
                return;
        }
        // page 2504 - read process
        HW_OCOTP_CTRL = HW_OCOTP_CTRL_ADDR(addr);
        HW_OCOTP_READ_CTRL = HW_OCOTP_READ_CTRL_READ_FUSE;
        while (HW_OCOTP_CTRL & HW_OCOTP_CTRL_BUSY) ; // wait
        value = HW_OCOTP_READ_FUSE_DATA;
        Serial.printf("fuse_write, read back as %x\n", value);
        *fuses = value; // update shadow register

        Serial.println("fuse_write finish");
}
