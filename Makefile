all:
	cd src/ && make proc_gen
	cd ..

run: all
	cd src/ && ./proc_gen
	cd ..

clean:
	cd src/ && make clean
	cd ..