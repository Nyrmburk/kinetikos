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

	document.addEventListener('keydown', keyDown);
	document.addEventListener('keyup', keyUp);

	var videostreamDiv = document.getElementById("videostream");
	var videoCanvas = document.createElement("canvas");
	videoCanvas.width = 640;
	videoCanvas.height = 480;
	videostreamDiv.appendChild(videoCanvas);
	streamPlayer = new WSAvcPlayer(videoCanvas, "webgl");
	//var streamSocket = new WebSocket(uri, "h264");
	//streamPlayer.connect(videoSocket);
	streamPlayer.connect(getUri(8081));

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
}

function joystickMove() {
	if (current) {
		var u = joystick.deltaX() / joystick._stickRadius;
		var y = -joystick.deltaY() / joystick._stickRadius;

		client.sendJoystick(0, y, u, 0);
	}
}

function joystickEnd() {
	current = false;
	client.sendJoystick(0, 0, 0, 0);
}

var moves = {
	forward: 0,
	backward: 0,
	left: 0,
	right: 0,
	rotateLeft: 0,
	rotateRight: 0
}

function keyChange(code, value) {
	switch (code) {
		case 87: // W
			moves.forward = value;
			break;
		case 83: // S
			moves.backward = value;
			break;
		case 65: // A
			moves.left = value;
			break;
		case 68: // D
			moves.right = value;
			break;
		case 81: // Q
			moves.rotateLeft = value;
			break;
		case 69: // E
			moves.rotateRight = value;
			break;
	}
	client.sendJoystick(
			moves.right - moves.left, 
			moves.forward - moves.backward, 
			moves.rotateRight - moves.rotateLeft, 0);
	console.log(moves.forward - moves.backward);
}

function keyDown(event) {
	console.log('key ' + event.keyCode + ' down');
	keyChange(event.keyCode, 1);
}

function keyUp(event) {
	console.log('key ' + event.keyCode + ' up');
	keyChange(event.keyCode, 0);
}
