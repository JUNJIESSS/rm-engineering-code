#include "filter.h"
/**
  * @brief
  * @author
  * @param[in] 
  * @param[in] 
  * @retval
  */
first_order_filter chassis_cmd_vx;
first_order_filter chassis_cmd_vy;
first_order_filter chassis_cmd_rotate;

float first_order_filter_cali(first_order_filter_t filter, float input)
{
	  filter->out =filter->num / (filter->num + filter->frame_period) * filter->out + filter->frame_period / (filter->num+ filter->frame_period) * (input);
    return filter->out;
}
void first_order_filter_init(first_order_filter_t filter, float num, float frame_period)
{
	filter->num = num;
	filter->frame_period = frame_period;
}

