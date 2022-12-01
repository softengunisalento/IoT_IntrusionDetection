#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contiki.h"
#include "nn.h"
#include "coap-engine.h"
#include "result.h"
#include "result.c"

#if PLATFORM_SUPPORTS_BUTTON_HAL
	#include "dev/button-hal.h"
#else
	#include "dev/button-sensor.h"
#endif

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "COAP Server"
#define LOG_LEVEL LOG_LEVEL_APP
/*
 * Resources to be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
extern coap_resource_t
  res_anomaly,
  res_hello,
  res_mirror,
  res_chunks,
  res_separate,
  res_push,
  res_event,
  res_sub,
  res_b1_sep_b2;

#if PLATFORM_HAS_LEDS
	extern coap_resource_t res_leds, res_toggle;
#endif

#if PLATFORM_HAS_LIGHT
	#include "dev/light-sensor.h"
	extern coap_resource_t res_light;
#endif

#if PLATFORM_HAS_BATTERY
	#include "dev/battery-sensor.h"
	extern coap_resource_t res_battery;
#endif

#if PLATFORM_HAS_TEMPERATURE
	#include "dev/temperature-sensor.h"
	extern coap_resource_t res_temperature;
#endif

void DebugLog(const char* s) {
  if(s) {
    printf("%s", s);
  }
}


/*---------------------------------------------------------------------------*/
PROCESS(ml_process, "ML Process");
PROCESS(coap_process, "Coap process");
AUTOSTART_PROCESSES(&coap_process, &ml_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(coap_process, ev, data)
{
    LOG_INFO("Coap process started\n");
    PROCESS_BEGIN();

    PROCESS_PAUSE();
    LOG_INFO("Starting Erbium Example Server\n");

    //Bind the resources to their Uri-Path.
    //WARNING: Activating twice only means alternate path, not two instances!
    //All static variables are the same for each URI path.
    coap_activate_resource(&res_anomaly, "anomaly");
    coap_activate_resource(&res_hello, "test/hello");
    coap_activate_resource(&res_separate, "test/separate");
    coap_activate_resource(&res_sub, "test/sub");
	coap_activate_resource(&res_mirror, "debug/mirror");
	coap_activate_resource(&res_chunks, "test/chunks");
	coap_activate_resource(&res_separate, "test/separate");
	coap_activate_resource(&res_push, "test/push");
	#if PLATFORM_HAS_BUTTON
		coap_activate_resource(&res_event, "sensors/button");
	#endif /* PLATFORM_HAS_BUTTON */
		coap_activate_resource(&res_sub, "test/sub");
		coap_activate_resource(&res_b1_sep_b2, "test/b1sepb2");
	#if PLATFORM_HAS_LEDS
	/*  coap_activate_resource(&res_leds, "actuators/leds"); */
	coap_activate_resource(&res_toggle, "actuators/toggle");
	#endif

	#if PLATFORM_HAS_LIGHT
		coap_activate_resource(&res_light, "sensors/light");
		SENSORS_ACTIVATE(light_sensor);
	#endif
	#if PLATFORM_HAS_BATTERY
		coap_activate_resource(&res_battery, "sensors/battery");
		SENSORS_ACTIVATE(battery_sensor);
	#endif
	#if PLATFORM_HAS_TEMPERATURE
		coap_activate_resource(&res_temperature, "sensors/temperature");
		SENSORS_ACTIVATE(temperature_sensor);
	#endif

    while(1) {
        PROCESS_WAIT_EVENT();
        LOG_INFO("Coap loop started\n");
        #if PLATFORM_HAS_BUTTON
            #if PLATFORM_SUPPORTS_BUTTON_HAL
                if(ev == button_hal_release_event) {
            #else
                if(ev == sensors_event && data == &button_sensor) {
            #endif //PLATFORM_SUPPORTS_BUTTON_HAL
            LOG_DBG("*******BUTTON*******\n");
            res_event.trigger();
            res_separate.resume();
            }
        #endif //PLATFORM_HAS_BUTTON

    LOG_INFO("CHANGE PROCESS\n");
    process_poll(&ml_process);
    PROCESS_YIELD();
    }
    PROCESS_END();
}
//---------------------------------------------------------------------------
PROCESS_THREAD(ml_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    LOG_INFO("ML Process Started");
    if(nn_setup() >= 0) { //Esegue il setup dell'algoritmo di ML
        //Setup a periodic timer that expires after 1 second.
        etimer_set(&timer, CLOCK_SECOND * 1);
        while(1) {
            LOG_INFO("Running inference: \n");
            int result = nn_run_inference(); //1 = Normal ; 0 = Attack
            setStatus(result); //salva il risultato nella variabile statica
                               //contenuta in result.c
            //LOG_INFO("Battery: %d\n", battery_sensor.value(0));
            //LOG_INFO("Temperature: %d\n", temperature_sensor.value(0));
            if(result < 0) {
            LOG_INFO("nn_run_inference() failed\n");
            }
            //Wait for the periodic timer to expire and then restart the timer.
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
            etimer_reset(&timer);
            }
    }else {
        LOG_INFO("nn_setup failed\n");
    }
    LOG_INFO("CHANGE PROCESS\n");
    process_poll(&coap_process); //passa al processo server
    PROCESS_YIELD();

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
