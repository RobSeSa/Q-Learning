EXECBIN = run

SOURCE  = Q_Learning

all: $(EXECBIN) clean

$(EXECBIN): $(SOURCE).java
	javac $<
	java $(SOURCE)

time: $(SOURCE).java clean
	javac $<
	time java $(SOURCE)

clean:
	@rm -rf *.class
