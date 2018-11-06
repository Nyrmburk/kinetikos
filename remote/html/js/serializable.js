function Serializable() {
	Serializable.prototype.doSerialize = new Error('undefined serialize');
	Serializable.prototype.doDeserialize = new Error('undefined deserialize');
}

