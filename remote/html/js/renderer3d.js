function Renderer3d(container, robot) {
	if (!WEBGL.isWebGLAvailable()) {
    	var warning = WEBGL.getWebGLErrorMessage();
	    // document.getElementById('container').appendChild(warning);
		alert("WebGL not available");
		throw new Error("WebGL not available");
	}

	this.container = container;
	this.robot = robot;
	this.bodyDisplay;

	var controls, gui;

	this.resolution = new THREE.Vector2();
	this.scene = new THREE.Scene();

	this.renderer = new THREE.WebGLRenderer({
			antialias: true, 
			logarithmicDepthBuffer: false, 
			alpha: true});

	this.initCamera();

	var robotControls = {
		velocity : [0, 0],
		rotation : 0.0,
		height : 40.0,
		pose : false,
	};

	var terrain;

	this.container.appendChild(this.renderer.domElement);
	this.stats = new Stats();
	this.container.appendChild(this.stats.dom);

	this.scene.add(generateFloor(20, 25));
	
	this.onResize()
	
	//this.initGUI();
}

Renderer3d.prototype.initCamera = function() {
	this.camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 20000);
	this.camera.up = new THREE.Vector3(0, 0, 1);
	this.camera.position.set(0, -250, 150);
	this.camera.lookAt(new THREE.Vector3(0, 0, 0));
}

Renderer3d.prototype.initGUI = function() {
	if (gui != null)
		gui.destroy();
	gui = new dat.GUI();

	var servos = gui.addFolder("Servos");
	this.robotContext.joints.forEach(function(val, i) {
		servos.add(this.robotContext.joints, i, 0, 1).listen();
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

function generateFloor(gridCount, gridSpacing) {

	var geometry = new THREE.Geometry();
	for (var i = -gridCount; i <= gridCount; i++) {

		var u = gridSpacing * i;
		var v = gridSpacing * gridCount;
		geometry.vertices.push(new THREE.Vector3(u, v, 0));
		geometry.vertices.push(new THREE.Vector3(u, -v, 0));

		geometry.vertices.push(new THREE.Vector3(v, u, 0));
		geometry.vertices.push(new THREE.Vector3(-v, u, 0));
	}

	var material = new THREE.LineBasicMaterial({color: 0x003f7f});
	var floor = new THREE.LineSegments(geometry, material);
	floor.name = "floor";

	return floor;
}

Renderer3d.prototype.setRobot = function(robot) {
	this.robot = robot;
	this.bodyDisplay = this.getBodyDisplay(robot.body);
	this.scene.add(this.bodyDisplay);

	for (var i = 0; i < robot.feet.length; i++) {
		this.scene.add(this.getFootDisplay(robot.feet[i], i, 0x7f007f));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

Renderer3d.prototype.makeLine = function(geometry, material) {
	var line = new MeshLine();
	line.setGeometry(geometry);
	var meshMaterial = new MeshLineMaterial({
		color: material.color,
		resolution: this.resolution,
		sizeAttenuation: !true,
		lineWidth: material.linewidth,
		near: this.camera.near,
		far: this.camera.far
	});
	return new THREE.Mesh(line.geometry, meshMaterial);
}

Renderer3d.prototype.getPositionDisplay = function() {
	var geometry = new THREE.Geometry()
	geometry.vertices.push(new THREE.Vector3(0, 0, 0));
	var material = new THREE.PointsMaterial({color: 0xffff00});
	var position = new THREE.Points(geometry, material);
	position.name = "position";
	return position;
}

Renderer3d.prototype.getDestinationDisplay = function() {
	var material = new THREE.MeshBasicMaterial({color: 0xffffff});
	var destination = new THREE.Mesh(cubeGeometry, material);
	destination.name = "destination";
	return destination;
}

Renderer3d.prototype.getVelocityDisplay = function() {
	geometry = new THREE.Geometry()
	geometry.vertices.push(new THREE.Vector3(0, 0, 0));
	geometry.vertices.push(new THREE.Vector3(1, 0, 0));
	var material = new THREE.LineBasicMaterial({color: 0xffffff, linewidth: 5});
	var velocity = this.makeLine(geometry, material);
	velocity.name = "velocity";
	velocity.matrixAutoUpdate = false;
	return velocity;
}

Renderer3d.prototype.getBodyDisplay = function(body) {
	var bodyGroup = new THREE.Group();
	bodyGroup.name = "body";
	bodyGroup.matrixAutoUpdate = false;

	for (var i = 0; i < body.legs.length; i++) {
		var leg = body.legs[i];
		var geometry = new THREE.Geometry();
		geometry.vertices.push(new THREE.Vector3(0, 0, 0));
		geometry.vertices.push(new THREE.Vector3(leg.offset[0], leg.offset[1], leg.offset[2]));
		var material = new THREE.LineBasicMaterial({color: 0xffff00, linewidth: 5});
		var base = this.makeLine(geometry, material);
		base.name = "base" + i;
		base.matrixAutoUpdate = false;
		bodyGroup.add(base);
	}

	for (var i = 0; i < body.legs.length; i++) {
		var leg = body.legs[i];
		bodyGroup.add(this.getLegDisplay(leg, i));
	}

	return bodyGroup;
}

Renderer3d.prototype.getLegDisplay = function(leg, index) {
	var legGroup = new THREE.Group();
	legGroup.name = "leg" + index;
	legGroup.matrixAutoUpdate = false;

	var coxa = this.getCoxaDisplay(leg.coxa);
	legGroup.add(coxa);

	var femur = this.getFemurDisplay(leg.femur);
	coxa.add(femur);

	var tibia = this.getTibiaDisplay(leg.tibia);
	femur.add(tibia);

	return legGroup;
}

Renderer3d.prototype.getBoneDisplay = function(bone, color) {
	var geometry = new THREE.Geometry();
	geometry.vertices.push(new THREE.Vector3(0, 0, 0));
	geometry.vertices.push(new THREE.Vector3(bone.length, 0, 0));
	var material = new THREE.LineBasicMaterial({color: color, linewidth: 5});
	var bone = this.makeLine(geometry, material);
	bone.matrixAutoUpdate = false;
	return bone;
}

Renderer3d.prototype.getCoxaDisplay = function(bone) {
	var coxa = this.getBoneDisplay(bone, 0xff7f00);
	coxa.name = "femur";
	return coxa;
}

Renderer3d.prototype.getFemurDisplay = function(bone) {
	var femur = this.getBoneDisplay(bone, 0xff007f);
	femur.name = "femur";
	return femur;
}

Renderer3d.prototype.getTibiaDisplay = function(bone) {
	var tibia = this.getBoneDisplay(bone, 0x7f007f);
	tibia.name = "tibia";
	return tibia;
}

Renderer3d.prototype.getFootDisplay = function(foot, index, color) {
	var cubeGeometry = new THREE.BoxGeometry(5, 5, 5);
	var material = new THREE.MeshBasicMaterial({color: color});
	var foot = new THREE.Mesh(cubeGeometry, material);
	foot.name = "foot" + index;
	return foot;
}

Renderer3d.prototype.getFootTargetDisplay = function(target, index) {
	var cubeGeometry = new THREE.BoxGeometry(5, 5, 5);
	var material = new THREE.MeshBasicMaterial({color: 0x007fff});
	var target = new THREE.Mesh(cubeGeometry, material);
	target.name = "target" + index;
	return target;
}

Renderer3d.prototype.getFootPathDisplay = function(path) {
}

Renderer3d.prototype.getrobotDisplay = function() {

	// this doesn't work because when the robot moves, then so does literally everything else.
	// that's not right.
	// I need a worldDisplay
	var robotGroup = new THREE.Group();
	robotGroup.add(getPositionDisplay());
	robotGroup.add(getDestinationDisplay());
	robotGroup.add(getVelocityDisplay());

	robotGroup.add(getBodyDisplay());

	this.robot.body.legs.forEach(function(leg, index) {
	});
	
	return bodyGroup;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

Renderer3d.prototype.animateBody = function(robot, bodyGroup) {
	var body = robot.body;
	for (var i = 0; i < body.legs.length; i++) {
		var matrix = new THREE.Matrix4();
		var legObject = bodyGroup.getObjectByName("leg" + i);

		var coxa = legObject.children[0];
		var femur = coxa.children[0];
		var tibia = femur.children[0];

		var joints = robot.joints;

		var leg = body.legs[i];
		var coxaAngle = leg.coxa.angle - (joints[i * 3] - 0.5) * leg.coxa.range;
		var femurAngle = leg.femur.angle - (joints[i * 3 + 1] - 0.5) * leg.femur.range;
		var tibiaAngle = leg.tibia.angle - (joints[i * 3 + 2] - 0.5) * leg.tibia.range;
		
		// coxa
        var axis = new THREE.Vector3(0, 0, 1); 
        matrix.makeTranslation(leg.offset[0], leg.offset[1], leg.offset[2]);
        coxa.matrix.copy(matrix);
        matrix.makeRotationAxis(axis, coxaAngle);
        coxa.matrix.multiply(matrix);

        // femur
        axis = new THREE.Vector3(0, -1, 0); 
        matrix.makeTranslation(leg.coxa.length, 0, 0); 
        femur.matrix.copy(matrix);
        matrix.makeRotationAxis(axis, femurAngle);
        femur.matrix.multiply(matrix);

        // tibia
        matrix.makeTranslation(leg.femur.length, 0, 0); 
        tibia.matrix.copy(matrix);
        matrix.makeRotationAxis(axis, tibiaAngle);
        tibia.matrix.multiply(matrix);
	}

	if (bodyGroup) {
		bodyGroup.matrix.copy(robot.orientation);
	}
}

Renderer3d.prototype.animateRobot = function(scene, robot) {
	for (var i = 0; i < robot.feet.length; i++) {
		var foot = scene.getObjectByName("foot" + i);
		foot.position.copy(robot.feet[i]);
	}
}

Renderer3d.prototype.animateLegs = function(bodyGroup) {
	// I don't want to have these checks if I can avoid them
	/*
	if (context.body == null) return;
	if (context.joints.length == 0) return;
	if (context.feet.length == 0) return;
	if (context.feetTargets.length == 0) return;
	*/

	bodyGroup.matrix.elements = this.robot.body.orientation;

	var floor = this.scene.getObjectByName("floor");
	// I don't have this coded up yet
	// todo: uncomment when position is implemented
	//floor.position.x = mod(-robot.position.x, gridSpacing);
	//floor.position.y = mod(-robot.position.y, gridSpacing);
	floor.position.x = 0;
	floor.position.y = 0;

	var matrix = new THREE.Matrix4();

	var velocity = this.scene.getObjectByName("velocity");
	var angle = 0;//Math.atan2(context.velocity.y, context.velocity.x);
	var length = 0;//context.velocity.length();
	velocity.matrix.identity();
	velocity.matrix.scale(new THREE.Vector3(length, length, length));
	matrix.makeRotationZ(angle);
	velocity.matrix.multiply(matrix);
	
	this.robot.body.legs.forEach(function(leg, index) {
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

Renderer3d.prototype.render = function() {
	this.animateBody(this.robot, this.bodyDisplay);
	this.animateRobot(this.scene, this.robot);
	this.renderer.render(this.scene, this.camera);
	this.stats.update();
}

function mod(n, m) {
        return ((n % m) + m) % m;
}

Renderer3d.prototype.onResize = function() {
	var w = this.container.offsetWidth;
	var h = this.container.offsetHeight;
	this.resolution.set(w, h);
	this.camera.aspect = w / h;
	this.camera.updateProjectionMatrix();
	this.renderer.setSize(w, h);
}

