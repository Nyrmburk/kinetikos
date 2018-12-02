function Robot() {
	this.orientation = new THREE.Matrix4();
	this.body = new Body([]);

	this.joints = [];
	this.feet = [];
}

function Body(legs) {
	this.legs = legs;
}

Body.prototype.serialize = function(data) {
	data.setUint8(legs.length);
	for (var i = 0; i < legs.length; i++) {
		legs[i].serialize(data);
	}
}

Body.prototype.deserialize = function(data) {
	var length = data.getUint8();
	for (var i = 0; i < length; i++) {
		var leg = new Leg(); 
		leg.deserialize(data);
		this.legs[i] = leg;
	}
}

function Leg(offset = [], coxa, femur, tibia) {
	this.offset = offset;
	this.coxa = (coxa === undefined) ? new Bone() : coxa;
	this.femur = (femur === undefined) ? new Bone() : femur;
	this.tibia = (tibia === undefined) ? new Bone() : tibia;
}

Leg.prototype.serialize = function(data) {
	data.setFloat32(this.offset[0]);
	data.setFloat32(this.offset[1]);
	data.setFloat32(this.offset[2]);
	this.coxa.serialize(data);
	this.femur.serialize(data);
	this.tibia.serialize(data);
}

Leg.prototype.deserialize = function(data) {
	this.offset[0] = data.getFloat32();
	this.offset[1] = data.getFloat32();
	this.offset[2] = data.getFloat32();
	this.coxa.deserialize(data);
	this.femur.deserialize(data);
	this.tibia.deserialize(data);
}

function Bone(length = 0, angle = 0, range = 0) {
	this.length = length;
	this.angle = angle;
	this.range = range;
}

Bone.prototype.serialize = function(data) {
	data.setFloat32(this.length);
	data.setFloat32(this.angle);
	data.setFloat32(this.range);
}

Bone.prototype.deserialize = function(data) {
	this.length = data.getFloat32();
	this.angle = data.getFloat32();
	this.range = data.getFloat32();
}
