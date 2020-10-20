//  AN4581.pdf -   quick HAB overview
//  HAB4_API.pdf - reference for ROM functions and data types
//  AN12263.pdf  - actual memory locations for ROM functions and OCRAM data
//  IMXRT106XSRM.pdf - fusemap & (unneeded) hardware details (NXP Confidential Info)
//  CST_UB.pdf   - how to run CST to generate keys & signed code
//  HAB Examples:
//   https://boundarydevices.com/high-assurance-boot-hab-dummies/
//   https://variwiki.com/index.php?title=High_Assurance_Boot

//  ROM Vector Table (RVT) is at 0x00200300
//  OCRAM data is from 0x20201000 to 0x20201B7F

#include "hab_defines.h"
#define hab_rvt (*(hab_rvt_t *)0x00200300)

const char *status_name(uint8_t status);
const char *reason_name(uint8_t reason);
const char *context_name(uint8_t context);

uint32_t big_endian32(const uint8_t *p)
{
	return (p[0] << 24) | (p[1] << 16) |  (p[2] << 8) | p[3];
}


void setup()
{
	while (!Serial) ; // wait for serial monitor
	Serial.println("High Assurance Boot API Test - Show Logged Events");
	Serial.println("-------------------------------------------------");
	Serial.println();
	Serial.printf("Unique ID: %08X %08X\n", HW_OCOTP_CFG0, HW_OCOTP_CFG1);
	Serial.printf("SRK Hash:  %08X %08X %08X %08X %08X %08X %08X %08X\n",
		HW_OCOTP_SRK0, HW_OCOTP_SRK1, HW_OCOTP_SRK2, HW_OCOTP_SRK3,
		HW_OCOTP_SRK4, HW_OCOTP_SRK5, HW_OCOTP_SRK6, HW_OCOTP_SRK7);
	const uint32_t lockbits = HW_OCOTP_LOCK;
	Serial.printf("Fuse Lock: %08X\n", lockbits);
	print_lock_3way("Boot Config", lockbits >> 2);
	print_lock_2way("Misc Config", lockbits >> 22);
	print_lock_3way("Mac Address", lockbits >> 8);
	print_lock_3way("GP1", lockbits >> 10);
	print_lock_3way("GP2", lockbits >> 12);
	print_lock_3way("GP3", lockbits >> 26);
	print_lock_2way("SW_GP1", lockbits >> 24);
	const uint32_t *ivt = (uint32_t *)0x60001000;
	Serial.println("Image Vector Table: (ref manual rev 2: 9.7.1.1, page 261)");
	Serial.printf("  Header   %08X\n  Vector   %08X\n  reserved %08X\n  DCD      %08X\n",
		ivt[0], ivt[1], ivt[2], ivt[3]);
	Serial.printf("  Bootdata %08X\n  Self     %08X\n  CSF      %08X\n  reserved %08X\n",
		ivt[4], ivt[5], ivt[6], ivt[7]);

	Serial.println("Bootdata: (ref manual rev 2: 9.7.1.2, page 262)");
	if (ivt[4] >= 0x60000000 && ivt[4] <= 0x60FFFFFF) {
		const uint32_t *bootdata = (uint32_t *)ivt[4];
		Serial.printf("  Start    %08X\n  Length   %08X\n  Plugin   %08X\n",
			bootdata[0], bootdata[1], bootdata[2]);
	} else {
		Serial.println("ERROR: Bootdata is not an address in flash memory!");
	}
	Serial.println("DCD: (ref manual rev 2: 9.7.2, page 262)");
	if (ivt[3] == 0) {
		Serial.println("  No DCD (null)");
	} else if (ivt[3] >= 0x60000000 && ivt[3] <= 0x60FFFFFF) {
		Serial.println("  TODO: parse and print DCD");
	} else {
		Serial.println("  ERROR: DCD not an address in flash memory!");
	}
	Serial.println("CSF: (cst/docs/HAB4_API.pdf: 4.3, pages 23-42)");
	if (ivt[6] == 0) {
		Serial.println("  No CSF (null)");
	} else if (ivt[6] >= 0x60000000 && ivt[6] <= 0x60FFFFFF) {
		Serial.println("  TODO: parse and print CSF");
	} else {
		Serial.println("  ERROR: CSF not an address in flash memory!");
	}

	Serial.println();
	Serial.printf("HAB Version: %08X\n", hab_rvt.get_version());
	Serial.println();

	hab_config_t config;
	hab_state_t state;
	hab_status_t status;
	status = hab_rvt.report_status(&config, &state);
	if (status == HAB_SUCCESS) {
		Serial.println("no warning or failure audit events have been logged");
	} else if (status == HAB_WARNING) {
		Serial.println("warning events have been logged");
	} else if (status == HAB_FAILURE ) {
		Serial.println("failure event");
	} else {
		Serial.println("unknown report status");
	}

	uint32_t index = 0;
	while (1) {
		uint8_t buf[256];
		size_t size = sizeof(buf);
		memset(buf, 0, size);
		status = hab_rvt.report_event(HAB_STS_ANY, index, buf, &size);
		if (status != HAB_SUCCESS) break;
		// Event: 4.4 on page 42, HAB4_API.pdf
		//Serial.printf("Event %d (%d bytes)\n", index, size);
		//for (unsigned int i=0; i < size; i++) Serial.printf(" %02X", buf[i]);
		//Serial.println();
		Serial.printf(" %s: %s in \"%s\"\n",
			status_name(buf[4]), reason_name(buf[5]), context_name(buf[6]));
		if (buf[4] == HAB_FAILURE && buf[5] == HAB_INV_ASSERTION
		   && buf[6] == HAB_CTX_ASSERT && size >= 20) {
			uint32_t type = big_endian32(buf + 8);
			uint32_t data = big_endian32(buf + 12);
			uint32_t count = big_endian32(buf + 16);
			if (type == 0) {
				Serial.printf("  %u bytes at %08X not authenticated\n",
					count, data);
			}
		}

		index++;
	}




}

void loop()
{

}



const char *status_name(uint8_t status)
{
	if (status == HAB_FAILURE) return "Failure";
	if (status == HAB_WARNING) return "Warning";
	if (status == HAB_SUCCESS) return "Success";
	return "Unknown Status";
}

const char *reason_name(uint8_t reason)
{
	if (reason == HAB_ENG_FAIL) return "Engine failure";
	if (reason == HAB_INV_ADDRESS) return "Invalid address: access denied";
	if (reason == HAB_INV_ASSERTION) return "Invalid assertion";
	if (reason == HAB_INV_CALL) return "Function called out of sequence";
	if (reason == HAB_INV_CERTIFICATE) return "Invalid certificate";
	if (reason == HAB_INV_COMMAND) return "Invalid command: command malformed";
	if (reason == HAB_INV_CSF) return "Invalid @ref csf";
	if (reason == HAB_INV_DCD) return "Invalid @ref dcd";
	if (reason == HAB_INV_INDEX) return "Invalid index: access denied";
	if (reason == HAB_INV_IVT) return "Invalid @ref ivt";
	if (reason == HAB_INV_KEY) return "Invalid key";
	if (reason == HAB_INV_RETURN) return "Failed callback function";
	if (reason == HAB_INV_SIGNATURE) return "Invalid signature";
	if (reason == HAB_INV_SIZE) return "Invalid data size";
	if (reason == HAB_MEM_FAIL) return "Memory failure";
	if (reason == HAB_OVR_COUNT) return "Expired poll count";
	if (reason == HAB_OVR_STORAGE) return "Exhausted storage region";
	if (reason == HAB_UNS_ALGORITHM) return "Unsupported algorithm";
	if (reason == HAB_UNS_COMMAND) return "Unsupported command";
	if (reason == HAB_UNS_ENGINE) return "Unsupported engine";
	if (reason == HAB_UNS_ITEM) return "Unsupported configuration item";
	if (reason == HAB_UNS_KEY) return "Unsupported key type or parameters";
	if (reason == HAB_UNS_PROTOCOL) return "Unsupported protocol";
	if (reason == HAB_UNS_STATE) return "Unsuitable state";
	return "Unknown reason";
}

const char *context_name(uint8_t context)
{
	if (context == HAB_CTX_FAB) return "hab_fab_test()";
	if (context == HAB_CTX_ENTRY) return "hab_rvt.entry()";
	if (context == HAB_CTX_TARGET) return "hab_rvt.check_target()";
	if (context == HAB_CTX_AUTHENTICATE) return "hab_rvt.authenticate_image()";
	if (context == HAB_CTX_DCD) return "hab_rvt.run_dcd()";
	if (context == HAB_CTX_CSF) return "hab_rvt.run_csf()";
	if (context == HAB_CTX_COMMAND) return "executing csf or dcd command";
	if (context == HAB_CTX_AUT_DAT) return "Authenticated data block";
	if (context == HAB_CTX_ASSERT) return "hab_rvt.assert()";
	if (context == HAB_CTX_EXIT) return "hab_rvt.exit()";
	return "unknown context";
}

void print_lock_state(const char *name, const char *state)
{
	Serial.print("  ");
	Serial.print(name);
	Serial.print(" is ");
	Serial.println(state);
}

void print_lock_2way(const char *name, uint32_t bits)
{
	if ((bits & 1) == 0) {
		print_lock_state(name, "unlocked");
	} else {
		print_lock_state(name, "locked");
	}
}

void print_lock_3way(const char *name, uint32_t bits)
{
	if ((bits & 3) == 0) {
		print_lock_state(name, "unlocked");
	} else {
		if (bits & 1) print_lock_state(name, "write locked");
		if (bits & 2) print_lock_state(name, "operation locked");
	}
}
