class Client extends RobotProtocol {
	constructor(websocket, robot, renderer) {
		super(robot, renderer);

		this.socket = websocket;
		this.socket.binaryType = "arraybuffer";

		var self = this;

		this.socket.onopen = function(evt) {
			self.onopen(evt);
		};
		this.socket.onclose = function(evt) {
			self.onclose(evt);
		};
		this.socket.onmessage = function(evt) {
			self.handleMessage(evt.data);
		};
	}

	send(data) {
		var bytesWritten = data.getIndex();
		this.socket.send(data.buffer.slice(0, bytesWritten));
	}
}

