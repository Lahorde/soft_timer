/******************************************************************************
 * @file    timer.h
 * @author  Rémi Pincent - INRIA
 * @date    01/03/2018
 *
 * @brief Simple 32bits software timer with a max of 1ms resolution (depending on
 * tick feeding timer).
 *
 * Project : soft_timer
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 *
 * Revision History:
 * Insert github reference
 *
 * LICENSE :
 * soft_timer (c) by Rémi Pincent
 * soft_timer is licensed under a
 * Creative Commons Attribution-NonCommercial 3.0 Unported License.
 *
 * You should have received a copy of the license along with this
 * work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
 *****************************************************************************/
#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif
/**************************************************************************
 * Include Files
 **************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/
typedef void (*TTimerCb)(void*);

typedef struct
{
	uint32_t _u32_count;
	uint32_t _u32_period;
	TTimerCb _pfn_cb;
	void* _p_cbParam;
}TsTimer;

/**************************************************************************
 * Global variables
 **************************************************************************/

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Global Functions Declarations
 **************************************************************************/
void Timer_init(TsTimer*);

void Timer_notifyAfter(TsTimer*, uint32_t arg_u32_ms, void* arg_p_cbParam);

void Timer_periodicNotify(TsTimer*, uint32_t arg_u32_ms, void* arg_p_cbParam);

void Timer_cancel(TsTimer*);

bool Timer_isActive(TsTimer*);

void Timer_tick(TsTimer*, uint32_t arg_u32_ms);

#ifdef __cplusplus
}
#endif
#endif /* TIMER_H */
