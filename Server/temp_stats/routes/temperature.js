exports.init = function (app) {
	app.get('/Temperature', function(req, res) {
		res.render('temperature', 
			{ 
				title: 'Temperature information', 
				pagescript: '/javascripts/temperature.js'
			}
		);
	});

	app.get('/Temperature/Range/:start', function(req, res) {
		res.send(
			{
				temp : 2132.20,
				occur: new Date(req.params.start).toString()
			}
		);
	});
}
