# LandControl

This is mainly for the Particle Electron but can also be used on a Particle Photon.

Have not triled this on any other device.

This is for controlling the power and a remote location with only cellular connectivity.

This contains the following ControlEverything devices



2 @ PR16-17-30A Electron interface 2 relay 30A  Address (0,0,0) (0,0,1)

1 @ PR8-15-30   I2C interface 2 relay 30A       Address (1,0,0)

1 @ PR8-14-20   I2C interface 4 relay 20A       Address (0,1,0)

1 @ PR2-25      I2C Temperature sensor

1 @ PR2-12      I2C Temperature and Humidity sensor



PR16-17-30A Address (0,0,1) contains the Particle Electron (is the start I2C out only)

PR16-17-30A Address (0,0,0) contains a I2C input (is the end I2C in only)

PR8-15-30   Address (1,0,0) is in the middle (I2C in and out)

PR8-14-20   Address (0,1,0) is in the middle (I2C in and out)

PR2-25      Only 1 address, is in the middle (I2C in and out)

PR2-12      Address (0,0,0) is in the middle (I2C in and out)



Variables -


Hi2cdevice

Ti2cdevice

humidity

hcTemp

hfTemp

tcTemp

tfTemp

Count

Version


Functions --


r2aStatus   Controls PR16-17-30A Address (0,0,0)

r2bStatus   Controls PR8-15-30   Address (1,0,0)

r2cStatus   Controls PR16-17-30A Address (0,0,1)

r4aStatus   Controls PR8-14-20   Address (0,1,0)


  
