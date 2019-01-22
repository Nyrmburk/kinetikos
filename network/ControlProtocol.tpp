template <typename T>
void RobotProtocol<T>::controlBodyOrientation(T& remote, uint16_t opcode, DataView& data) {
}

template <typename T>
void RobotProtocol<T>::controlJoints(T& remote, uint16_t opcode, DataView& data) {
}

template <typename T>
void RobotProtocol<T>::controlFeet(T& remote, uint16_t opcode, DataView& data) {
}

template <typename T>
void RobotProtocol<T>::controlFootPaths(T& remote, uint16_t opcode, DataView& data) {
}

template <typename T>
void RobotProtocol<T>::controlOrientation(T& remote, uint16_t opcode, DataView& data) {
}

template <typename T>
void RobotProtocol<T>::controlVelocity(T& remote, uint16_t opcode, DataView& data) {
}

template <typename T>
void RobotProtocol<T>::controlNavigationPath(T& remote, uint16_t opcode, DataView& data) {
}

template <typename T>
void RobotProtocol<T>::controlDestination(T& remote, uint16_t opcode, DataView& data) {
}

template <typename T>
void RobotProtocol<T>::controlJoystick(T& remote, uint16_t opcode, DataView& data) {
	if (joystickPlan) {
		RobotUserData* userData = (RobotUserData*) this->getUserData(remote);
		if (!userData->joysticks) {
			userData->joysticks = new JoystickPlan::JoystickPair();
			joystickPlan->addChannel(userData->joysticks);
		}
		JoystickPlan::JoystickPair* joysticks = userData->joysticks;
		deserializev2(&data, &joysticks->xy);
		deserializev2(&data, &joysticks->uv);
	}
}

