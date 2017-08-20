# LandControl

This is mainly for the Particle Electron but can also be used on a Particle Photon. </br>
Have not triled this on any other device.</br>
This is for controlling the power and a remote location with only cellular connectivity.</br>
</br>
<b>This contains the following ControlEverything devices</b></br>
</br>
2 @ PR16-17-30A Electron interface 2 relay 30A  Address (0,0,0) (0,0,1)</br>
1 @ PR8-15-30   I2C interface 2 relay 30A       Address (1,0,0)</br>
1 @ PR8-14-20   I2C interface 4 relay 20A       Address (0,1,0)</br>
1 @ PR2-25      I2C Temperature sensor</br>
1 @ PR2-12      I2C Temperature and Humidity sensor</br>
</br>
PR16-17-30A Address (0,0,1) contains the Particle Electron (is the start I2C out only)</br>
PR16-17-30A Address (0,0,0) contains a I2C input (is the end I2C in only)</br>
PR8-15-30   Address (1,0,0) is in the middle (I2C in and out)</br>
PR8-14-20   Address (0,1,0) is in the middle (I2C in and out)</br>
PR2-25      Only 1 address, is in the middle (I2C in and out)</br>
PR2-12      Address (0,0,0) is in the middle (I2C in and out)</br></br>

<b>Variables -</b></br>

Hi2cdevice</br>
Ti2cdevice</br>
humidity</br>
hcTemp</br>
hfTemp</br>
tcTemp</br>
tfTemp</br>
Count</br>
Version</br></br>

<b>Functions --</b></br>

r2aStatus   Controls PR16-17-30A Address (0,0,0)</br>
r2bStatus   Controls PR8-15-30   Address (1,0,0)</br>
r2cStatus   Controls PR16-17-30A Address (0,0,1)</br>
r4aStatus   Controls PR8-14-20   Address (0,1,0)</br>


  
