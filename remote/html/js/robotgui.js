class RobotGui {
	constructor() {
		var _this = this;
		this.gui = new dat.GUI();

		this.servos = this.gui.addFolder("Servos");
		this.servoControllers = [];

//		gui.add(robotControls, "height", 0.0, 80.0);
//		gui.add(robotControls, "pose");

		this.workspace = false;
		this.gui.add(this, "workspace").onChange(function() {
			var workspaceDisplay = renderer.robotDisplay
					.getObjectByName("body")
					.getObjectByName("workspace");

			if (workspaceDisplay) {
				workspaceDisplay.visible = _this.workspace;
			} else {
				client.requestWorkspaces();
			}
		});
	}

	setRobot(robot) {
		for (var i = 0; i < this.servoControllers.length; i++) {
			var controller = this.servoControllers[i];
			this.servos.remove(controller);
		}
		this.servoControllers.length = 0;
		for (var i = 0; i < robot.joints.length; i++) {
			var controller = this.servos.add(robot.joints, i, 0, 1);
			this.servoControllers.push(controller);
			controller.listen();
		}
	}
}
