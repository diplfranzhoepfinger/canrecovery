
 

#ifndef COMPONENTS_GPIO_ABSTRACTION_INCLUDE_GPIO_ABSTRACTION_H_
#define COMPONENTS_GPIO_ABSTRACTION_INCLUDE_GPIO_ABSTRACTION_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/***************** Inclusions **************************/
/*******************************************************/
#include "sdkconfig.h"
#include "esp_err.h"
#include <stdbool.h>



/*******************************************************/
/***************** Interface Macros ********************/
/*******************************************************/


/*******************************************************/
/***************** Interface types *********************/
/*******************************************************/
/**
 * Description of the interface type for the SWC x
 */


/*******************************************************/
/***************** Interface functions *****************/
/*******************************************************/





/**
 * call this 1 time to init DI1 and DI2 Monitoring with Debouncing.
 * only these 2 Inputs are debounced.
 *
 * all others are not debounced.

 */
extern esp_err_t gpio_init_DI1_DI2(void);
extern esp_err_t gpio_delete_DI1_DI2(void);





/**
 * @brief  GPIO get input level
 *
 * @warning If the pad is not configured for input (or input and output) the returned value is always 0.
 *
 *
 * @return
 *     - 0 the GPIO input level is 0
 *     - 1 the GPIO input level is 1
 *
 */
extern int gpio_get_level_DI1(void);
extern int gpio_get_level_DI2(void);



#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* COMPONENTS_GPIO_ABSTRACTION_INCLUDE_GPIO_ABSTRACTION_H_ */
