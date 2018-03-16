/******************************************************************************
 * @file    timer.c
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

/**************************************************************************
 * Include Files
 **************************************************************************/
#include "timer.h"
#include "assert.h"
#include "tick_listener.h"
#include <stddef.h>

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**************************************************************************
 * Global Variables
 **************************************************************************/

/**************************************************************************
 * Static Variables
 **************************************************************************/

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Local Functions Declarations
 **************************************************************************/

/**************************************************************************
 * Global Functions Definitions
 **************************************************************************/
void Timer_init(TsTimer* arg_ps_timer)
{
	assert(arg_ps_timer != NULL
			&& arg_ps_timer->_pfn_cb != NULL);

	arg_ps_timer->_p_cbParam = NULL;
	arg_ps_timer->_u32_period = 0;
	arg_ps_timer->_u32_count = 0;
}

void Timer_notifyAfter(TsTimer* arg_ps_timer, uint32_t arg_u32_ms, void* arg_p_cbParam)
{
	assert(arg_ps_timer != NULL
			&& arg_u32_ms > 0
			&& arg_ps_timer->_pfn_cb != NULL
			&& !Timer_isActive(arg_ps_timer));

	TsTickListener loc_s_tickListener = {
			._pfn_cb = (void (*)(void*, uint32_t)) &Timer_tick,
			._p_cbData = arg_ps_timer
	};
	TickListener_registerListener(&loc_s_tickListener);

	arg_ps_timer->_u32_count = arg_u32_ms;
	arg_ps_timer->_u32_period = 0;
	arg_ps_timer->_p_cbParam = arg_p_cbParam;
}

void Timer_periodicNotify(TsTimer* arg_ps_timer, uint32_t arg_u32_ms, void* arg_p_cbParam)
{
	assert(arg_ps_timer != NULL
			&& arg_u32_ms > 0
			&&arg_ps_timer->_pfn_cb != NULL
			&& !Timer_isActive(arg_ps_timer));

	TsTickListener loc_s_tickListener = {
			._pfn_cb = (void (*)(void*, uint32_t)) &Timer_tick,
			._p_cbData = arg_ps_timer
	};
	TickListener_registerListener(&loc_s_tickListener);

	arg_ps_timer->_u32_count = arg_u32_ms;
	arg_ps_timer->_u32_period = arg_u32_ms;
	arg_ps_timer->_p_cbParam = arg_p_cbParam;
}

void Timer_cancel(TsTimer* arg_ps_timer)
{
	assert(arg_ps_timer != NULL);

	TsTickListener loc_s_tickListener = {
			._pfn_cb = (void (*)(void*, uint32_t)) &Timer_tick,
			._p_cbData = arg_ps_timer
	};
	TickListener_unregisterListener(&loc_s_tickListener);

	arg_ps_timer->_u32_count = 0;
	arg_ps_timer->_u32_count = 0;
	arg_ps_timer->_u32_period = 0;
}

bool Timer_isActive(TsTimer* arg_ps_timer)
{
	assert(arg_ps_timer != NULL);
	return arg_ps_timer->_u32_count > 0;
}

/**
 * Called under interrupted context
 * @param arg_ps_timer
 * @param arg_u32_ms
 */
void Timer_tick(TsTimer* arg_ps_timer, uint32_t arg_u32_ms)
{
	if(Timer_isActive(arg_ps_timer))
	{
		/** timer elapsed */
		if(arg_u32_ms >= arg_ps_timer->_u32_count)
		{
			if(arg_ps_timer->_u32_period > 0)
			{
				arg_ps_timer->_u32_count = arg_ps_timer->_u32_period - (arg_u32_ms - arg_ps_timer->_u32_count);
			}
			else
			{
				Timer_cancel(arg_ps_timer);
			}
			/** callback called under interrupted context */
			arg_ps_timer->_pfn_cb(arg_ps_timer->_p_cbParam);
		}
		else
		{
			arg_ps_timer->_u32_count -= arg_u32_ms ;
		}
	}
}
 /**************************************************************************
 * Local Functions Definitions
 **************************************************************************/

