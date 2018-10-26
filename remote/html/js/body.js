
function Body(orientation, legs) {
	this.orientation = orientation;
	this.legs = legs;
}

function Leg(offset, coxa, femur, tibia) {
	this.offset = offset;
	this.coxa = coxa;
	this.femur = femur;
	this.tibia = tibia;
}

function Bone(length, angle, minAngle, maxAngle) {
	this.length = length;
	this.angle = angle;
	this.minAngle = minAngle;
	this.maxAngle = maxAngle;
}