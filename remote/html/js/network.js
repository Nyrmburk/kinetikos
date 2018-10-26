//////  networking  //////

function Network(dict, websocket) {
	this.dict = dict;
	this.socket = websocket;
	socket.binaryType = "arraybuffer";

	socket.onmessage = function(evt) {
		var data = new BSON().deserialize(Buffer.from(evt.data));
		//console.log("subject: " + data.subject);
		//console.log("body: ");
		//console.log(data.body);

		if (data.subject in dict)
			dict[data.subject].call(this, data.body);
	};

	this.packet = {subject:null, body:null};
}

Network.prototype.send = function(subject, body) {
	this.packet.subject = subject;
	this.packet.body = body;
	this.socket.send(new BSON().serialize(this.packet));
}

