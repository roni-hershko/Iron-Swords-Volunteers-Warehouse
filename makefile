
all: src

bin/BaseAction.o: src/BaseAction.cpp
    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/BaseAction.o src/BaseAction.cpp

bin/WareHouse.o: src/WareHouse.cpp
    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp

bin/customer.o: src/customer.cpp
    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/customer.o src/customer.cpp

bin/order.o: src/order.cpp
    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/order.o src/order.cpp
	
bin/volunteer.o: src/volunteer.cpp
    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/orvolunteerder.o src/volunteer.cpp
.PHONY: clean
clean:
	rm -f bin/*
