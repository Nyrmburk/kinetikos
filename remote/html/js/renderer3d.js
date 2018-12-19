class Renderer3d {
	constructor(container, robot) {
		if (!WEBGL.isWebGLAvailable()) {
			var warning = WEBGL.getWebGLErrorMessage();
			// document.getElementById('container').appendChild(warning);
			alert("WebGL not available");
			throw new Error("WebGL not available");
		}

		this.container = container;
		this.robot = robot;
		this.robotDisplay;

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

		this.scene.add(this.generateFloor(20, 25));

		this.onResize()

		//this.initGUI();
	}

	initCamera() {
		this.camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 20000);
		this.camera.up = new THREE.Vector3(0, 0, 1);
		this.camera.position.set(0, -250, 150);
		this.camera.lookAt(new THREE.Vector3(0, 0, 0));
	}

	initGUI() {
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

	generateFloor(gridCount, gridSpacing) {
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

		floor.gridCount = gridCount;
		floor.gridSpacing = gridSpacing;

		return floor;
	}

	setRobot(robot) {
		this.robot = robot;

		if (this.robotDisplay) {
			this.scene.remove(this.robotDisplay);
		}
		this.robotDisplay = this.getRobotDisplay(robot);
		this.robotDisplay.add(this.camera);
		this.scene.add(this.robotDisplay);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

	makeLine(geometry, material) {
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

	getPositionDisplay() {
		var geometry = new THREE.Geometry()
		geometry.vertices.push(new THREE.Vector3(0, 0, 0));
		var material = new THREE.PointsMaterial({color: 0xffff00});
		var position = new THREE.Points(geometry, material);
		position.name = "position";
		return position;
	}

	getDestinationDisplay() {
		var material = new THREE.MeshBasicMaterial({color: 0xffffff});
		var destination = new THREE.Mesh(cubeGeometry, material);
		destination.name = "destination";
		return destination;
	}

	getVelocityDisplay() {
		geometry = new THREE.Geometry()
		geometry.vertices.push(new THREE.Vector3(0, 0, 0));
		geometry.vertices.push(new THREE.Vector3(1, 0, 0));
		var material = new THREE.LineBasicMaterial({color: 0xffffff, linewidth: 5});
		var velocity = this.makeLine(geometry, material);
		velocity.name = "velocity";
		velocity.matrixAutoUpdate = false;
		return velocity;
	}

	getBodyDisplay(body) {
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

	getLegDisplay(leg, index) {
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

	getBoneDisplay(bone, color) {
		var geometry = new THREE.Geometry();
		geometry.vertices.push(new THREE.Vector3(0, 0, 0));
		geometry.vertices.push(new THREE.Vector3(bone.length, 0, 0));
		var material = new THREE.LineBasicMaterial({color: color, linewidth: 5});
		var bone = this.makeLine(geometry, material);
		bone.matrixAutoUpdate = false;
		return bone;
	}

	getCoxaDisplay(bone) {
		var coxa = this.getBoneDisplay(bone, 0xff7f00);
		coxa.name = "femur";
		return coxa;
	}

	getFemurDisplay(bone) {
		var femur = this.getBoneDisplay(bone, 0xff007f);
		femur.name = "femur";
		return femur;
	}

	getTibiaDisplay(bone) {
		var tibia = this.getBoneDisplay(bone, 0x7f007f);
		tibia.name = "tibia";
		return tibia;
	}

	getFootDisplay(foot, index, color) {
		var cubeGeometry = new THREE.BoxGeometry(5, 5, 5);
		var material = new THREE.MeshBasicMaterial({color: color});
		var foot = new THREE.Mesh(cubeGeometry, material);
		foot.name = "foot" + index;
		return foot;
	}

	getFootTargetDisplay(target, index) {
		var cubeGeometry = new THREE.BoxGeometry(5, 5, 5);
		var material = new THREE.MeshBasicMaterial({color: 0x007fff});
		var target = new THREE.Mesh(cubeGeometry, material);
		target.name = "target" + index;
		return target;
	}

	getFootPathDisplay(path) {
	}

	getRobotDisplay(robot) {

		// this doesn't work because when the robot moves, then so does literally everything else.
		// that's not right.
		// I need a worldDisplay
		var robotDisplay = new THREE.Group();
		robotDisplay.matrixAutoUpdate = false;
		//robotDisplay.add(getPositionDisplay());
		//robotDisplay.add(getDestinationDisplay());
		//robotDisplay.add(getVelocityDisplay());

		robotDisplay.add(this.getBodyDisplay(robot.body));

		for (var i = 0; i < robot.body.legs.length; i++)  {
			robotDisplay.add(this.getFootDisplay(robot.feet[i], i, 0x7f007f));
		}

		return robotDisplay;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	animateBody(robot, bodyGroup) {
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

		bodyGroup.matrix.copy(robot.bodyOrientation);
	}

	animateRobot(robot, robotDisplay) {
		var bodyGroup = robotDisplay.getObjectByName("body");
		this.animateBody(robot, bodyGroup);

		for (var i = 0; i < robot.feet.length; i++) {
			var foot = robotDisplay.getObjectByName("foot" + i);
			foot.position.copy(robot.feet[i]);
		}

		robotDisplay.matrix.copy(robot.orientation);
	}

	render() {
		if (this.robotDisplay) {
			this.animateRobot(this.robot, this.robotDisplay);
			var floor = this.scene.getObjectByName("floor");
			var position = new THREE.Vector3();
			position.setFromMatrixPosition(this.robotDisplay.matrix);
			position.x = position.x % floor.gridSpacing;
			position.y = position.y % floor.gridSpacing;
			this.robotDisplay.matrix.setPosition(position);

		}
		this.renderer.render(this.scene, this.camera);
		this.stats.update();
	}

	onResize() {
		var w = this.container.offsetWidth;
		var h = this.container.offsetHeight;
		this.resolution.set(w, h);
		this.camera.aspect = w / h;
		this.camera.updateProjectionMatrix();
		this.renderer.setSize(w, h);
	}
}

function mod(n, m) {
        return ((n % m) + m) % m;
}

