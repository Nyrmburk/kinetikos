//////  stream player  //////

function StreamPlayer() {
	this.dom = document.createElement('canvas');
	this.dom.style.width = "640px";
	this.dom.style.height = "480px";
	this.dom.style.position = "fixed";
	this.dom.style.bottom = "0px";
	this.dom.width = "640";
	this.dom.height = "480";

	var ctx = this.dom.getContext("2d");
	ctx.beginPath();
	ctx.rect(0, 0, 640, 480);
	ctx.fillStyle = "green";
	ctx.fill();
}
