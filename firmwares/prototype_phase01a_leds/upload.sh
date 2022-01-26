arduino-cli compile --fqbn arduino:megaavr:uno2018 &&
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:megaavr:uno2018 &&
picocom -b 115200 /dev/ttyACM0
