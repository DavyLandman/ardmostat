$("#updateTemperatureInfo").click(function (e) {
	e.preventDefault();
	$.get("/Temperature/Range/" + new Date(2011, 11, 29).toJSON() + "/"+ new Date(2011,11,31).toJSON(), function (data) {
		alert(data.length);
	});
	return false;
});
