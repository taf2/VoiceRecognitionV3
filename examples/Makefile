ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

all: esp32_example

upload: esp32_example
	pio run -j 2 --target upload && sleep 1;  pio device monitor

esp32_example: configure src/main.cpp
	pio run 

monitor:
	pio device monitor

configure:
	pio lib install "TinyPICO Helper Library"
	mkdir -p include
	cp ../VoiceRecognitionV3.cpp src/VoiceRecognitionV3.cpp
	cp ../VoiceRecognitionV3.h include/VoiceRecognitionV3.h

clean:
	pio run --target clean
