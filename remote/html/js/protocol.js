function Protocol() {
	this.buffer = new ArrayBuffer(1024, 1024);

	this.Opcode = {
		opPublish:     10,
        opSubscribe:   11,
        opUnsubscribe: 12
	}
}

Protocol.prototype.handleMessage = function(dataIn) {
	var reader = new DataView2(dataIn) // DataView is big-endian

	// get control code
	var controlCode = reader.getUint16();

	// handle message in implementation
	this.handle(controlCode, reader);
}

Protocol.prototype.sendSerial = function(controlCode, serial) {
	var data = new DataView2(this.buffer);
	data.setUint16(controlCode);

	if (serial) {
		serial.serialize(data);
	}
	
	this.send(data);
}
