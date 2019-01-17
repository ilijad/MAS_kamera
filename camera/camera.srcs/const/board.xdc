set_property -dict {PACKAGE_PIN T10 PULLUP true IOSTANDARD LVCMOS33} [get_ports iic_0_scl_io]
set_property -dict {PACKAGE_PIN Y18 PULLUP true IOSTANDARD LVCMOS33} [get_ports iic_0_sda_io]


#Creating SCL CLOCK at 100 KHz
create_clock -period 10000.000 -name IIC_SCL_CLOCK [get_ports iic_0_scl_io]

#create_clock -period 40 -name XCLK [get_ports XCLK]

##Input Delays

set_input_delay -clock IIC_SCL_CLOCK -clock_fall -max 0.000 [get_ports iic_0_sda_io]
set_input_delay -clock IIC_SCL_CLOCK -clock_fall -min 0.000 [get_ports iic_0_sda_io]


##Output Delays

set_output_delay -clock IIC_SCL_CLOCK -max -0.592 [get_ports iic_0_scl_io]
set_output_delay -clock IIC_SCL_CLOCK -min 0.040 [get_ports iic_0_scl_io]

set_output_delay -clock IIC_SCL_CLOCK -max -1.000 [get_ports iic_0_sda_io]
set_output_delay -clock IIC_SCL_CLOCK -min -1.000 [get_ports iic_0_sda_io]


set_property -dict {PACKAGE_PIN Y19 IOSTANDARD LVCMOS33 SLEW FAST OFFCHIP_TERM NONE} [get_ports XCLK]

set_property -dict {PACKAGE_PIN Y17 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[0]}]
set_property -dict {PACKAGE_PIN W14 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[1]}]
set_property -dict {PACKAGE_PIN W19 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[2]}]
set_property -dict {PACKAGE_PIN V16 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[3]}]
set_property -dict {PACKAGE_PIN Y16 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[4]}]
set_property -dict {PACKAGE_PIN Y14 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[5]}]
set_property -dict {PACKAGE_PIN W18 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[6]}]
set_property -dict {PACKAGE_PIN W16 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[7]}]
set_property -dict {PACKAGE_PIN T11 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[8]}]
set_property -dict {PACKAGE_PIN U19 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[9]}]
set_property -dict {PACKAGE_PIN V12 IOSTANDARD LVCMOS33} [get_ports {camera_tri_i[10]}]

## Buttons
set_property -dict {PACKAGE_PIN D19 IOSTANDARD LVCMOS33} [get_ports {btns_4bits_tri_i[0]}]
set_property -dict {PACKAGE_PIN D20 IOSTANDARD LVCMOS33} [get_ports {btns_4bits_tri_i[1]}]
set_property -dict {PACKAGE_PIN L20 IOSTANDARD LVCMOS33} [get_ports {btns_4bits_tri_i[2]}]
set_property -dict {PACKAGE_PIN L19 IOSTANDARD LVCMOS33} [get_ports {btns_4bits_tri_i[3]}]

## LEDs
set_property -dict {PACKAGE_PIN R14 IOSTANDARD LVCMOS33} [get_ports {leds_4bits_tri_io[0]}]
set_property -dict {PACKAGE_PIN P14 IOSTANDARD LVCMOS33} [get_ports {leds_4bits_tri_io[1]}]
set_property -dict {PACKAGE_PIN N16 IOSTANDARD LVCMOS33} [get_ports {leds_4bits_tri_io[2]}]
set_property -dict {PACKAGE_PIN M14 IOSTANDARD LVCMOS33} [get_ports {leds_4bits_tri_io[3]}]

