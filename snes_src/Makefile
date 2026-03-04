all: main.bin

build/main.o: code/main.s hacks/*
	wla-65816 -h -o $@ $<

main.bin: build/main.o
	wlalink -b -S linkfile $@

clean:
	rm build/main.o main.bin main.sym
packets:
	python3 tools/gen_packets.py 0902
