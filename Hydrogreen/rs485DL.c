/**
 * @file rs485.c
 * @brief Biblioteka do obslugi komunikacji UART <-> RS485 <-> UART z data loggerem
 * @author Alicja Miekina na podstawie Piotra Durakiewicza
 * @date 22.04.2021
 * @todo
 * @bug
 * @copyright 2021 HYDROGREEN TEAM
 */

#include "rs485DL.h"
#include "usart.h"
#include "rxinterrupt.h"
#include "rs485SW.h"
#include "rs485EF.h"

// ******************************************************************************************************************************************************** //
//Zmienne dla transmisji danych z przeplywem energii
uint8_t dataFromRx_DL[RX_FRAME_LENGHT_DL]; ///< Tablica w ktorej zawarte sa nieprzetworzone przychodzace dane
uint16_t posInRxTab_DL = 0;	///< Aktualna pozycja w tabeli wykorzystywanej do odbioru danych
volatile static uint8_t intRxCplt_DL; ///< Flaga informujaca o otrzymaniu nowego bajtu (gdy 1 - otrzymanowy nowy bajt)
static uint8_t dataToTx_DL[TX_FRAME_LENGHT_DL]; ///< Tablica w ktorej zawarta jest ramka danych do wyslania
static uint16_t posInTxTab_DL=0;											///< Aktualna pozycja w tabeli wykorzystywanej do wysylania danych
uint8_t rs485_flt_DL = RS485_NEW_DATA_TIMEOUT_DL;///< Zmienna przechowujaca aktualny kod bledu magistrali
uint32_t rejectedFramesInRow_DL = 0;
size_t frameSizeTX_DL = 0;
// ******************************************************************************************************************************************************** //

/**
 * @struct RS485_BUFFER
 * @brief Struktura zawierajaca bufory wykorzystywane do transmisji danych
 */

RS485_BUFFER_DL RS485_BUFF_DL;

RS485_DATA_DL RS485_DL_DATA_TO_TX;

// ******************************************************************************************************************************************************** //
//dla przeplywu energii
static void sendData_DL(void);
static void prepareNewDataToSend_DL(void);
// ******************************************************************************************************************************************************** //

/**
 * crc_calc(void)
 * Funkcja obliczajaca sume kontrolna dla RX
 */
uint8_t crc_calc_DL(void) {
	uint8_t crcSumOnMCU_DL = 0xFF;
	uint8_t xbit, data1 = 1;
#define polynomial 0x7;

	for (uint8_t l = 0; l < RX_FRAME_LENGHT_DL - 1; l++) {
		uint8_t data = dataFromRx_DL[l];
		xbit = data1 << 7;
		for (uint8_t k = sizeof(RX_FRAME_LENGHT_DL - 1) * 8; k > 0; --k) // obliczanie wartosci najbardziej znaczacego bitu
				{
			if (crcSumOnMCU_DL & 0x80)    //jesli najbardziej znaczacy bit = 1
					{
				crcSumOnMCU_DL = (crcSumOnMCU_DL << 1) ^ polynomial
				; //XOR i leftshift
			} else { //jesli = 0
				crcSumOnMCU_DL = (crcSumOnMCU_DL << 1); //leftshift
			}
			if (data & xbit) {
				crcSumOnMCU_DL = crcSumOnMCU_DL ^ polynomial
				;
			}
			xbit >>= 1;
		}
	}
	return crcSumOnMCU_DL;

}
/**
 * crc_calc(void)
 * Funkcja obliczajaca sume kontrolna dla TX
 */

uint8_t crc_calc_TX_DL(void) {
	uint8_t crcSumOnMCUTX_DL = 0xFF;
	uint8_t xbit, data1 = 1;
#define polynomial 0x7;

	for (uint8_t l = 0; l < TX_FRAME_LENGHT_DL - 1; l++) {
		uint8_t data = dataToTx_DL[l];
		xbit = data1 << 7;
		for (uint8_t k = sizeof(TX_FRAME_LENGHT_DL - 1) * 8; k > 0; --k) // obliczanie wartosci najbardziej znaczacego bitu
				{
			if (crcSumOnMCUTX_DL & 0x80)    //jesli najbardziej znaczacy bit = 1
					{
				crcSumOnMCUTX_DL = (crcSumOnMCUTX_DL << 1) ^ polynomial
				; //XOR i leftshift
			} else { //jesli = 0
				crcSumOnMCUTX_DL = (crcSumOnMCUTX_DL << 1); //leftshift
			}
			if (data & xbit) {
				crcSumOnMCUTX_DL = crcSumOnMCUTX_DL ^ polynomial
				;
			}
			xbit >>= 1;
		}
	}
	return crcSumOnMCUTX_DL;
}
/**
 * @fn rs485_init_DL(void)
 * @brief Inicjalizacja magistrali RS-485, umiescic wewnatrz hydrogreen_init(void) dla kierownicy
 */
void rs485_init_DL(void) {
	//HAL_UART_Receive_DMA(&UART_PORT_RS485_DL, &RS485_BUFF_DL.rx, 1);				//Rozpocznij nasluchiwanie
	prepareNewDataToSend_DL();					//Przygotuj nowy pakiet danych
	frameSizeTX_DL = sizeof(RS485_DL_DATA_TO_TX);
}

/**
 * @fn rs485_step(void)
 * @brief Funkcje obslugujace magistrale, umiescic wewnatrz hydrogreen_step(void) dla kierownicy i przeplywu energii
 */
void rs485_step_DL(void) {
	sendData_DL();
}

/**
 * @fn sendData(void)
 * @brief Funkcja ktorej zadaniem jest obsluga linii TX, powinna zostac umieszczona w wewnatrz rs485_step() dla przeplywu energii
 */
static void sendData_DL(void) {
	static uint16_t cntEndOfTxTick_DL = 0;							//Zmienna wykorzystywana do odliczenia czasu wskazujacego na koniec transmisji

	//Sprawdz czy wyslano cala ramke danych
	if (posInTxTab_DL < TX_FRAME_LENGHT_DL)
	{
		//Nie, wysylaj dalej
		RS485_BUFF_DL.tx = dataToTx_DL[posInTxTab_DL];

		//Na czas wysylania danych wylacz przerwania
		__disable_irq();
		HAL_UART_Transmit(&UART_PORT_RS485_DL, &RS485_BUFF_DL.tx, 1, HAL_MAX_DELAY);
		__enable_irq();

		posInTxTab_DL++;
	}
	else if (cntEndOfTxTick_DL < TX_FRAME_LENGHT_DL)
	{
		//Cala ramka danych zostala wyslana, zacznij odliczac "czas przerwy" pomiedzy przeslaniem kolejnej ramki
		cntEndOfTxTick_DL++;
	}
	else
	{
		//Przygotuj nowe dane do wysylki
		cntEndOfTxTick_DL = 0;
		posInTxTab_DL = 0;

		prepareNewDataToSend_DL();
	}


	//HAL_UART_Transmit(&UART_PORT_RS485_DL, &RS485_BUFF_DL.tx,
	//		TX_FRAME_LENGHT_DL, HAL_MAX_DELAY);

	//prepareNewDataToSend_DL();
}
/**
 * @fn prepareNewDataToSend(void)
 * @brief Funkcja przygotowujaca dane do wysylki, wykorzystana wewnatrz sendData(void) dla data loggera
 */
static void prepareNewDataToSend_DL(void) {
	uint8_t j = 0;
	
	RS485_DL_DATA_TO_TX.LOGIC_STRUCT.is_emergency = RS485_TX_DATA_EF.emergencyScenario;
	RS485_DL_DATA_TO_TX.LOGIC_STRUCT.is_hydrogen_leaking = RS485_TX_DATA_SW.h2SensorDigitalPin;
	RS485_DL_DATA_TO_TX.LOGIC_STRUCT.is_SC_relay_closed = RS485_RX_VERIFIED_DATA_SW.scClose;
	RS485_DL_DATA_TO_TX.LOGIC_STRUCT.vehicle_is_speed_button_pressed = RS485_RX_VERIFIED_DATA_SW.fullGas;
	RS485_DL_DATA_TO_TX.LOGIC_STRUCT.vehicle_is_half_speed_button_pressed = RS485_RX_VERIFIED_DATA_SW.halfGas;
	RS485_DL_DATA_TO_TX.LOGIC_STRUCT.hydrogen_cell_button_state = RS485_TX_DATA_EF.fuellCellModeButtons;
	RS485_DL_DATA_TO_TX.LOGIC_STRUCT.is_super_capacitor_button_pressed = RS485_RX_VERIFIED_DATA_SW.scClose;
	
	dataToTx_DL[j] = RS485_DL_DATA_TO_TX.LOGIC_STRUCT.logic_state; // 4 b
	dataToTx_DL[++j] = 0;
	dataToTx_DL[++j] = 0;
	dataToTx_DL[++j] = 0; // padding

	for (uint8_t k = 0; k < 4; k++) { // 8 b
		RS485_DL_DATA_TO_TX.FC_CURRENT.array[k] = RS485_RX_VERIFIED_DATA_EF.FC_CURRENT.array[k];
		dataToTx_DL[++j] = RS485_RX_VERIFIED_DATA_EF.FC_CURRENT.array[k];
	}

	for (uint8_t k = 0; k < 4; k++) { // 12 b
		RS485_DL_DATA_TO_TX.CURRENT_SENSOR_FC_TO_SC.array[k] = RS485_RX_VERIFIED_DATA_EF.CURRENT_SENSOR_FC_TO_SC.array[k];
		dataToTx_DL[++j] = RS485_RX_VERIFIED_DATA_EF.CURRENT_SENSOR_FC_TO_SC.array[k];
	}

	for (uint8_t k = 0; k < 4; k++) { // 16 b
		RS485_DL_DATA_TO_TX.CURRENT_SENSOR_SC_TO_MOTOR.array[k] = RS485_TX_DATA_SW.CURRENT_SENSOR_SC_TO_MOTOR.array[k];
	    dataToTx_DL[++j] = RS485_TX_DATA_SW.CURRENT_SENSOR_SC_TO_MOTOR.array[k];
	}

	for (uint8_t k = 0; k < 4; k++) { // 20 b
		RS485_DL_DATA_TO_TX.FC_V.array[k] = RS485_RX_VERIFIED_DATA_EF.FC_V.array[k];
		dataToTx_DL[++j] = RS485_RX_VERIFIED_DATA_EF.FC_V.array[k];
	}

	for (uint8_t k = 0; k < 4; k++) { // 24 b
		RS485_DL_DATA_TO_TX.SC_V.array[k] = RS485_RX_VERIFIED_DATA_EF.SC_V.array[k];
		dataToTx_DL[++j] = RS485_RX_VERIFIED_DATA_EF.SC_V.array[k];
	}

	for (uint8_t k = 0; k < 4; k++) { // 28 b
		RS485_DL_DATA_TO_TX.H2_SENSOR_V.array[k] = RS485_TX_DATA_SW.H2_SENSOR_V.array[k];
		dataToTx_DL[++j] = RS485_TX_DATA_SW.H2_SENSOR_V.array[k];
	}

	for (uint8_t k = 0; k < 4; k++) { // 32 b
		RS485_DL_DATA_TO_TX.FC_TEMP.array[k] = RS485_RX_VERIFIED_DATA_EF.FC_TEMP.array[k];
		dataToTx_DL[++j] = RS485_RX_VERIFIED_DATA_EF.FC_TEMP.array[k];
	}

	for (uint8_t k = 0; k < 2; k++) { // 36 b
		RS485_DL_DATA_TO_TX.FC_FAN_RPM.array[k] = RS485_RX_VERIFIED_DATA_EF.fcFanRPM.array[k];
		dataToTx_DL[++j] = RS485_RX_VERIFIED_DATA_EF.fcFanRPM.array[k];
	}
	dataToTx_DL[++j] = 0;
	dataToTx_DL[++j] = 0; // Padding

	RS485_DL_DATA_TO_TX.VEHICLE_AVG_SPEED.value = (float) RS485_TX_DATA_SW.averageSpeed;
	for (uint8_t k = 0; k < 4; k++) {
		dataToTx_DL[++j] = RS485_DL_DATA_TO_TX.VEHICLE_AVG_SPEED.array[k]; // 40 b
	}

	RS485_DL_DATA_TO_TX.MOTOR_PWM = RS485_TX_DATA_SW.motorPWM;
	dataToTx_DL[++j] = RS485_TX_DATA_SW.motorPWM; // 44 b
	dataToTx_DL[++j] = 0;
	dataToTx_DL[++j] = 0;
	dataToTx_DL[++j] = 0; // Padding

	RS485_DL_DATA_TO_TX.HYDROGEN_PRESSURE.value = 2137; // TODO change
	for (uint8_t k = 0; k < 4; k++) { // 48 b
		dataToTx_DL[++j] = RS485_DL_DATA_TO_TX.HYDROGEN_PRESSURE.array[k];
	}
	//dataToTx_DL[++j] = EOT_BYTE_DL; // 49 b
	//OBLICZ SUME KONTROLNA

	//Wrzuc obliczona sume kontrolna na koniec wysylanej tablicy
	//[dataToTx_DL[TX_FRAME_LENGHT_DL - 1] = crc_calc_TX_DL(); // 42 b
}




///**
// * crc_calc(void)
// * Funkcja obliczajaca sume kontrolna dla TX
// */
//uint8_t crc_calc_TX_DL(void)
//{
//  	uint8_t crcSumOnMCUTX_DL = 0xFF;
//  	uint8_t xbit, data1 =1;
//	#define polynomial 0x7;
//
//  	for(uint8_t l = 0; l < TX_FRAME_LENGHT_DL-1; l++){
//  	uint8_t data = dataToTx_DL[l];
//  	xbit = data1 << 7;
//  	for(uint8_t k = sizeof(TX_FRAME_LENGHT_DL-1)*8; k > 0;--k) // obliczanie wartosci najbardziej znaczacego bitu
//  	{
//  		if(crcSumOnMCUTX_DL & 0x80)    //jesli najbardziej znaczacy bit = 1
//  		{
//  			crcSumOnMCUTX_DL = (crcSumOnMCUTX_DL << 1)^polynomial; //XOR i leftshift
//  		}else { //jesli = 0
//  			crcSumOnMCUTX_DL = (crcSumOnMCUTX_DL << 1); //leftshift
//  		}
//  		if(data & xbit){
//  			crcSumOnMCUTX_DL = crcSumOnMCUTX_DL ^polynomial;
//  		}
//  		xbit >>= 1;
//  	}
//  	}
//  	return crcSumOnMCUTX_DL;
//
//}

