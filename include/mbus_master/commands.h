

/****************************************************************************************************************
*	Standard Value Information Field (VIF) Used
*	-------------------------------------------
*	VIF(BIN)	VIF(HEX)	Description											Unit
*	--------	--------	--------------------------------------------		-------------
*	01111001 	79			Set Secondary Address 								Dimensionless
*	01111010 	7A			Set Primary Address 								Dimensionless
*	11111101 	FD			A standard VIFE from extension table follows 		Dimensionless
*	11111111 	FF			A further manufacturer specific VIFE follows 		Dimensionless
*
*
*
*	Standard Value Information Field Extension (VIFE) Used
*	------------------------------------------------------
*	VIFE(BIN)	VIFE(HEX)	Description 										Unit
*	--------	--------	--------------------------------------------		-------------
*	00001011 	0B			Parameter Set Identification 						Dimensionless
*	00001100 	0C			Firmware Version 									Dimensionless
*	00001101 	0D			Hardware Version 									Dimensionless
*	11001100 	C6			Voltage(VIFE follows) 								mV
*	11011001 	D9			Current(VIFE follows) 								mA
*	11111111 	FF			A further manufacturer specific VIFE follows 		Dimensionless
*
*
*
*	Manufacturer Specific Value Information Field Extension (VIFE) Used
*	-------------------------------------------------------------------
*	VIFE(BIN)	VIFE(HEX)	Description 										Unit
*	--------	--------	--------------------------------------------		-------------
*	01110001 	71			Set Calibration Mode(on/off)		 				Dimensionless - OK
*	01110010 	72			Calibrate Current Sensor 							Dimensionless - OK
*	01110011 	73			Read Calibration Mode Status						Dimensionless - OK (RSP_UD)
*	01110100 	74			Read Calibration Status								Dimensionless - OK (RSP_UD)
*	01110101 	75			Read Calibration Current mA							mA			  - OK (RSP_UD)
*	01110110 	76			Set M-BUS Power(on/off)		 						Dimensionless - OK
*	01110111 	77			Read M-BUS Power(on/off)		 					Dimensionless - OK (RSP_UD)
*	01111000 	78			Lock Device				 							Dimensionless - OK
*	01111001 	79			Unlock Device				 						Dimensionless - OK
*	01111010 	7A			Read Device Lock Status				 				Dimensionless - OK (RSP_UD)
*	01111011 	7B			Set Customer Location				 				Dimensionless - OK
*	01111100 	7C			Read Customer Location				 				Dimensionless - OK
*	01111101 	7D			Enter Password						 				Dimensionless - OK
*	01111110 	7E			Change Password						 				Dimensionless - OK
*	01111111 	7F			Read Baudrate						 				Dimensionless - OK
*	10000000	80			Device Restart										Dimensionless - OK
*	10000001	81			Enter Super Password								Dimensionless - OK
*  	10000010	82			Set Super Password									Dimensionless - NOT IMPLEMENTED
* 	10000011	83			Read Super Password									Dimensionless - NOT IMPLEMENTED
*	10000100	84			Set Short Circuit Recovery Time in Seconds			Dimensionless - OK
*	10000101	85			Read Short Circuit Recovery Time in Seconds			Dimensionless - OK
*	10000110	86			Set Short Circuit Recovery Try Count				Dimensionless - OK
*	10000111	87			Read Short Circuit Recovery Try Count				Dimensionless - OK
*	10001000	88			Fabrication	Reset									Dimensionless - OK
*	10001001	89			Set Over Current Limit								mA			  - OK
*	10001010	8A			Read Over Current Limit								Dimensionless - OK
*	10001011	8B			Set Breaking Current Limit							mA			  - OK
*	10001100	8C			Read Breaking Current Limit							Dimensionless - OK
*	10001101	8D			Read Device Status									Dimensionless
*	10001110	8E			Fabrication	Reset Except Calibration				Dimensionless
*	10001111	8F			Reset Calibration									Dimensionless
*	10010000	90			Reset Storage										Dimensionless
*	10010001	91			Set Breaking Current Compare Match Count			Dimensionless
*	10010001	92			Read Breaking Current Compare Match Count			Dimensionless
*	10010011	93			Set Receiving Current Treshold						Dimensionless
*	10010100	94			Read Receiving Current Treshold						Dimensionless - OK
*	10010100	95			Read Fault Records									Dimensionless - OK
*
****************************************************************************************************************/

/****************************************************************************************************************
*	Veri Gönderme Yöntemi
*	---------------------
*	DIF		: 0D	Variable lenght
*	VIF		: FD	Linear VIF - Extension
*	VIFE	: 9X	Manufacturer Specific (komut olarak kullnilacak)
*	Len		: XX	Lenght of data
*	Data	: XX XX XX XX XX(Komut 4 byte, deger 4 byte)
*
*   DIF		: XX	DIF
*	VIF		: FF	VIF followed by manufacturer specific VIFE
*	VIFE	: 9X	Manufacturer Specific (komut olarak kullnilacak)
*	Data	: XX XX XX XX
*
****************************************************************************************************************/

/********************************************* DESTEKLENEN KOMUTLAR *********************************************
*	Default primary address: 1
*	Default secondaary address: 12345678
*
*	KOMUT YAPISI:
*	--------------
*	<serial_number:command:parameter:data>
*	<ModemSerialNo:Command:parameter(primary or secondary addreses, input or output number etc.):data>
*
*	KOMUT KODLARI:
*	--------------
*	Komut kodlari string 4 digit olarak iletilir, int32 olarak degrlendirilir.
*	Digit 3: Platform kodu (IoT:1)
*	Digit 2: Aile kodu (Remote meter reading: 1)
*	Digit 1: Komut MSB
*	Digit 0: Komut LSB
*
*
*
*
*	PA: Primary Addressing
*	SA: Secondary Addressing
*
* 	A.	GENERAL COMMANDS
*	----------------------
*	1.	SND_NKE	[Short Frame]: (10 40 FD 3D 16) or (10 40 FE 3E 16) or (10 40 FF 3F 16)
*
*	2.	REQ_UD2	[Short Frame]:	(10 7B FD 78 16)
*		Açiklama: READ THE PRIMARY ADDRESS için (10 7B FE 79 16) kullanilir.
*
*
*
*	B.	PRIMARY ADDRESSING (PA)
*	----------------------
*
*	3.	READ THE PRIMARY ADDRESS PA [Long Frame]:  (broadcast) (68 05 05 68 73 FE 51 08 7A 44 16) (E5) (10 7B FD 78 16) (answer)
*		Açiklama:	Cihazin primary adresini ögrenmek için kullanilir.
*					Bu komut broadcast adresine gönderildiginden cihaz tek basina(point to point) baglanarak gönderilmelidir.
*					Aksi takdirde agdaki diger cihazlari da etkileyeceginden veri alinamayabilir.
*		MODEM KOMUTU:
*			Komut Kodu: READ_PRIMARY_ADDRESS_PA: 1101
*			<ModemSerialNo:READ_PRIMARY_ADDRESS_PA::>
*			<ModemSerialNo:1101::>
*
*
*	4.	READ THE SECONDARY ADDRESS PA [Long Frame]:   (68 05 05 68 73 01 51 08 79 46 16) (E5) (10 7B FD 78 16) (answer)
*		Açiklama:	Cihazin secondary adresini ögrenmek için kullanilir ve cihazin primary adresine gönderilir.
*		MODEM KOMUTU:
*			Komut Kodu: READ_SECONDARY_ADDRESS_PA: 1102
*			<ModemSerialNo:READ_SECONDARY_ADDRESS_PA:primary_address:>
* 			<ModemSerialNo:1102:primary_address:>
*
*	5.	SET PRIMARY ADDRESS PA [Long Frame]:	 (68 06 06 68 73 01 51 01 7A 02 42 16) (E5)
*		MODEM KOMUTU:
*			Komut Kodu: SET_PRIMARY_ADDRESS_PA: 1103
*			<ModemSerialNo:SET_PRIMARY_ADDRESS_PA:primary_address:new_primary_address>
* 			<ModemSerialNo:1103:primary_address:new_primary_address>
*
*	6.	SET SECONDARY ADDRESS PA [Long Frame]: (adres) (68 09 09 68 73 01 51 04 79 66 77 88 99 40 16) (E5)
*		MODEM KOMUTU:
*			Komut Kodu: SET_SECONDARY_ADDRESS_PA: 1104
*			<ModemSerialNo:SET_PRIMARY_ADDRESS_PA:primary_address:new_secondary_address>
* 			<ModemSerialNo:1104:primary_address:new_secondary_address>
*
*	7.	SET COMPLATE IDENTIFICATION PA [Long Frame]: (ID, Man, Gen, Med)
*		(ID=55667788, Man=1234h (PAD), Gen=99, Med=0E (Heat)(68 0D 0D 68 73 01 51 07 79 88 77 66 55 34 12 99 0E EC 16) (E5)
*		MODEM KOMUTU:
*			Komut Kodu: SET_COMPLATE_IDENTIFICATION_PA: 1105
*			<ModemSerialNo:SET_COMPLATE_IDENTIFICATION_PA:primary_address:(8byte ID)(2byte MAN)(1byte GEN)(1byte MEDIUM)>
*			 <ModemSerialNo:1105:primary_address:(8byte ID)(2byte MAN)(1byte GEN)(1byte MEDIUM)>
*
*	8.	SET BAUDRATE PA [Control Frame]: (68 03 03 68 73 01 BB 2F 16) (E5)
*		Açiklama:
*		MODEM KOMUTU:
*			Komut Kodu: SET_BAUDRATE_PA: 1106
*			<ModemSerialNo:SET_BAUDRATE_PA:primary_address:new_baudrate>
*			<ModemSerialNo:1106:primary_address:new_baudrate>
*
*	9.	READ BAUDRATE PA [Long Frame]: (68 06 06 68 73 01 51 08 FF 91 5D 16) (E5) (10 7B FD 78 16) (answer)
*		MODEM KOMUTU:
*			Komut Kodu: READ_BAUDRATE_PA: 1107
*			<ModemSerialNo:READ_BAUDRATE_PA:primary_address:>
* 			<ModemSerialNo:1107:primary_address:>
*
*
*
*	C.	SECONDARY ADDRESSING (SA)
*	------------------------
*	10.	SELECT_SLAVE SA [Long Frame]: (68 0B 0B 68 53 FD 52 78 56 34 12 FF FF FF FF B2 16) (E5) 8
*
*	11.	SET BAUDRATE SA [Long Frame]: (68 0B 0B 68 73 FD BB 78 56 34 12 13 06 FF FF 56 16) (E5) 8
*		MODEM KOMUTU:
*			Komut Kodu: SET_BAUDRATE_SA: 1108
*			<ModemSerialNo:SET_BAUDRATE_SA:secondary_address:new_baudrate>
* 			<ModemSerialNo:1108:secondary_address:new_baudrate>
*
*	12.	SET PRIMARY ADDRESS SA [Long Frame]: (68 0E 0E 68 73 FD 51 78 56 34 12 13 06 FF FF 01 7A 02 69 16) (E5) 11
*		MODEM KOMUTU:
*			Komut Kodu: SET_PRIMARY_ADDRESS_SA: 1109
*			<ModemSerialNo:SET_BAUDRATE_SA:secondary_address:new_primary_address>
* 			<ModemSerialNo:1109:secondary_address:new_primary_address>
*
*	13.	SET SECONDARY ADDRESS SA [Long Frame]: (68 11 11 68 73 FD 51 78 56 34 12 13 06 FF FF 04 79 88 77 66 55 23 16) (E5) 14
*		MODEM KOMUTU:
*			Komut Kodu: SET_SECONDARY_ADDRESS_SA: 1110
*			<ModemSerialNo:SET_SECONDARY_ADDRESS_SA:secondary_address:new_secondary_address>
* 			<ModemSerialNo:1110:secondary_address:new_secondary_address>
*
*	14.	SET CALIBRATION MODE SA (ON/OFF) (manufacturer specific) [Long Frame]: (68 16 16 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 01 FF 81 01 92 16) (E5) 19
*		Açiklama:	1 byte(on/off) 0x00:etkisiz, 0x01 on, 0x02 off
*		MODEM KOMUTU:
*			Komut Kodu: SET_CALIBRATION_MODE_SA: 1111
*			<ModemSerialNo:SET_CALIBRATION_MODE_SA:secondary_address:mode(1:on, 2:off)>
* 			<ModemSerialNo:1111:secondary_address:mode(1:on, 2:off)>
*
*	15.	CALIBRATE CURRENT SENSOR SA (manufacturer specific) [Long Frame]: (68 17 17 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 02 FF 82 09 05 A1 16) (E5) 20
*		MODEM KOMUTU:
*			Komut Kodu: CALIBRATE_SENSOR_SA: 1112
*			<ModemSerialNo:CALIBRATE_CURRENT_SENSOR_SA:secondary_address:calibration_value>
* 			<ModemSerialNo:1112:secondary_address:calibration_value>
*
*	16.	READ CURRENT SENSOR CALIBRATION VALUE SA (manufacturer specific) [Long Frame]: (68 15 15 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 00 FF 83 92 16) (E5)  (10 7B FD 78 16) (answer) 18
*		MODEM KOMUTU:
*			Komut Kodu: READ_CALIBRATION_VALUE_SA: 1113
*			<ModemSerialNo:READ_CALIBRATION_VALUE_SA:secondary_address:>
* 			<ModemSerialNo:1113:secondary_address:>
*
*	17.	SET M-BUS POWER SA (on/off) (manufacturer specific) [Long Frame]: (68 0F 0F 68 73 FD 51 78 56 34 12 13 06 FF FF 01 FF 84 01 71 16) (E5) 12
*		Açiklama:	1 byte(on/off) 0x00:etkisiz, 0x01 on, 0x02 off
*		MODEM KOMUTU:
*			Komut Kodu: SET_MBUS_POWER_SA: 1114
*			<ModemSerialNo:SET_MBUS_POWER_SA:secondary_address:mbus_power_status(1:on, 2:off)>
* 			<ModemSerialNo:1114:secondary_address:mbus_power_status(1:on, 2:off)>
*
*	18.	LOCK DEVICE SA (manufacturer specific) [Long Frame]:  (68 15 15 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 00 FF 85 94 16) (E5) 18
*		MODEM KOMUTU:
*			Komut Kodu: LOCK_DEVICE_SA: 1115
*			<ModemSerialNo:LOCK_DEVICE_SA:secondary_address:>
* 			<ModemSerialNo:1115:secondary_address:>
*
*	19.	UNLOCK DEVICE SA (manufacturer specific) [Long Frame]: (68 15 15 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 00 FF 86 95 16) (E5) 18
*		MODEM KOMUTU:
*			Komut Kodu: UNLOCK_DEVICE_SA: 1116
*			<ModemSerialNo:UNLOCK_DEVICE_SA:secondary_address:>
* 			<ModemSerialNo:1116:secondary_address:>
*
*	20.	SET CUSTOMER LOCATION SA (manufacturer specific) [Long Frame]: (68 19 19 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 04 FF 87 88 66 44 22 EE 16) (E5) 22
*		MODEM KOMUTU:
*			Komut Kodu: SET_CUSTOMER_LOCATION_SA: 1117
*			<ModemSerialNo:SET_CUSTOMER_LOCATION_SA:secondary_address:customer_location>
* 			<ModemSerialNo:1117:secondary_address:customer_location>
*
*	21.	READ CUSTOMER LOCATION SA (manufacturer specific) [Long Frame]: (68 15 15 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 00 FF 88 97 16) (E5) (10 7B FD 78 16) (answer) 18
*		MODEM KOMUTU:
*			Komut Kodu: READ_CUSTOMER_LOCATION_SA: 1118
*			<ModemSerialNo:SET_CUSTOMER_LOCATION_SA:secondary_address:>
* 			<ModemSerialNo:1118:secondary_address:>
*
*	22.	CHANGE PASSWORD SA (manufacturer specific) [Long Frame]: (68 19 19 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 04 FF 90 77 55 33 11 B3 16) (E5) 22
*		MODEM KOMUTU:
*			Komut Kodu: CHANGE_PASSWORD_SA: 1119
*			<ModemSerialNo:CHANGE_PASSWORD_SA:secondary_address:old_password, new_password)>
*			<ModemSerialNo:1119:secondary_address:old_password, new_password)>
*
*  	23.	FABRICATION RESET SA (manufacturer specific) [Long Frame]:
*		Açiklama:	Cihazi fabrika ayarlarina döndürür.
*		MODEM KOMUTU:
*			Komut Kodu: FABRICATION_RESET_SA: 1127
*			<ModemSerialNo:FABRICATION_RESET_SA:secondary_address:>
* 			<ModemSerialNo:1127:secondary_address:data>
*
//*  	.	READ SUPER PASSWORD SA (manufacturer specific) [Long Frame]: () (E5) (10 7B FD 78 16) (answer) data size:11
//*		Açiklama:	Super password okunur.
//*		MODEM KOMUTU:
//*			Komut Kodu: READ_SUPER_PASSWORD_SA: 1122
//*			<ModemSerialNo:ENTER_SUPER_PASSWORD_SA:secondary_address:data>
//* 			<ModemSerialNo:1122:secondary_address:data>
*
*  	24.	SET SHORT CIRCUIT RECOVERY TIME IN SECONDS SA (manufacturer specific) [Long Frame]: () (E5) data size:22
*		Açiklama:	Kisa devre korumasi devreye girdikten sonra normal çalisma durumuna geçmek için beklenecek süreyi ayarlar. Saniye cinsindendir.
*		MODEM KOMUTU:
*			Komut Kodu: SET_SHORT_CIRCUIT_RECOVERY_TIME_IN_SECONDS_SA: 1123
*			<ModemSerialNo:SET_SHORT_CIRCUIT_RECOVERY_TIME_IN_SECONDS_SA:secondary_address:data>
* 			<ModemSerialNo:1123:secondary_address:data>
*
*  	25.	READ SHORT CIRCUIT RECOVERY TIME IN SECONDS SA (manufacturer specific) [Long Frame]: () (E5) (10 7B FD 78 16) (answer) data size:18
*		Açiklama:	Kisa devre korumasi devreye girdikten sonra normal çalisma durumuna geçmek için beklenecek süreyi sorgular. Saniye cinsindendir.
*		MODEM KOMUTU:
*			Komut Kodu: READ_SHORT_CIRCUIT_RECOVERY_TIME_IN_SECONDS_SA: 1124
*			<ModemSerialNo:READ_SHORT_CIRCUIT_RECOVERY_TIME_IN_SECONDS_SA:secondary_address:>
* 			<ModemSerialNo:1124:secondary_address:data>
*
*  	26.	SET SHORT CIRCUIT RECOVERY TRY COUNT SA (manufacturer specific) [Long Frame]:() (E5) data size:22
*		Açiklama:	Kisa devre korumasi devreye girdikten sonra normal çalisma durumuna geçmeyi kaç defa deneyecegini ayarlar.
*		MODEM KOMUTU:
*			Komut Kodu: SET_SHORT_CIRCUIT_RECOVERY_TRY_COUNT_SA: 1125
*			<ModemSerialNo:SET_SHORT_CIRCUIT_RECOVERY_TRY_COUNT_SA:secondary_address:data>
* 			<ModemSerialNo:1125:secondary_address:data>
*
*  	27.	READ SHORT CIRCUIT RECOVERY TRY COUNT SA (manufacturer specific) [Long Frame]:() (E5) (10 7B FD 78 16) (answer) data size:18
*		Açiklama:	Kisa devre korumasi devreye girdikten sonra normal çalisma durumuna geçmeyi kaç defa deneyecegini sorgular.
*		MODEM KOMUTU:
*			Komut Kodu: READ_SHORT_CIRCUIT_RECOVERY_TRY_COUNT_SA: 1126
*			<ModemSerialNo:SET_SHORT_CIRCUIT_RECOVERY_TRY_COUNT_SA:secondary_address:>
* 			<ModemSerialNo:1126:secondary_address:>
*
*  	28.	DEVICE RESTART SA (manufacturer specific) [Long Frame]: () (E5) data size:18
*		Açiklama:	Cihazi yeniden baslatir.
*		MODEM KOMUTU:
*			Komut Kodu: FABRICATION_RESET_SA: 1120
*			<ModemSerialNo:FABRICATION_RESET_SA:secondary_address:data>
* 			<ModemSerialNo:1120:secondary_address:data>
*
*  	29.	SET OVER CURRENT LIMIT SA (manufacturer specific) [Long Frame]:() (E5) data size:22
*		Açiklama:	Kisa devre korumasi devreye girdikten sonra normal çalisma durumuna geçmeyi kaç defa deneyecegini sorgular.
*		MODEM KOMUTU:
*			Komut Kodu: SET_OVER_CURRENT_LIMIT_SA: 1128
*			<ModemSerialNo:SET_OVER_CURRENT_LIMIT_SA:secondary_address:data>
* 			<ModemSerialNo:1128:secondary_address:data>
*
*  	30.	READ OVER CURRENT LIMIT SA (manufacturer specific) [Long Frame]:() (E5) (10 7B FD 78 16) (answer) data size:18
*		Açiklama:	Kisa devre korumasi devreye girdikten sonra normal çalisma durumuna geçmeyi kaç defa deneyecegini sorgular.
*		MODEM KOMUTU:
*			Komut Kodu: READ_OVER_CURRENT_LIMIT_SA: 1129
*			<ModemSerialNo:READ_OVER_CURRENT_LIMIT_SA:secondary_address:>
* 			<ModemSerialNo:1129:secondary_address:>
*
*  	31.	SET BREAKING CURRENT LIMIT SA (manufacturer specific) [Long Frame]:() (E5) data size:22
*		Açiklama:	Kisa devre korumasi devreye girdikten sonra normal çalisma durumuna geçmeyi kaç defa deneyecegini sorgular.
*		MODEM KOMUTU:
*			Komut Kodu: SET_BREAKING_CURRENT_LIMIT_SA: 1130
*			<ModemSerialNo:SET_BREAKING_CURRENT_LIMIT_SA:secondary_address:data>
* 			<ModemSerialNo:1130:secondary_address:data>
*
*  	32.	READ BREAKING CURRENT LIMIT SA (manufacturer specific) [Long Frame]:() (E5) (10 7B FD 78 16) (answer) data size:18
*		Açiklama:	Kisa devre korumasi devreye girdikten sonra normal çalisma durumuna geçmeyi kaç defa deneyecegini sorgular.
*		MODEM KOMUTU:
*			Komut Kodu: READ_BREAKING_CURRENT_LIMIT_SA: 1131
*			<ModemSerialNo:READ_BREAKING_CURRENT_LIMIT_SA:secondary_address:>
* 			<ModemSerialNo:1131:secondary_address:>
*
****************************************************************************************************************/

/************************************* ALTERNATIF VERI GÖNDERME YÖNTEMLERI **************************************
*
*	Veri Gönderme Yöntem 1
*	----------------------
*	DIF	: 0D	Variable lenght
*	VIF	: FD	Linear VIF-Extension
*	VIFE: 76	Manufacturer Specific
*	Len	: XX	Lenght of data
*	Data: XX XX XX XX XX (Komut 4 byte, deger 4 byte)
*
*
*	Veri Gönderme Yöntem 2 (M-BUS.pdf sayfa 16)
*	-------------------------------------------
*	DIF	: data uzunlugu
*	VIF	: FF (VIF followed by manufacturer specific VIFE)
*	VIFE: 82 Partial Counters (kirilim 1)
*	VIFE: FF VIFE followed by specific VIFE
*	VIFE: 70 (Manufacturer specific VIFE: Reset Partial Counter) (kirilim 2)
*	Data: XX
*
*
*	Veri Gönderme Yöntem 3(Primary Addressing)
*	DIF	: 00 no data
*	VIF	: FF followed by manufacturer specific VIFE
*	VIFE: 82 Partial Counters
*	VIFE: FF followed by manufacturer specific VIFE
*	VIFE: 72 Manufacturer specific VIFE: Stop Counter
*
*****************************************************************************************************************/


/***************************************** KONVERTÖRÜN GÖNDERDIGI DATALAR ****************************************
*
*
*	1.	Hardware version											//MBUS_VIFE_EXTENSION_HARDWARE_VERSION*
* 	2.	Version Of The Device										//MBUS_VIFE_EXTENSION_MODEL_VERSION
*	3.	Firmware version											//MBUS_VIFE_EXTENSION_FIRMWARE_VERSION
*	4.	Device Status Flags	(1 Byte)								//MBUS_VIFE_EXTENSION_STATE_OF_PARAMETER_ACTIVATION
*	5.	Error Flags (2 Byte)										//MBUS_VIFE_EXTENSION_ERROR_FLAGS
*	6.	M-BUS Power Durumu(1: M-BUS power on, 0: M-BUS power off)	//MBUS_VIFE_EXTENSION_DIGITAL_OUTPUT
*	7.	Mbus current												//MBUS_VIFE_EXTENSION_MILLI_AMPERE	- Storage 0
*	8.	Mbus voltage												//MBUS_VIFE_EXTENSION_MILLI_VOLT	- Storage 0
*	9.	24V supply voltage											//MBUS_VIFE_EXTENSION_MILLI_VOLT	- Storage 1
*	10.	36V supply voltage											//MBUS_VIFE_EXTENSION_MILLI_VOLT	- Storage 2
*	11.	Power supply voltage										//MBUS_VIFE_EXTENSION_MILLI_VOLT	- Storage 3
*	12. Kalibrasyon akimi (current mA)								//MBUS_VIFE_EXTENSION_MILLI_AMPERE	- Storage 1
*	13. Device Lock Durumu (1: Device locked, 0: Device unlocked)	//MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION - ST No 0	(1: Device Locked, 2: Device Unlocked)
*	14. Kalibrasyon Modu Durumu										//MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION - ST No 1	(1: Kalibrasyon modu aktif, 2: Kalibrasyon modu pasif)
*	15. Kalibrasyon Durumu											//MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION - ST No 2	(1: Kalibre edilmis, 2: Kalibre edilmemis)
*
*
*	Hardware version
*	-----------------
*	Byte 0: Hardware Minor Number
*	Byte 1: Hardware Major Number
*	Byte 2: Communication Type
*	Byte 3: Device Type
*
*
*	Version Of The Device
*	---------------------
*	Byte 0: Device Version
*
*
*	Device Status Flags Bits
*	-------------------------
*	bit 0: Kalibrasyon Modu Durumu	(1: Kalibrasyon modu aktif, 0: Kalibrasyon modu pasif)
*	bit 1: Kalibrasyon Durumu		(1: Kalibre edilmis, 0: Kalibre edilmemis)
*	bit 2:
* 	bit 3:
* 	bit 4:
* 	bit 5:
* 	bit 6:
* 	bit 7:
*
*
*	Error Flags
*	-----------
*	bit 0:
*	bit 1:
*	bit 2:
* 	bit 3:
* 	bit 4:
* 	bit 5:
* 	bit 6:
* 	bit 7:
*  	bit 8:
*  	bit 9:
*  	bit 10:
*  	bit 11:
*  	bit 12:
*  	bit 13:
*  	bit 14:
*  	bit 15:
*
*****************************************************************************************************************/



/********************************************* MBUS MASTER OPERASYONLARI *******************************************
*
*	1.READ SLAVE
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Select Slave
*		3.											E5
*		4. REQ_UD2
*		5.											RSP_UD
*
*
*	2. READ THE PRIMARY ADDRESS PA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Read Primary Address PA
*		3.											E5
*		4. REQ_UD2
*		5.											RSP_UD
*
*
* 	3. READ THE SECONDARY ADDRESS PA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Read Secondary Address PA
*		3.											E5
*
*
*	4. SET PRIMARY ADDRESS PA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Primary Address PA
*		3.											E5
*
*
*	5.SET SECONDARY ADDRESS PA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Secondary Address PA
*		3.											E5
*
*
*	6.SET COMPLATE IDENTIFICATION PA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Complate Identification PA
*		3.											E5
*
*
*	7.SET BAUDRATE PA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Baudrate PA
*		3.											E5
*
*
*	8.READ BAUDRATE PA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Read Baudrate PA
*		3.											E5
*		4. REQ_UD2
*		5.											RSP_UD
*
*
*	9.SET BAUDRATE SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Baudrate SA
*		3.											E5
*
*
*	10.SET PRIMARY ADDRESS SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Primary Address SA
*		3.											E5
*
*
*	11.SET SECONDARY ADDRESS SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Secondary Address SA
*		3.											E5
*
*
*	12.SET CALIBRATION MODE SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Calibration Mode SA
*		3.											E5
*
*
*	13.CALIBRATE CURRENT SENSOR SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Calibrate Current Sensor SA
*		3.											E5
*
*
*	14.READ CURRENT SENSOR CALIBRATION VALUE SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Read Sensor Calibration Value SA
*		3.											E5
*		4. REQ_UD2
*		5.											RSP_UD
*
*
*	15.SET M-BUS POWER SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Mbus Power SA
*		3.											E5
*
*
*	16.LOCK DEVICE SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Lock Device SA
*		3.											E5
*
*
*	17.UNLOCK DEVICE SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Unlock Device SA
*		3.											E5
*
*
*	18.SET CUSTOMER LOCATION SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Customer Location SA
*		3.											E5
*
*
*	19.READ CUSTOMER LOCATION SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Read Customer Location SA
*		3.											E5
*		4. REQ_UD2
*		5.											RSP_UD
*
*
*	20.CHANGE PASSWORD SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Change Password SA
*		3.											E5
*
*
*	21.DEVICE RESET SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Reset Device SA
*		3.											E5
*
*
*	22.SET SUPER PASSWORD SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Super Password
*		3.											E5
*
*
//*	.READ SUPER PASSWORD SA
//*	----------------------------------------------------------------
//*		Master to Slave								Slave To Master
//*		----------------							----------------
//*		1. SND_NKE
//*		2. Read Super Password
//*		3.											E5
//* 		4. REQ_UD2
//*		5.											RSP_UD
*
*
*	23.SET SHORT CIRCUIT RECOVERY TIME IN SECONDS SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Short Cicuit Recovery Periode
*		3.											E5
*
*
*	24.READ SHORT CIRCUIT RECOVERY TIME IN SECONDS SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Read Short Cicuit Recovery Periode
*		3.											E5
*		4. REQ_UD2
*		5.											RSP_UD
*
*
*	25.SET SHORT CIRCUIT RECOVERY TRY COUNT SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Short Cicuit Recovery Try Count
*		3.											E5
*
*
*	26.READ SHORT CIRCUIT RECOVERY TRY COUNT SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Read Short Cicuit Recovery Try Count
*		3.											E5
*		4. REQ_UD2
*		5.											RSP_UD
*
*
*	27.FABRICATION RESET SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Fabrication Reset
*		3.											E5
*
*
*	28.SET OVER CURRENT LIMIT SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Over Current Limit
*		3.											E5
*
*
*	29.READ OVER CURRENT LIMIT SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Read Over Current Limit
*		3.											E5
*		4. REQ_UD2
*		5.											RSP_UD
*
*
*	30.SET BREAKING CURRENT LIMIT SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Set Breaking Current Limit
*		3.											E5
*
*
*	31.READ BREAKING CURRENT LIMIT SA
*	----------------------------------------------------------------
*		Master to Slave								Slave To Master
*		----------------							----------------
*		1. SND_NKE
*		2. Read Breaking Current Limit
*		3.											E5
*		4. REQ_UD2
*		5.											RSP_UD
*
*
* *****************************************************************************************************************/

