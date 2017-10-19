
1. Enter the following into the ground command line:    /sbin/ifconfig eth0|grep inet

the first number returned is the IP of the ground station, referred to as <server-ip> and formatted as in the following example (including the quotes): "192.168.0.1"

2. Enter the following into the ground command line:	python3 Ground.py <server-ip> <server-port>
<server-port> is any integer > 1024, typed without quotes

3. Enter the following into the rocket command line:    python3 Rocket.py <server-ip> <server-port> 






