var graph;

$("#updateTemperatureInfo").click(function (e) {
	e.preventDefault();
	$.get("/Temperature/Range/" + new Date(2011, 11, 30).toJSON() + "/"+ new Date(2011,11,31).toJSON(), function (data) {
		if (typeof(data) === 'string') {
			alert(data);
		}
		else {
			var newData = [];
			for (var i = 0, d; d = data[i++];) {
				newData.push([new Date(d.occurance), d.temperature]);
			}
			graph.updateOptions({'file': newData});
		}
	});
	return false;
});

$(function() {
	graph = new Dygraph($("#tempGraph").get(0), [[new Date(),20]], {
		drawPoints: true,
		rollPeriod: 10,
		showRoller: true,
		labels: ['Temperature', 'Occurance']
	});
});
