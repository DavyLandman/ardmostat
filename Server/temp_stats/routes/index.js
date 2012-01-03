exports.index = function(req, res){
  res.render('index', { title: 'ArdMostat - Arduino Internet Thermostat' })
};

exports.temperature = function(req, res) {
	res.render('temperature', { title: 'Temperature information'})
};

