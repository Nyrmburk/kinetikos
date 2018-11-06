function Packet() {
	Serializable.call(this);
	controlCode = 0;

	Packet.prototype.serialize = function(view) {
		this.controlCode = view.getUint16();
	}

	Packet.prototype.deserialize = function(view) {
		this.controlCode = view.setUint16();
	}
}

function Protocol(procedures) {
	this.packet = new Packet();
	this.procedures = procedures;
}

Protocol.prototype.handleMessage = function(bufferIn, bufferOut) {
	var reader = new DataView(bufferIn) // DataView is big-endian

	// get control code
	packet.deserialize(reader);
	var controlCode = packet.controlCode;

	// execute procedure and get its response
	var response = procedures[controlCode](reader);

	if (!response)
		return;

	// write the response out
	var writer = new DataView(bufferOut);
	packet.serialize(writer);
	reponse.serialize(writer);
}
