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
			provideGraphData(newData);
		}
	});
	return false;
});


var graph;
function provideGraphData(data) {
	if (typeof(graph) === 'undefined') {
		initializeGraph(data);
	}
	else {
		graph.updateOptions({'file': data});
	}
}

function initializeGraph(data) {
	graph = new Dygraph($("#tempGraph").get(0), data, {
		drawPoints: true,
		rollPeriod: 10,
		showRoller: true,
		labels: ['Occurance', 'Temperature']
	});
}
