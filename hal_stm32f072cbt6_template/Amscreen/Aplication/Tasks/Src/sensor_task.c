/**
 * @file sensor_task.c
 *
 * @brief This file contains the implementation of the sensor task, which is responsible for
 * reading temperature and humidity from the SHT4X sensor.
 *
 * @date Feb 23, 2023
 *
 * @author ARahmatinia
 */

/*****************************************************************************
   Includes
*****************************************************************************/
#include <millis.h>
#include <stdbool.h>
#include "i2c.h"
#include "sensor_task.h"
#include "sht4x_driver.h"
#include "usart.h"
#include "stm32f0xx_hal.h"


/*****************************************************************************
   defines
*****************************************************************************/
#define SENSOR_NAME "SHT4X_SENSOR"
#define SAMPLE_INTERVAL 10000
#define SENSOR_UPDATE_DELAY 10 /**< Delay in msec */


/*****************************************************************************
   typedefs
*****************************************************************************/
typedef enum
{
	SENSOR_TASK_INITIALIZE,
	SENSOR_TASK_START_MEASUREMENT,
	SENSOR_TASK_WAITING_FOR_MEASUREMENT,
	SENSOR_TASK_READ_MEASUREMENT
}sensor_task_states_t;



/*****************************************************************************
   Variables
*****************************************************************************/
static sht4x_device_t sht4x_sensor;
static I2C_HandleTypeDef * const sht4x_sensor_i2c_bus = &hi2c2;
static sensor_task_states_t sensor_task_state = SENSOR_TASK_INITIALIZE;

static millis_t sample_timer;
static millis_t sensor_update_timer;

// force sample at startup to determine if a cold start cycle is required
static bool force_initial_sample = true;

static uint16_t last_humidity_measurement;
static int16_t last_temperature_measurement;
static acp_result_t last_result = ACP_ERROR_FAULTY_SENSOR;


/*****************************************************************************
   Functions Prototypes
*****************************************************************************/
static void sensor_waiting_for_measurement(void);
static void sensor_task_initialize(void);
static void sensor_task_start_measurement(void);
static void sensor_task_read_measurement(void);
static HAL_StatusTypeDef read_temperature_humidity(float *temperature, float *humidity);


/*****************************************************************************
   Functions
*****************************************************************************/

/**
 * @brief The main function of the sensor task.
 *
 * This function is responsible for reading temperature and humidity
 * from the SHT4X sensor.
 */
void sensor_task(void)
{
	switch(sensor_task_state)
	{
	    case SENSOR_TASK_INITIALIZE:
	    	sensor_task_initialize();
	    	break;

	    case SENSOR_TASK_START_MEASUREMENT:
	    	sensor_task_start_measurement();
	    	break;

	    case SENSOR_TASK_WAITING_FOR_MEASUREMENT:
	    	sensor_waiting_for_measurement();
		    break;

	    case SENSOR_TASK_READ_MEASUREMENT:
	    	sensor_task_read_measurement();
		    break;
	}
}

/**
 * @brief Reads the last recorded temperature measurement from the SHT4X sensor.
 *
 * @param temperature Pointer to a variable where the temperature will be stored.
 *
 * @return The result of the last sensor measurement.
 */
acp_result_t sensor_task_read_temperature(int16_t *temperature)
{
	*temperature = last_temperature_measurement;
    return last_result;
}

/**
 * @brief Reads the last recorded humidity measurement from the SHT4X sensor.
 *
 * @param humidity Pointer to a variable where the humidity will be stored.
 *
 * @return The result of the last sensor measurement.
 */
acp_result_t sensor_task_read_humidity(uint16_t *humidity)
{
	*humidity = last_humidity_measurement;
	return last_result;
}

/*****************************************************************************
  static functions
*****************************************************************************/
/**
 * @brief Initializes the sensor task.
 *
 * This function initializes the SHT4X sensor and sets the state to
 * SENSOR_TASK_START_MEASUREMENT.
 */
static void sensor_task_initialize(void)
{
	sample_timer = millis();
	sht4x_init(&sht4x_sensor, SENSOR_NAME, sht4x_sensor_i2c_bus);
	sensor_task_state = SENSOR_TASK_START_MEASUREMENT;
}


/**
 * @brief Starts the measurement of temperature and humidity using SHT4x sensor.
 *
 * This function checks whether it is time to start a new measurement. If it is, the function
 * starts the measurement using the SHT4x sensor and sets the state of the sensor task to
 * SENSOR_TASK_WAITING_FOR_MEASUREMENT.
 *
 * @return void
 */
static void sensor_task_start_measurement(void)
{
    if (millis_timeout(&sample_timer, SAMPLE_INTERVAL) || force_initial_sample)
    {
    	force_initial_sample = false;
        sht4x_start_temperature_humidity_measurement(&sht4x_sensor);
        sensor_update_timer = millis();
        sensor_task_state = SENSOR_TASK_WAITING_FOR_MEASUREMENT;
    }
}


/**
 * @brief Waits for the measurement to be completed.
 *
 * This function waits for a certain delay after starting the measurement, so that the
 * measurement can be completed. After the delay, the state of the sensor task is set to
 * SENSOR_TASK_READ_MEASUREMENT.
 *
 * @return void
 */
static void sensor_waiting_for_measurement(void)
{
    if (millis_timeout(&sensor_update_timer, SENSOR_UPDATE_DELAY))
    {
        sensor_task_state = SENSOR_TASK_READ_MEASUREMENT;
    }
}


/**
 * @brief Reads the measurement of temperature and humidity from the SHT4x sensor.
 *
 * This function reads the measurement of temperature and humidity from the SHT4x sensor
 * using the read_temperature_humidity() function. The temperature and humidity values
 * are stored in the last_temperature_measurement and last_humidity_measurement variables,
 * respectively. The state of the sensor task is set to SENSOR_TASK_START_MEASUREMENT.
 *
 * @return void
 */
static void sensor_task_read_measurement(void)
{
    float temperature = 0.0;
    float humidity = 0.0;

    last_result = hal_status_to_acp_result(read_temperature_humidity(&temperature, &humidity));
    last_temperature_measurement = (int16_t)(temperature * 10);
    last_humidity_measurement = (uint16_t)(humidity *10);
    sensor_task_state = SENSOR_TASK_START_MEASUREMENT;

#if Debug
    char buffer[100];
    sprintf(buffer,"Temp = %d C Humidity = %d %% \r\n" , last_temperature_measurement, last_humidity_measurement);
    HAL_UART_Transmit(&huart1, buffer, strlen(buffer), 100);
#endif
}


/**
 * @brief Reads the measurement of temperature and humidity from the SHT4x sensor.
 *
 * This function reads the measurement of temperature and humidity from the SHT4x sensor
 * using the sht4x_read_temperature_humidity_measurements() function. The temperature and
 * humidity values are stored in the variables pointed to by the temperature and humidity
 * parameters, respectively.
 *
 * @param temperature Pointer to a float variable that will store the temperature value.
 * @param humidity Pointer to a float variable that will store the humidity value.
 * @return HAL_StatusTypeDef Returns HAL_OK if the measurement is successful, or HAL_ERROR
 * if there is an error reading the measurement.
 */
static HAL_StatusTypeDef read_temperature_humidity(float *temperature, float *humidity)
{
    return sht4x_read_temperature_humidity_measurements(&sht4x_sensor, temperature, humidity);
}
