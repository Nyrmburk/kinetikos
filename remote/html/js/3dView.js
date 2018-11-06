if (!WEBGL.isWebGLAvailable()) {
    var warning = WEBGL.getWebGLErrorMessage();
    // document.getElementById('container').appendChild(warning);
	alert("WebGL not available");
	throw new Error("WebGL not available");
}

var dict = {
	body:onGetBody,
	worldMap:onGetWorldMap,
	worldChange:onWorldChange,
	worldChunk:onGetWorldChunk,
	dynamic:onGetDynamic
};

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

var videoCanvas = document.createElement("canvas");
var videoPlayer = new WSAvcPlayer(videoCanvas, "webgl");
//var videoSocket = new WebSocket(uri, "h264");
//videoPlayer.connect(videoSocket);
videoPlayer.connect(uri);

var container, camera, controls, stats, gui, joystick;
var resolution = new THREE.Vector2();
var scene = new THREE.Scene();

var renderer = new THREE.WebGLRenderer({antialias: true, logarithmicDepthBuffer: false, alpha: true});

var robotContext = {
	position : new THREE.Vector3(),
	destination : new THREE.Vector3(),
	velocity : new THREE.Vector2(),
	joints : [],
	feet : [],
	feetTargets : [],
};

var robotControls = {
	velocity : [0, 0],
	rotation : 0.0,
	height : 40.0,
	pose : false,
};

var bodyGroup;
var terrain;

var gridSpacing = 25;
var gridCount = 20;

init();

function init() {

	container = document.createElement("div");
	document.body.appendChild(container);

	camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 20000);
	camera.up = new THREE.Vector3(0, 0, 1);
	camera.position.set(0, -250, 150);
	camera.lookAt(new THREE.Vector3(0, 0, 0));
	//controls = new THREE.OrbitControls( camera, renderer.domElement );
	joystick = new VirtualJoystick({container : container, mouseSupport : true, limitStickTravel : true, stickRadius : 150});

	window.addEventListener('resize', onWindowResize, false);
	onWindowResize();

	container.appendChild(renderer.domElement);
	videoCanvas.style.width = "100%";
	//videoCanvas.style.height = "auto";
	container.appendChild(videoCanvas);
	stats = new Stats();
	container.appendChild(stats.dom);

	scene.add(generateFloor());

	render();
}

function initGUI() {
	if (gui != null)
		gui.destroy();
	gui = new dat.GUI();

	var servos = gui.addFolder("Servos");
	robotContext.joints.forEach(function(val, i) {
		servos.add(robotContext.joints, i, 0, 1).listen();
	});

	gui.add(robotControls, "height", 0.0, 80.0);
	gui.add(robotControls, "pose");

	setInterval(function(){
		if (socket.readyState == WebSocket.OPEN) {
			if (joystick) {
				robotControls.velocity[1] = Double(-joystick.deltaY() / joystick._stickRadius);
				robotControls.rotation = Double(-joystick.deltaX() / joystick._stickRadius);
			}
			robotControls.height = Double(robotControls.height.valueOf());
			//network.send("controls", robotControls);
		}
	}, 50);
}

function generateFloor() {

	geometry = new THREE.Geometry();
	for (var i = -gridCount; i <= gridCount; i++) {

		var u = gridSpacing * i;
		var v = gridSpacing * gridCount;
		geometry.vertices.push(new THREE.Vector3(u, v, 0));
		geometry.vertices.push(new THREE.Vector3(u, -v, 0));

		geometry.vertices.push(new THREE.Vector3(v, u, 0));
		geometry.vertices.push(new THREE.Vector3(-v, u, 0));
	}

	material = new THREE.LineBasicMaterial({color: 0x003f7f});
	var floor = new THREE.LineSegments(geometry, material);
	floor.name = "floor";

	return floor;
}

//////  requests  //////

// request for the robot layout to be sent
function requestBody() {
	//network.send("body", "");
}

// request a full version of the map
function requestWorldMap() {
	//network.send("worldMap", "");
}

// tell the robot to move to a certain location
function setDestination(destination) {
	//network.send("destination", destination);
}

//////  responses  //////

// create the model based on the layout information given here
function onGetBody(body) {

	robotContext.body = body;
	robotContext.joints = [];
	body.legs.forEach(function() {
		robotContext.joints.push(0.5);
		robotContext.joints.push(0.5);
		robotContext.joints.push(0.5);
	});
	initGUI();

	var index = scene.children.indexOf(bodyGroup);
	if (index > -1)
		scene.children.splice(index, 1);
	bodyGroup = getBodyGroup(body);
	scene.add(bodyGroup);
}

// start requesting all chunks relative to body location
function onGetWorldMap(map) {

}

// somehow determine if it is worth updating the current map
// network saturation or some shit
function onWorldChange(location) {

}

// update the view with new data
function onGetWorldChunk(chunk) {

}

// update the model with robot state
function onGetDynamic(dynamic) {
	
	if (robotContext.body == null)
		return;

	robotContext.body.orientation = dynamic.orientation;

	robotContext.position.set(dynamic.position[0], dynamic.position[1], dynamic.position[2]);

	robotContext.destination.set(dynamic.destination[0], dynamic.destination[1], dynamic.destination[2]);

	robotContext.velocity.set(dynamic.velocity[0], dynamic.velocity[1]);

	dynamic.joints.forEach(function(joint, i) {
		robotContext.joints[i] = joint;
	});

	dynamic.feet.forEach(function(foot, i) {
		robotContext.feet[i] = new THREE.Vector3(foot[0], foot[1], foot[2]);
	});

	dynamic.feetTargets.forEach(function(target, i) {
		robotContext.feetTargets[i] = new THREE.Vector3(target[0], target[1], target[2]);
	});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function makeLine(geometry, material) {
	var line = new MeshLine();
	line.setGeometry(geometry);
	var meshMaterial = new MeshLineMaterial( {
		color: material.color,
		resolution: resolution,
		sizeAttenuation: !true,
		lineWidth: material.linewidth,
		near: camera.near,
		far: camera.far
	});
	return new THREE.Mesh(line.geometry, meshMaterial);
}

function getBodyGroup(body) {

	var bodyGroup = new THREE.Group();
	bodyGroup.matrixAutoUpdate = false;
	bodyGroup.add(camera);

	var cubeGeometry = new THREE.BoxGeometry(5, 5, 5);
	var geometry;
	var material;

	// position
	geometry = new THREE.Geometry()
	geometry.vertices.push(new THREE.Vector3(0, 0, 0));
	material = new THREE.PointsMaterial({color: 0xffff00});
	var position = new THREE.Points(geometry, material);
	position.name = "position";
	scene.add(position);

	// destination
	material = new THREE.MeshBasicMaterial({color: 0xffffff});
	var destination = new THREE.Mesh(cubeGeometry, material);
	destination.name = "destination";
	scene.add(destination);

	// velocity
	geometry = new THREE.Geometry()
	geometry.vertices.push(new THREE.Vector3(0, 0, 0));
	geometry.vertices.push(new THREE.Vector3(1, 0, 0));
	material = new THREE.LineBasicMaterial({color: 0xffffff, linewidth: 5});
	var velocity = makeLine(geometry, material);
	velocity.name = "velocity";
	velocity.matrixAutoUpdate = false;
	scene.add(velocity);

	// body
	body.legs.forEach(function(leg, index) {

		// base
		geometry = new THREE.Geometry();
		geometry.vertices.push(new THREE.Vector3(0, 0, 0));
		geometry.vertices.push(new THREE.Vector3(leg.offset[0], leg.offset[1], leg.offset[2]));
		material = new THREE.LineBasicMaterial({color: 0xffff00, linewidth: 5});
		var base = makeLine(geometry, material);
		base.name = "base" + index;
		base.matrixAutoUpdate = false;
		bodyGroup.add(base);

		// coxa
		geometry = new THREE.Geometry();
		geometry.vertices.push(new THREE.Vector3(0, 0, 0));
		geometry.vertices.push(new THREE.Vector3(leg.coxa.length, 0, 0));
		material = new THREE.LineBasicMaterial({color: 0xff7f00, linewidth: 5});
		var coxa = makeLine(geometry, material);
		coxa.name = "coxa";
		coxa.matrixAutoUpdate = false;
		base.add(coxa);

		// femur
		geometry = new THREE.Geometry();
		geometry.vertices.push(new THREE.Vector3(0, 0, 0));
		geometry.vertices.push(new THREE.Vector3(leg.femur.length, 0, 0));
		material = new THREE.LineBasicMaterial({color: 0xff007f, linewidth: 5});
		var femur = makeLine(geometry, material);
		femur.name = "femur";
		femur.matrixAutoUpdate = false;
		coxa.add(femur);

		// tibia
		geometry = new THREE.Geometry();
		geometry.vertices.push(new THREE.Vector3(0, 0, 0));
		geometry.vertices.push(new THREE.Vector3(leg.tibia.length, 0, 0));
		material = new THREE.LineBasicMaterial({color: 0x7f007f, linewidth: 5});
		var tibia = makeLine(geometry, material);
		tibia.name = "tibia";
		tibia.matrixAutoUpdate = false;
		femur.add(tibia);

		// foot
		material = new THREE.MeshBasicMaterial({color: 0x7f007f});
		var foot = new THREE.Mesh(cubeGeometry, material);
		foot.name = "foot" + index;
		scene.add(foot);

		// footTarget
		material = new THREE.MeshBasicMaterial({color: 0x007fff});
		var target = new THREE.Mesh(cubeGeometry, material);
		target.name = "target" + index;
		scene.add(target);
	});
	
	return bodyGroup;
}

function animateLegs(context, bodyGroup) {

	if (context.body == null) return;
	if (context.joints.length == 0) return;
	if (context.feet.length == 0) return;
	if (context.feetTargets.length == 0) return;

	bodyGroup.matrix.elements = context.body.orientation;

	var floor = scene.getObjectByName("floor");
	floor.position.x = mod(-context.position.x, gridSpacing);
	floor.position.y = mod(-context.position.y, gridSpacing);

	var matrix = new THREE.Matrix4();

	var velocity = scene.getObjectByName("velocity");
	var angle = Math.atan2(context.velocity.y, context.velocity.x);
	var length = context.velocity.length();
	velocity.matrix.identity();
	velocity.matrix.scale(new THREE.Vector3(length, length, length));
	matrix.makeRotationZ(angle);
	velocity.matrix.multiply(matrix);
	
	context.body.legs.forEach(function(leg, index) {
		var base = bodyGroup.getObjectByName("base" + index);
		var coxa = base.children[0];
		var coxaAngle = leg.coxa.angle - (context.joints[index * 3] - 0.5) * leg.coxa.range;
		var femur = coxa.children[0];
		var femurAngle = leg.femur.angle - (context.joints[index * 3 + 1] - 0.5) * leg.femur.range;
		var tibia = femur.children[0];
		var tibiaAngle = leg.tibia.angle - (context.joints[index * 3 + 2] - 0.5) * leg.tibia.range;

		// base
		//matrix.makeTranslation(0, 0, 40);
		//base.matrix.identity();
		//base.matrix.multiply(matrix);

		// coxa
		var axis = new THREE.Vector3(0, 0, 1);
		matrix.makeTranslation(leg.offset[0], leg.offset[1], leg.offset[2]);
		coxa.matrix.identity();
		coxa.matrix.multiply(matrix);
		matrix.makeRotationAxis(axis, coxaAngle);
		coxa.matrix.multiply(matrix);

		// femur
		axis = new THREE.Vector3(0, -1, 0);
		matrix.makeTranslation(leg.coxa.length, 0, 0);
		femur.matrix.identity();
		femur.matrix.multiply(matrix);
		matrix.makeRotationAxis(axis, femurAngle);
		femur.matrix.multiply(matrix);

		// tibia
		matrix.makeTranslation(leg.femur.length, 0, 0);
		tibia.matrix.identity();
		tibia.matrix.multiply(matrix);
		matrix.makeRotationAxis(axis, tibiaAngle);
		tibia.matrix.multiply(matrix);

		var foot = scene.getObjectByName("foot" + index);
		var target = scene.getObjectByName("target" + index);

		var pos = context.feet[index];
		foot.position.set(pos.x, pos.y, pos.z);
		pos = context.feetTargets[index]
		target.position.set(pos.x, pos.y, pos.z)
	});
}

function render() {
	animateLegs(robotContext, bodyGroup);
	requestAnimationFrame(render);
	renderer.render(scene, camera);
	stats.update();
}

function onWindowResize() {
	var w = window.innerWidth;
	var h = window.innerHeight / 2;
	resolution.set(w, h);
	camera.aspect = w / h;
	camera.updateProjectionMatrix();
	renderer.setSize(w, h);
}

function mod(n, m) {
        return ((n % m) + m) % m;
}
