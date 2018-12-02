var parameters = {
	body:        100
}

var control = {
	joints:      200,
	feet:        201,
	footPaths:   202,
	velocity:    203,
	navigation:  204,
	destination: 205
}

RobotProtocol.prototype = Object.create(Protocol.prototype);
function RobotProtocol(robot, renderer) {
	Protocol.call(this);
	this.robot = robot;
	this.renderer = renderer;
}

RobotProtocol.prototype.handle = function(controlCode, dataIn, dataOut) {
	switch (controlCode) {
		case control.joints: return this.controlJoints(dataIn, dataOut);
		case control.feet: return this.controlFeet(dataIn, dataOut);
		case control.footPaths: return this.controlFootPaths(dataIn, dataOut);
		case control.velocity: return this.controlVelocity(dataIn, dataOut);
		case control.navigation: return this.controlNavigationPath(dataIn, dataOut);
		case control.destination: return this.controlDestination(dataIn, dataOut);
		case parameters.body: return this.onGetBody(dataIn, dataOut);
	}
}

RobotProtocol.prototype.onopen = function(evt) {
	console.log("socket opened");
	this.requestBody();
}

RobotProtocol.prototype.onclose = function(evt) {
	console.log("socket closed");
}

RobotProtocol.prototype.controlJoints = function(dataIn, dataOut) {
	// hey!
}


// request for the robot layout to be sent
RobotProtocol.prototype.requestBody = function() {
	this.send(parameters.body);
}

// create the model based on the layout information given here
RobotProtocol.prototype.onGetBody = function(dataIn, dataOut) {
	this.robot.body.deserialize(dataIn);
	renderer.setRobot(this.robot);
}

// request a full version of the map
function requestWorldMap() {
	//network.send("worldMap", "");
}

// tell the robot to move to a certain location
function setDestination(destination) {
	//network.send("destination", destination);
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

