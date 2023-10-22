/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stm32f4xx_hal.h>
#include "diag/Trace.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

I2C_HandleTypeDef hi2c1;
float temp;
float read_dht11() {
  // Khởi tạo giao tiếp I2C
  HAL_I2C_Init(&hi2c1);

  // Gửi lệnh đọc dữ liệu
  uint8_t cmd[2] = {0x01, 0x00};
  HAL_I2C_Master_Transmit(&hi2c1, 0x48, cmd, 2, 1000);

  // Đọc dữ liệu
  uint8_t buf[5];
  HAL_I2C_Master_Receive(&hi2c1, 0x48, buf, 5, 1000);

  // Giải mã dữ liệu
  temp = ((buf[0] << 8) | buf[1]) / 10;

  return temp;
}
int
main(int argc, char* argv[])
{
		trace_printf("Start process",0);
		//
		HAL_Init();
	  // Khởi tạo giao tiếp I2C
	  hi2c1.Instance = I2C1;
	  hi2c1.Init.ClockSpeed = 100000;

	  hi2c1.Init.OwnAddress1 = 0x00;
	  hi2c1.Init.AddressingMode = 7 ;
	  HAL_I2C_Init(&hi2c1);

	  // Khởi tạo vòng lặp định kỳ
	  struct timeval start, end;
	  gettimeofday(&start, NULL);
	  while (1) {
	    // Đọc dữ liệu từ cảm biến
	    temp = read_dht11();

	    // In dữ liệu nhiệt độ lên màn hình console
	    trace_printf("Nhiệt độ: %.2f độ C\n", temp);

	    // Ngủ 2 giây
	    gettimeofday(&end, NULL);
	    int diff = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
	    if (diff < 2000000) {

	    }

	  }

	  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
