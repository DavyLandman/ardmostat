var orm = require('orm');
var Temp;
var db = orm.connect('postgresql://node_temp:report@localhost/Thermostat', function( success, db) {
	if (!success) {
		console.log('Could not connect the database!\n\t%d:%s', db.number, db.message);
		return;
	}
	Temp = db.define("temperature", {
		"occurance":  { "type": "date" },
		"temperature": { "type": "float"}
	});
	Temp.sync();
});

exports.init = function (app) {
	app.get('/Temperature', function(req, res) {
		res.render('temperature', { 
			title: 'Temperature information', 
			pagescript: ['/javascripts/temperature.js', '/javascripts/dygraph-combined.js']
		});
	});

	function ajaxRequired(req, res, next) {
		if (req.xhr) {
			next();
		}
		else {
			res.send('This method is only intended for ajax calls', 500);
		}
	}

	app.get('/Temperature/Range/:start/:stop', ajaxRequired, function(req, res) {
		if (typeof(Temp) == 'undefined') {
			console.log("Temp orm object is not defined yet?");
			res.send("Error with database connection", 500);
			return;
		}
		Temp.find({ 
				"occurance >" : new Date(req.params.start).toISOString(),
				"occurance <" : new Date(req.params.stop).toISOString()
			}, 
			function (temps) {
				if (temps != null) {
					result = [];
					for (var tIndex = 0, t; t = temps[tIndex++];) {
						result.push({ temperature: t.temperature, occurance: t.occurance});
					}
					res.json(result);
				}
				else {
					res.send("No temperatures found in this range");
				}
			}
		);
	});
}
