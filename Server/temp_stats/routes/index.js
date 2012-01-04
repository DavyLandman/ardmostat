exports.index = function(req, res){
  res.render('index', { title: 'ArdMostat - Arduino Internet Thermostat' })
};

exports.temperature = function(req, res) {
	res.render('temperature', { title: 'Temperature information', pagescript: '/javascripts/temperature.js'})
};


exports.temperatureRange = function(req, res) {
	res.send({
		temp : 2132.20,
		occur: new Date(req.params.start).toString()
	});
};

