# Main code build rules

src-y += sched.c command.c basecmd.c debugcmds.c
src-$(CONFIG_HAVE_GPIO) += initial_pins.c gpiocmds.c stepper.c endstop.c \
    trsync.c
src-$(CONFIG_HAVE_GPIO_ADC) += adccmds.c
src-$(CONFIG_HAVE_GPIO_SPI) += spicmds.c
src-$(CONFIG_HAVE_GPIO_SDIO) += sdiocmds.c
src-$(CONFIG_HAVE_GPIO_I2C) += i2ccmds.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += pwmcmds.c

src-$(CONFIG_WANT_GPIO_BITBANGING) += buttons.c tmcuart.c neopixel.c \
    pulse_counter.c
#src-$(CONFIG_WANT_DISPLAYS) += lcd_st7920.c lcd_hd44780.c
src-$(CONFIG_WANT_SOFTWARE_SPI) += spi_software.c
src-$(CONFIG_WANT_SOFTWARE_I2C) += i2c_software.c
sensors-src-$(CONFIG_HAVE_GPIO_SPI) := thermocouple.c sensor_adxl345.c \
    sensor_angle.c
#src-$(CONFIG_WANT_LIS2DW) += sensor_lis2dw.c
#sensors-src-$(CONFIG_HAVE_GPIO_I2C) += sensor_mpu9250.c
src-$(CONFIG_WANT_SENSORS) += $(sensors-src-y)
src-y += sensor_cs1237.c sensor_lis2dw12.c sensor_sc7a20.c
src-y += ota_data.c otacmd.c 
