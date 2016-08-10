1. What is this
  This porject aims to use simple AVR microcontroler and LAN network to drive electromagnetic relays and read 
  temperarure via DS18B20 OW and humidity using TH sensors series.
  Whole device is closed in DIN size enclosure to put inside home switchgear
  
2. Directories structure
├── Android	control app for android
├── AVR		main AVR app	
├── Doc		Documentation,pdf's and other
└── x86		configuration tool for PC,Qt based

3. Used Software/source code

  - Android Studio
  - Qt Creator
  - Cmake
  - cmake scripts for AVR by Matthias Kleemann
  - network stack library by Pascal Stang
  - enc28j60 driver

4.TODO
  - add renaming avr device and display avr device insted of ip address
  - add external ip address to connect with when client is not connected to local network

  
