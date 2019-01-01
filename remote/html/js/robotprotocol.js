var parameters = {
	body:            100,
	workspaces:      101
}

var control = {
	bodyOrientation: 200,
	joints:          201,
	feet:            202,
	footPaths:       203,
	orientation:     204,
	velocity:        205,
	navigation:      206,
	destination:     207
}

class RobotProtocol extends Protocol {
	constructor(robot, renderer) {
		super();
		this.robot = robot;
		this.renderer = renderer;
	}

	handle(opcode, data) {
		switch (opcode) {
			case control.bodyOrientation: return this.handleBodyOrientation(data);
			case control.joints: return this.handleJoints(data);
			case control.feet: return this.handleFeet(data);
			case control.footPaths: return this.handleFootPaths(data);
			case control.orientation: return this.handleOrientation(data);
			case control.velocity: return this.handleVelocity(data);
			case control.navigation: return this.handleNavigationPath(data);
			case control.destination: return this.handleDestination(data);
			case parameters.body: return this.handleBody(data);
			case parameters.workspaces: return this.handleWorkspaces(data);
		}
	}

	onopen(evt) {
		console.log("socket opened");
		this.requestBody();
		this.subscribe(this.Opcode.opSubscribe, control.bodyOrientation);
		this.subscribe(this.Opcode.opSubscribe, control.joints);
		this.subscribe(this.Opcode.opSubscribe, control.feet);
		this.subscribe(this.Opcode.opSubscribe, control.orientation);
		this.requestWorkspaces();
	}

	onclose(evt) {
		console.log("socket closed");
	}

	handleBodyOrientation(data) {
		for (var i = 0; i < 16; i++) {
			this.robot.bodyOrientation.elements[i] = data.getFloat32();
		}
	}

	handleJoints(data) {
		for (var i = 0; i < this.robot.body.legs.length * 3; i++) {
			this.robot.joints[i] = data.getFloat32();
		}
	}

	handleFeet(data) {
		for (var i = 0; i < this.robot.body.legs.length; i++) {
			this.robot.feet[i].x = data.getFloat32();
			this.robot.feet[i].y = data.getFloat32();
			this.robot.feet[i].z = data.getFloat32();
		}
	}

	handleOrientation(data) {
		for (var i = 0; i < 16; i++) {
			this.robot.orientation.elements[i] = data.getFloat32();
		}
	}

	// request for the robot layout to be sent
	requestBody() {
		this.sendSerial(parameters.body);
	}

	// create the model based on the layout information given here
	handleBody(data) {
		this.robot.body.deserialize(data);
		for (var i = 0; i < this.robot.body.legs.length; i++) {
			this.robot.feet[i] = new THREE.Vector3();
		}
		renderer.setRobot(this.robot);
	}

	// request for the robot layout to be sent
	requestWorkspaces() {
		this.sendSerial(parameters.workspaces);
	}

	// get leg workspace data
	handleWorkspaces(data) {
		var workspaces = [];
		for (var i = 0; i < this.robot.body.legs.length; i++) {
			var workspace = new Workspace();
			workspace.deserialize(data);
			workspaces.push(workspace);
		}
		renderer.setWorkspaces(workspaces);
	}
}

