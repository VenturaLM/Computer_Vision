/*
*	File:	README.txt
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

Compilation:

	- Open a terminal in the current directory and type the following:
		- mkdir build
		- cd build
		- cmake ..
		- make

Execution:

	- ./i72lumav ../testcall.mp4 -n -a=0.25 -t=0.5 -s=9 -r=7 -g
	- ./i72lumav ../testcall.mp4 -n -a=0.5 -t=0.025 -s=10 -r=9 -g
	- ./i72lumav ../testcall.mp4 -n -a=0.75 -t=0.075 -r=25
	- ./i72lumav ../testcall.mp4 -n -a=0.75 -t=0.05 -s=15 -r=19 -g
	- ./i72lumav ../testcall.mp4 -a=0.5
	- ./i72lumav ../testcall.mp4 -s=12
	- ./i72lumav ../testcall.mp4 -a=0.5 -s=12
	- ./i72lumav ../testcall.mp4 -a=0.5 -s=12 -t=0.5
