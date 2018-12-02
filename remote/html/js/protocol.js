function Protocol() {
	this.buffer = new DataView2(new ArrayBuffer(1024*1024));
}

Protocol.prototype.handleMessage = function(dataIn) {
	var reader = new DataView2(dataIn) // DataView is big-endian

	// get control code
	var controlCode = reader.getUint16();

	// handle message in implementation
	this.handle(controlCode, reader);
}

Protocol.prototype.send = function(controlCode, serial) {
	this.buffer.rewind();
	this.buffer.setUint16(controlCode);

	if (serial) {
		serial.serialize(this.buffer);
	}
	
	var bytesWritten = this.buffer.getIndex();
	this.client.send(this.buffer.buffer.slice(0, bytesWritten));
}
