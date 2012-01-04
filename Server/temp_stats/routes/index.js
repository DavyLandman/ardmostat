exports.init = function (app) {
	app.get('/', function(req, res){
	  res.render('index', { title: 'ArdMostat - Arduino Internet Thermostat' })
	});
}


