var renderer;
var client;
var streamPlayer;
var joystick;
var gui;

window.onload = function() {
	console.log("Murphy the Spider Robot V2");
	console.log("");

	var robot = new Robot();

	var rendererDiv = document.getElementById("renderer");;
	renderer = new Renderer3d(rendererDiv, robot);
	window.addEventListener('resize', function() {renderer.onResize()}, false);

	var allContainer = document.getElementById("all");
	joystick = new VirtualJoystick({
			container: allContainer,
			mouseSupport: true,
			limitStickTravel: true,
			stickRadius: 150});

	allContainer.addEventListener("mousedown", joystickStart);
	allContainer.addEventListener("touchstart", joystickStart);
	allContainer.addEventListener("mousemove", joystickMove);
	allContainer.addEventListener("touchmove", joystickMove);
	allContainer.addEventListener("mouseup", joystickEnd);
	allContainer.addEventListener("touchend", joystickEnd);

	var videostreamDiv = document.getElementById("videostream");
	var videoCanvas = document.createElement("canvas");
	videoCanvas.width = 640;
	videoCanvas.height = 480;
	videostreamDiv.appendChild(videoCanvas);
	streamPlayer = new WSAvcPlayer(videoCanvas, "webgl");
	//var streamSocket = new WebSocket(uri, "h264");
	//streamPlayer.connect(videoSocket);
	//streamPlayer.connect(getUri(3000));

	gui = new RobotGui();

	var socket = new ReconnectingWebSocket(getUri(8080), null, {automaticOpen: false});
	this.client = new Client(socket, robot);
	socket.open();

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

var current = false;
function joystickStart() {
	current = true;
	console.log("start");
}

function joystickMove() {
	if (current) {
		console.log("move");
		var u = joystick.deltaX() / joystick._stickRadius;
		var y = -joystick.deltaY() / joystick._stickRadius;

		client.sendJoystick(0, y, u, 0);
	}
}

function joystickEnd() {
	current = false;
	console.log("end");
	client.sendJoystick(0, 0, 0, 0);
}
