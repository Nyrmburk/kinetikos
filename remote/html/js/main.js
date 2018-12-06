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

	this.client = new Client(new WebSocket(getUri(8080)), robot);

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

