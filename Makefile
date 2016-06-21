run: main
	./main

main: main.o point.o shape.o sphere.o triangle.o ray.o material.o light.o
	g++ -o main main.o point.o shape.o sphere.o triangle.o ray.o material.o light.o bmp2/EasyBMP.cpp -lm -O3

main.o: main.cpp
	g++ -c main.cpp

point.o: point.cpp
	g++ -c point.cpp

shape.o: shape.cpp
	g++ -c shape.cpp

sphere.o: sphere.cpp
	g++ -c sphere.cpp

triangle.o: triangle.cpp
	g++ -c triangle.cpp

ray.o: ray.cpp
	g++ -c ray.cpp

material.o: material.cpp
	g++ -c material.cpp

light.o: light.cpp
	g++ -c light.cpp

clean:
	rm -f *.o main Log.txt
