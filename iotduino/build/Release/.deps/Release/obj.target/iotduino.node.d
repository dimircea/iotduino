cmd_Release/obj.target/iotduino.node := flock ./Release/linker.lock g++ -shared -pthread -rdynamic  -Wl,-soname=iotduino.node -o Release/obj.target/iotduino.node -Wl,--start-group Release/obj.target/iotduino/../common/src/Duino.o Release/obj.target/iotduino/src/iotduino.o -Wl,--end-group 
