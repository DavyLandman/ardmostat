exports.init = function (app) {
	app.get('/Temperature', function(req, res) {
		res.render('temperature', { 
			title: 'Temperature information', 
			pagescript: '/javascripts/temperature.js'
		});
	});

	app.get('/Temperature/Range/:start', function(req, res) {
		if (req.xhr) {
			res.json({
				temp : 2132.20,
				occur: new Date(req.params.start).toString()
			});
		} 
		else {
			res.send('This method is only intended for ajax calls', 500);
		}
	});
}
