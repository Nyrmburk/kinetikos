var parameters = {
	body:        100
}

var control = {
	joints:      200,
	feet:        201,
	footPaths:   202,
	orientation: 203,
	velocity:    204,
	navigation:  205,
	destination: 206
}

class RobotProtocol extends Protocol {
	constructor(robot, renderer) {
		super();
		this.robot = robot;
		this.renderer = renderer;
	}

	handle(opcode, data) {
		switch (opcode) {
			case control.joints: return this.controlJoints(data);
			case control.feet: return this.controlFeet(data);
			case control.footPaths: return this.controlFootPaths(data);
			case control.orientation: return this.controlOrientation(data);
			case control.velocity: return this.controlVelocity(data);
			case control.navigation: return this.controlNavigationPath(data);
			case control.destination: return this.controlDestination(data);
			case parameters.body: return this.onGetBody(data);
		}
	}

	onopen(evt) {
		console.log("socket opened");
		this.requestBody();
		this.subscribe(this.Opcode.opSubscribe, control.joints);
		this.subscribe(this.Opcode.opSubscribe, control.feet);
		this.subscribe(this.Opcode.opSubscribe, control.orientation);
	}

	onclose(evt) {
		console.log("socket closed");
	}

	subscribe(opcode, roomId) {
		var s = {serialize: function(data) {data.setUint16(roomId)}};
		this.sendSerial(opcode, s);
	}

	controlJoints(data) {
		for (var i = 0; i < this.robot.body.legs.length * 3; i++) {
			this.robot.joints[i] = data.getFloat32();
		}
	}

	controlFeet(data) {
		for (var i = 0; i < this.robot.body.legs.length; i++) {
			this.robot.feet[i].x = data.getFloat32();
			this.robot.feet[i].y = data.getFloat32();
			this.robot.feet[i].z = data.getFloat32();
		}
	}

	controlOrientation(data) {
		for (var i = 0; i < 16; i++) {
			this.robot.orientation.elements[i] = data.getFloat32();
		}
	}

	// request for the robot layout to be sent
	requestBody() {
		this.sendSerial(parameters.body);
	}

	// create the model based on the layout information given here
	onGetBody(data) {
		this.robot.body.deserialize(data);
		for (var i = 0; i < this.robot.body.legs.length; i++) {
			this.robot.feet[i] = new THREE.Vector3();
		}
		renderer.setRobot(this.robot);
	}

	// request a full version of the map
	requestWorldMap() {
		//network.send("worldMap", "");
	}

	// tell the robot to move to a certain location
	setDestination(destination) {
		//network.send("destination", destination);
	}

	// start requesting all chunks relative to body location
	onGetWorldMap(map) {

	}

	// somehow determine if it is worth updating the current map
	// network saturation or some shit
	onWorldChange(location) {

	}
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

