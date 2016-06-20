run: main
	./main >> Log.txt

main: main.o point.o shape.o sphere.o triangle.o ray.o
	g++ -o main main.o point.o shape.o sphere.o triangle.o ray.o bmp2/EasyBMP.cpp -lm -O3

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

clean:
	rm -f *.o main Log.txt
