class Robot {
	constructor() {
		this.body = new Body([]);

		this.bodyOrientation = new THREE.Matrix4();
		this.joints = [];
		this.feet = [];
		this.orientation = new THREE.Matrix4();
	}
}

class Body {
	constructor(legs) {
		this.legs = legs;
	}

	serialize(data) {
		data.setUint8(legs.length);
		for (var i = 0; i < legs.length; i++) {
			legs[i].serialize(data);
		}
	}

	deserialize(data) {
		var length = data.getUint8();
		for (var i = 0; i < length; i++) {
			var leg = new Leg();
			leg.deserialize(data);
			this.legs[i] = leg;
		}
	}
}

class Leg {
	constructor(offset = [], coxa, femur, tibia) {
		this.offset = offset;
		this.coxa = (coxa === undefined) ? new Bone() : coxa;
		this.femur = (femur === undefined) ? new Bone() : femur;
		this.tibia = (tibia === undefined) ? new Bone() : tibia;
	}

	serialize(data) {
		data.setFloat32(this.offset[0]);
		data.setFloat32(this.offset[1]);
		data.setFloat32(this.offset[2]);
		this.coxa.serialize(data);
		this.femur.serialize(data);
		this.tibia.serialize(data);
	}

	deserialize(data) {
		this.offset[0] = data.getFloat32();
		this.offset[1] = data.getFloat32();
		this.offset[2] = data.getFloat32();
		this.coxa.deserialize(data);
		this.femur.deserialize(data);
		this.tibia.deserialize(data);
	}
}

class Bone {
	constructor(length = 0, angle = 0, range = 0) {
		this.length = length;
		this.angle = angle;
		this.range = range;
	}

	serialize(data) {
		data.setFloat32(this.length);
		data.setFloat32(this.angle);
		data.setFloat32(this.range);
	}

	deserialize(data) {
		this.length = data.getFloat32();
		this.angle = data.getFloat32();
		this.range = data.getFloat32();
	}
}

class Workspace {
	constructor(precision = 10) {
		this.precision = precision;
		this.points = [];
	}

	elementsPerSlice() {
		// 4 sices of workspace slice
		return this.precision * 4;
	}

	slices() {
		// n+1 vertical slices
		return this.precision + 1;
	}

	totalElements() {
		return this.elementsPerSlice() * this.slices();
	}

	deserialize(data) {
		this.precision = data.getInt32();
		var elements = this.totalElements();
		for (var i = 0; i < elements; i++) {
			var point = new THREE.Vector3();
			point.x = data.getFloat32();
			point.y = data.getFloat32();
			point.z = data.getFloat32();
			this.points.push(point);
		}
	}
}
