#include "devlib/devlib.h"
#include "mbus_master/mbus_master.h"

// winsock.h dosyasının dahil edilmesini engelle
#define _WINSOCKAPI_ 
#include "windows.h"

/* Komutu enum ile siniflandir */
typedef enum {
	TEST_CMD_UNKNOWN = 0,
	READ_1,
	READ_2,
	GET_PRI_ADDR_PA,
	GET_SEC_ADDR_PA,
	GET_BAUDRATE_PA,
	GET_CAL_CURRENT,
	GET_CUSTOMER,
	GET_RECOVERY_PERIODE,
	GET_OVER_CURRENT_LIMIT,
	GET_BREAKING_CURRENT_LIMIT,
	GET_RECOVERY_TRY_COUNT,
	GET_BREAKING_CURRENT_COMPARE_MATCH_COUNT,
	GET_MBUS_RECEIVING_CURRENT_THRESHOLD,
	GET_FAULT_RECORD,
	SET_PRI_ADDR_PA,
	SET_SEC_ADDR_PA,
	SET_BAUDRATE_PA,
	SET_FULL_IDENT_PA,
	SET_BAUDRATE,
	SET_PRI_ADDR,
	SET_SEC_ADDR,
	SET_RESTART_DEV,
	SET_MBUS_PWR,
	MBUS_PWR_ON,
	MBUS_PWR_OFF,
	SET_CHNGE_PASS,
	SET_SERVICE_MODE,
	SERVICE_MODE_ON_ENABLED,
	SERVICE_MODE_OFF_DISABLED,
	SET_CALIBRATION_CURRENT,
	SET_RESET_CALIBRATION,
	LOCK_DEVICE,
	UNLOCK_DEVICE,
	SET_CUSTOMER,
	SET_FCTRY_RST,
	SET_FCTRY_RST_EXC_CAL,
	SET_RECOVERY_PERIODE,
	SET_RECOVERY_TRY_COUNT,
	SET_OVERCURR_LIM,
	SET_BREAKCURR_LIM,
	SET_BREAKCURR_COMPARE_MATCH_CNT,
	SET_MBUS_RECEIVING_CURRENT_TRESHOLD,
	CLEAN_ALL_FAULT_RECORDS
} test_cmd_t;

uint32_t sayac_1 = 30205471;
uint32_t sayac_2 = 30205998;
//uint32_t convertor_id = 96780001;
uint32_t convertor_id = 96780008;
uint8_t primary_id = 1;
uint8_t generation = 0x01;
uint8_t medium = 0x0E;


/* Private variables ----------------------------------------------------------------------------------*/
devlib_sys_thread_t test_thread_handle;

mbus_lib_t mbus_master;
//#if (MBUS_MASTER_CFG_NT750_ENABLED)
//mbus_t nt750;
//#endif

/* Private function prototypes ------------------------------------------------------------------------*/
void test_thread(void* param);


void main(void) {

	devlib_init();
	mbus_master_init(&mbus_master);

	devlib_sys_thread_create(&test_thread_handle, "test_thread_handle", test_thread, NULL, 0, 0);

	while (1) {
		Sleep(1000);
	}
}


/**
 * \brief           AT process thread
 * \param[in]       arg: Thread argument
 */
void test_thread(void* arg) {
	DEVLIB_UNUSED(arg);
	mbus_master_res_t res = 0;
	printf("Test thread basladi. Komut girin (read1, read2 )\r\n");

	char line[64];
	while (1) {
		printf("> ");
		if (fgets(line, sizeof(line), stdin) == NULL) {
			Sleep(50);
			continue;
		}
		/* Satir sonu karakterlerini temizle */
		size_t len = strlen(line);
		while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
			line[--len] = '\0';
		}
		if (len == 0) {
			continue; /* Bos satir */
		}

		test_cmd_t cmd = TEST_CMD_UNKNOWN;
		if (strcmp(line, "read1") == 0) cmd = READ_1;
		else if (strcmp(line, "read2") == 0) cmd = READ_2;
		else if (strcmp(line, "nt750_get_primary_address_pa") == 0) cmd = GET_PRI_ADDR_PA;
		else if (strcmp(line, "nt750_get_secondary_address_pa") == 0) cmd = GET_SEC_ADDR_PA;
		else if (strcmp(line, "nt750_get_calibration_current") == 0) cmd = GET_CAL_CURRENT;
		else if (strcmp(line, "nt750_get_customer") == 0) cmd = GET_CUSTOMER;
		else if (strcmp(line, "nt750_get_recovery_periode") == 0) cmd = GET_RECOVERY_PERIODE;
		else if (strcmp(line, "nt750_get_over_current_limit") == 0) cmd = GET_OVER_CURRENT_LIMIT;
		else if (strcmp(line, "nt750_get_breaking_current_limit") == 0) cmd = GET_BREAKING_CURRENT_LIMIT;
		else if (strcmp(line, "nt750_get_recovery_try_count") == 0) cmd = GET_RECOVERY_TRY_COUNT;
		else if (strcmp(line, "nt750_get_breaking_current_compare_match_count") == 0) cmd = GET_BREAKING_CURRENT_COMPARE_MATCH_COUNT;
		else if (strcmp(line, "nt750_get_mbus_receiving_current_treshold") == 0) cmd = GET_MBUS_RECEIVING_CURRENT_THRESHOLD;
		else if (strcmp(line, "nt750_get_fault_record") == 0) cmd = GET_FAULT_RECORD;
		else if (strcmp(line, "service_mode_on") == 0) cmd = SERVICE_MODE_ON_ENABLED;
		else if (strcmp(line, "service_mode_off") == 0) cmd = SERVICE_MODE_OFF_DISABLED;
		else if (strcmp(line, "set_primary_address_pa") == 0) cmd = SET_PRI_ADDR_PA;
		else if (strcmp(line, "set_secondary_address_pa") == 0) cmd = SET_SEC_ADDR_PA;
		else if (strcmp(line, "set_full_identification_pa") == 0) cmd = SET_FULL_IDENT_PA;
		else if (strcmp(line, "set_primary_address") == 0) cmd = SET_PRI_ADDR;
		else if (strcmp(line, "set_secondary_address") == 0) cmd = SET_SEC_ADDR;
		else if (strcmp(line, "set_baudrate_pa") == 0) cmd = SET_BAUDRATE_PA;
		else if (strcmp(line, "set_baudrate") == 0) cmd = SET_BAUDRATE;
		else if (strcmp(line, "set_restart_device") == 0) cmd = SET_RESTART_DEV;
		else if (strcmp(line, "set_mbus_power") == 0) cmd = SET_MBUS_PWR;
		else if (strcmp(line, "mbus_power_on") == 0) cmd = MBUS_PWR_ON;
		else if (strcmp(line, "mbus_power_off") == 0) cmd = MBUS_PWR_OFF;
		else if (strcmp(line, "set_change_password") == 0) cmd = SET_CHNGE_PASS;
		else if (strcmp(line, "set_calibration_current") == 0) cmd = SET_CALIBRATION_CURRENT;
		//else if (strcmp(line, "lock_device") == 0) cmd = LOCK_DEVICE;
		//else if (strcmp(line, "unlock_device") == 0) cmd = UNLOCK_DEVICE;
		//else if (strcmp(line, "set_customer") == 0) cmd = SET_CUSTOMER;
		else if (strcmp(line, "set_factory_reset") == 0) cmd = SET_FCTRY_RST;
		else if (strcmp(line, "set_factory_reset_except_calibration") == 0) cmd = SET_FCTRY_RST_EXC_CAL;
		//else if (strcmp(line, "set_recovery_periode") == 0) cmd = SET_RECOVERY_PERIODE;
		//else if (strcmp(line, "set_recovery_try_count") == 0) cmd = SET_RECOVERY_TRY_COUNT;
		//else if (strcmp(line, "set_overcurrent_limit") == 0) cmd = SET_OVERCURR_LIM;
		//else if (strcmp(line, "set_breakingcurrent_limit") == 0) cmd = SET_BREAKCURR_LIM;
		else if (strcmp(line, "set_reset_calibration") == 0) cmd = SET_RESET_CALIBRATION;
		//else if (strcmp(line, "set_breakingcurrent_compare_match_count") == 0) cmd = SET_BREAKCURR_COMPARE_MATCH_CNT;
		else {
			printf("Bilinmeyen komut: %s\r\n", line);
			continue;
		}

		switch (cmd) {
		case READ_1: {
			//mbus_master.master.mbus_id = sayac_1;
			mbus_master.master.mbus_id = mbus_master_dec_to_bcd(sayac_1);
			mbus_master.master.read_type = 0x00; /* SND_NKE Network, Slave selection by 0x53 command, ACK(wait for ACK) */
			res = mbus_master_read(&mbus_master);
			break;
		}
		case READ_2: {
			//mbus_master.master.mbus_id = sayac_2;
			mbus_master.master.mbus_id = mbus_master_dec_to_bcd(convertor_id);
			mbus_master.master.read_type = 0x00; /* SND_NKE Network, Slave selection by 0x53 command, ACK(wait for ACK) */
			res = mbus_master_read(&mbus_master);
			break;
		}
#if (MBUS_MASTER_CFG_NT750_ENABLED)
		case GET_PRI_ADDR_PA: {
			mbus_master.master.primary_addr = primary_id;
			res = nt750_get_primary_address_pa(&mbus_master, &mbus_master.nt750);
			break;
		}
		case GET_SEC_ADDR_PA: {
			mbus_master.master.primary_addr = primary_id;
			res = nt750_get_secondary_address_pa(&mbus_master, &mbus_master.nt750, &primary_id);
			break;
		}
		case GET_CAL_CURRENT: {
			res = nt750_get_calibration_current(&mbus_master, &mbus_master.nt750, convertor_id, DEFAULT_PASSWORD, MANUFACTURER);
			break;
		}
		case GET_CUSTOMER: {
			res = nt750_get_customer(&mbus_master, &mbus_master.nt750, convertor_id, DEFAULT_PASSWORD, MANUFACTURER);
			break;
		}
		case GET_RECOVERY_PERIODE: {
			res = nt750_get_recovery_periode(&mbus_master, &mbus_master.nt750, convertor_id, DEFAULT_PASSWORD, MANUFACTURER);
			break;
		}
		case GET_OVER_CURRENT_LIMIT: {
			res = nt750_get_over_current_limit(&mbus_master, &mbus_master.nt750, convertor_id, DEFAULT_PASSWORD, MANUFACTURER);
			break;
		}
		case GET_BREAKING_CURRENT_LIMIT: {
			res = nt750_get_breaking_current_limit(&mbus_master, &mbus_master.nt750, convertor_id, DEFAULT_PASSWORD, MANUFACTURER);
			break;
		}
		case GET_RECOVERY_TRY_COUNT: {
			res = nt750_get_recovery_try_count(&mbus_master, &mbus_master.nt750, convertor_id, DEFAULT_PASSWORD, MANUFACTURER);
			break;
		}
		case GET_BREAKING_CURRENT_COMPARE_MATCH_COUNT: {
			res = nt750_get_breaking_current_compare_match_count(&mbus_master, &mbus_master.nt750, convertor_id, DEFAULT_PASSWORD, MANUFACTURER);
			break;
		}
		case GET_MBUS_RECEIVING_CURRENT_THRESHOLD: {
			res = nt750_get_mbus_receiving_current_treshold(&mbus_master, &mbus_master.nt750, convertor_id, DEFAULT_PASSWORD, MANUFACTURER);
			break;
		}
		case GET_FAULT_RECORD: {
			uint8_t fault_group = 0;
			res = nt750_get_fault_record(&mbus_master, &mbus_master.nt750, convertor_id, DEFAULT_PASSWORD, MANUFACTURER, &fault_group);
			break;
		}
		case SERVICE_MODE_ON_ENABLED: {
			mbus_service_mode_t service_mode_status = SERVICE_MODE_ON;
			//mbus_master.master.mbus_id = mbus_master_dec_to_bcd(convertor_id);
			res = nt750_set_service_mode_status(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, DEFAULT_PASSWORD, &service_mode_status);
			break;
		}
		case SERVICE_MODE_OFF_DISABLED: {
			mbus_service_mode_t service_mode_status = SERVICE_MODE_OFF;
			res = nt750_set_service_mode_status(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, DEFAULT_PASSWORD, &service_mode_status);
			break;
		}
		case SET_PRI_ADDR_PA: {
			uint8_t new_primary_id = 5;
			res = nt750_set_primary_address_pa(&mbus_master, &mbus_master.nt750, &primary_id, &new_primary_id);
			break;
		}
		case SET_SEC_ADDR_PA: {
			uint32_t conv_id = 96780002;
			res = nt750_set_secondary_address_pa(&mbus_master, &mbus_master.nt750, &primary_id, &conv_id);
			break;
		}
		case SET_FULL_IDENT_PA: {
			res = nt750_set_full_identification_pa(&mbus_master, &mbus_master.nt750, &primary_id, DEFAULT_PASSWORD, convertor_id, MANUFACTURER, &generation, &medium);
			break;
		}
		case SET_PRI_ADDR: {
			uint8_t new_primary_id = 1;
			res = nt750_set_primary_address(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, &new_primary_id);
			break;
		}
		case SET_SEC_ADDR: {
			uint32_t new_conv_id = 96780003;
			res = nt750_set_secondary_address(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, &new_conv_id);
			break;
		}
		case SET_RESTART_DEV: {
			res = nt750_restart(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, DEFAULT_PASSWORD);
			break;
		}
		case MBUS_PWR_ON: {
			res = nt750_set_mbus_power_status(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, DEFAULT_PASSWORD, MBUS_POWER_ON);
			break;
		}
		case MBUS_PWR_OFF: {
			res = nt750_set_mbus_power_status(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, DEFAULT_PASSWORD, MBUS_POWER_OFF);
			break;
		}
		case SET_CHNGE_PASS: {
			uint32_t new_pass = 87654321;
			res = nt750_set_change_password(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, DEFAULT_PASSWORD, &new_pass);
			break;
		}
		case SET_CALIBRATION_CURRENT: {
			uint16_t calibration_current_mA = 507;
			res = nt750_set_calibration_current(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, DEFAULT_PASSWORD, &calibration_current_mA);
			break;
		}
		case SET_FCTRY_RST: {
			res = nt750_set_factory_reset(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, DEFAULT_PASSWORD);
			break;
		}
		case SET_RESET_CALIBRATION: {
			res = nt750_set_reset_calibration(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, DEFAULT_PASSWORD);
			break;
		}
		case SET_FCTRY_RST_EXC_CAL: {
			res = nt750_set_factory_reset_except_calibration(&mbus_master, &mbus_master.nt750, convertor_id, MANUFACTURER, DEFAULT_PASSWORD);
			break;
		}
#endif
		default: {
#if DEBUG_LOG_ENABLE
			DBG_LOG(LOG_INFO, "Bilinmeyen komut: %s\r\n", line);
#endif
			break;
		}
		}
		if (res != MBUS_MASTER_OK) {
#if DEBUG_LOG_ENABLE
			DBG_LOG(LOG_INFO, "Komut hatasi: %d\r\n", res);
#endif
			continue;
		}
		else {
#if DEBUG_LOG_ENABLE
			DBG_LOG(LOG_INFO, "\r\nReceived Frames: %s\r\n", mbus_master.master.receive_buffer.data);
#endif
		}
	}
}