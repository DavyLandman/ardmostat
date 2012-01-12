$("#updateTemperatureInfo").click(function (e) {
	e.preventDefault();
	var startDate =new Date($('input[name="startDate"]').val());
	var stopDate =new Date($('input[name="stopDate"]').val());
	if (startDate > stopDate) {
		alert("The start date should be before the stop date!");
	}
	else {
		$('#noDataError').hide();
		$.get("/Temperature/Range/" + startDate.toISOString() + "/"+ stopDate.toISOString(), function (data) {
			if (typeof(data) === 'string') {
				$('#noDataError').show();
				$('#noDataError p').text(data);
			}
			else {
				var newData = [];
				for (var i = 0, d; d = data[i++];) {
					newData.push([new Date(d.occurance), d.temperature]);
				}
				provideGraphData(newData);
			}
		});
	}
	return false;
});

var oneDay = 24*60*60*1000;

function toShortDate(d) {
	if (typeof(d) !== 'Date') {
		d = new Date(d);
	}
	//return d.getDate() + "-" + (1+d.getMonth()) + "-" + d.getFullYear();
	return (1+d.getMonth()) + "/" + d.getDate() + "/" + d.getFullYear();
}

$(function() {
	$('input[name="startDate"]').val(toShortDate(new Date() - oneDay));
	$('input[name="stopDate"]').val(toShortDate(new Date() + oneDay ));
	$('#updateTemperatureInfo').click();
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
		rollPeriod: 15,
		//showRoller: true,
		labels: ['Occurance', 'Temperature']
	});
}
