# os-project-one
A project focusing on measuring performance parameters of various operating system level functions

Processor
	model: Intel core i7-4710HQ
	cycle time: 2.50GHz or approx. 700MHz x 8 ?
	cache sizes:
		L1: 32KB (Instruction)
		L1: 32KB (Data)
		L2: 256KB
		L3: 6144KB

SATA controller
	width: 32bits
	clock: 66MHz

Disk
	capacity: 1000GB
	RPM: 5400
	

RAM
	width: 64bits
	Size: 8192MB
	Type: DDR3 Synchronous
	Speed: 1600MHz
	Total slots: 4 (used 1)

OS
	Name: sourabhpc
	Kernel: 4.2.0-42-generic x86_64 (64 bit gcc: 5.2.1)
	Desktop: MATE 1.10.2 (Gtk 3.16.7-0ubuntu3.3) 
	Distro: Ubuntu 15.10 wily
	Laptop: Lenovo Y50-70

#Methods used
/proc/cpuinfo
lshw -short
lshw
/proc/meminfo
inxi -Fx
lspci
/proc/diskstats
lsblk
dmidecode
hardinfo (program)
hdparm -I /sda/dev
