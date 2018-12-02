function DataView2(buffer, byteOffset, byteLength, index = 0) {
	this.view = new DataView(buffer, byteOffset, byteLength);
	this.buffer = buffer;
	this.byteOffset = this.view.byteOffset;
	this.byteLength = this.view.byteLength;
	this.index = index;
	this.mark = 0;
}

DataView2.prototype.getIndex = function() {
	return this.index;
}

DataView2.prototype.setIndex = function(index) {
	this.index = index;
}

DataView2.prototype.rewind = function() {
	this.index = 0;
}

DataView2.prototype.mark = function(index) {
	this.mark = (index === undefined) ? this.index : index;
}

DataView2.prototype.reset = function() {	
	this.index = this.mark;
}

DataView2.prototype.getInt8 = function(index) {
	if (index !== undefined)
		return this.view.getInt8(index);
	var value = this.view.getInt8(this.index);
	this.index += 1;
	return value;
}

DataView2.prototype.getUint8 = function(index) {
	if (index !== undefined)
		return this.view.getUint8(index);
	var value = this.view.getUint8(this.index);
	this.index += 1;
	return value;
}

DataView2.prototype.getInt16 = function(index) {
	if (index !== undefined)
		return this.view.getInt16(index);
	var value = this.view.getInt16(this.index);
	this.index += 2;
	return value;
}

DataView2.prototype.getUint16 = function(index) {
	if (index !== undefined)
		return this.view.getUint16(index);
	var value = this.view.getUint16(this.index);
	this.index += 2;
	return value;
}

DataView2.prototype.getInt32 = function(index) {
	if (index !== undefined)
		return this.view.getInt32(index);
	var value = this.view.getInt32(this.index);
	this.index += 4;
	return value;
}

DataView2.prototype.getUint32 = function(index) {
	if (index !== undefined)
		return this.view.getUint32(index);
	var value = this.view.getUint32(this.index);
	this.index += 4;
	return value;
}

DataView2.prototype.getFloat32 = function(index) {
	if (index !== undefined)
		return this.view.getFloat32(index);
	var value = this.view.getFloat32(this.index);
	this.index += 4;
	return value;
}

DataView2.prototype.getFloat64 = function(index) {
	if (index !== undefined)
		return this.view.getFloat64(index);
	var value = this.view.getFloat64(this.index);
	this.index += 8;
	return value;
}

DataView2.prototype.setInt8 = function(value) {
	var value = this.view.setInt8(this.index, value);
	this.index += 1;
	return value;
}

DataView2.prototype.setUint8 = function(value) {
	var value = this.view.setUint8(this.index, value);
	this.index += 1;
	return value;
}

DataView2.prototype.setInt16 = function(value) {
	var value = this.view.setInt16(this.index, value);
	this.index += 2;
	return value;
}

DataView2.prototype.setUint16 = function(value) {
	var value = this.view.setUint16(this.index, value);
	this.index += 2;
	return value;
}

DataView2.prototype.setInt32 = function(value) {
	var value = this.view.setInt32(this.index, value);
	this.index += 4;
	return value;
}

DataView2.prototype.setUint32 = function(value) {
	var value = this.view.setUint32(this.index, value);
	this.index += 4;
	return value;
}

DataView2.prototype.setFloat32 = function(value) {
	var value = this.view.setFloat32(this.index, value);
	this.index += 4;
	return value;
}

DataView2.prototype.setFloat64 = function(value) {
	var value = this.view.setFloat64(this.index, value);
	this.index += 8;
	return value;
}
