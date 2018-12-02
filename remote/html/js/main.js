var renderer;
var client;
var streamPlayer;
var joystick;

window.onload = function() {
	console.log("Murphy the Spider Robot V2");
	console.log("");

	var robot = new Robot();

	var rendererDiv = document.getElementById("renderer");;
	renderer = new Renderer3d(rendererDiv, robot);
	window.addEventListener('resize', function() {renderer.onResize()}, false);

	joystick = new VirtualJoystick({
			container : document.body, 
			mouseSupport : true, 
			limitStickTravel : true, 
			stickRadius : 150});

	var videostreamDiv = document.getElementById("videostream");
	var videoCanvas = document.createElement("canvas");
	videoCanvas.width = 640;
	videoCanvas.height = 480;
	videostreamDiv.appendChild(videoCanvas);
	streamPlayer = new WSAvcPlayer(videoCanvas, "webgl");
	//var streamSocket = new WebSocket(uri, "h264");
	//streamPlayer.connect(videoSocket);
	//streamPlayer.connect(getUri(3000));

	var protocol = new RobotProtocol(robot, renderer);
	this.client = new Client(protocol, new WebSocket(getUri(8080)));

	render();
}

function getUri(port) {
	var uri = "ws://" + document.location.hostname;
	if (port) {
		uri += ":" + port;
	}
	return uri;
}

function render() {
	renderer.render();
	requestAnimationFrame(render);
}

// dumping ground
/*
var dict = {
	body:onGetBody,
	worldMap:onGetWorldMap,
	worldChange:onWorldChange,
	worldChunk:onGetWorldChunk,
	dynamic:onGetDynamic
};
*/

function initWebsockets() {
	var uri = "ws://" + document.location.hostname + ":8080";
	var socket = new WebSocket(uri, "bson-protocol");
	//var network = new Network(dict, socket);

	socket.onopen = function(evt) {
		console.log("socket opened");

		requestBody();
		requestWorldMap();
	};
	socket.onclose = function(evt) {
		console.log("socket closed");
	};
}


