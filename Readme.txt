Project Setup Intructions 

1. Install all the dependencies 
	1. Install crow 
	2. install boost library and SSL library because crow requires boost & SSL library to work
		sudo apt-get install libboost-all-dev libssl-dev


2. Directory Structure
	ecommerce-api/
	│
	├── main.cpp
	├── crow_all.h
	└── CMakeLists.txt

3. Define the API (main.cpp)

4. CMAke Configurations (CMakeLists.txt)

5. Build and run the API

   1. Build the project using CMake 
   	mkdir build
	cd build
	cmake ..
	make
    
    2. run the application
    	./EcommerceAPI

	The Application will run on 'http://localhost:8080'.
