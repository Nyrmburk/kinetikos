function Client(protocol, websocket) {
	this.protocol = protocol;
	this.protocol.client = this;

	this.socket = websocket;
	this.socket.binaryType = "arraybuffer";

	this.socket.onopen = function(evt) {
		protocol.onopen(evt);
	};
	this.socket.onclose = function(evt) {
		protocol.onclose(evt);
	};
	this.socket.onmessage = function(evt) {
		protocol.handleMessage(evt.data);
	};
}

Client.prototype.send = function(data) {
	this.socket.send(data);
}

