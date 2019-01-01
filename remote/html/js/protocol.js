class Protocol {
	constructor() {
		this.buffer = new ArrayBuffer(1024, 1024);

		this.Opcode = {
			opPublish:     10,
			opSubscribe:   11,
			opUnsubscribe: 12
		}
	}

	handleMessage(dataIn) {
		var reader = new DataView2(dataIn) // DataView is big-endian

		// get control code
		var controlCode = reader.getUint16();

		// handle message in implementation
		this.handle(controlCode, reader);
	}

	sendSerial(controlCode, serial) {
		var data = new DataView2(this.buffer);
		data.setUint16(controlCode);

		if (serial) {
			serial.serialize(data);
		}

		this.send(data);
	}

	subscribe(opcode, roomId) {
		var s = {serialize: function(data) {data.setUint16(roomId)}};
		this.sendSerial(opcode, s);
	}
}
