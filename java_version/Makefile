EXECBIN = run

SOURCE  = ReadCSVExample1

all: $(EXECBIN) clean

$(EXECBIN): $(SOURCE).java
	javac $<
	java $(SOURCE)

clean:
	rm -rf *.class
