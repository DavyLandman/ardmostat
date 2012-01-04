$("#updateTemperatureInfo").click(function (e) {
	e.preventDefault();
	$.get("/Temperature/Range/" + new Date().toJSON(), function (data) {
		alert(data.temp);
	});
	return false;
});
