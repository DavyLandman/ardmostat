var net = require('net');
var orm = require('orm');
var db = orm.connect('postgresql://node_temp:report@localhost/Thermostat', function( success, db) {
		if (!success) {
			console.log('Could not connect the database!\n\t%d:%s', db.number, db.message);
			return;
		}
		var Temp = db.define("temperature", {
			"occurance":  { "type": "date" },
			"temperature": { "type": "float"}
		});
		Temp.sync();

		var server = net.createServer(function(c) {
			c.on('data', function (data) {
				var dataBuf = new Buffer(data);
				if (dataBuf.length == 4) {
					var tempSend = dataBuf.readFloatLE(0);
					console.log('temperature: ' + tempSend);
					new Temp({ "occurance": new Date() , "temperature": tempSend }).save(function (err) {
						if (err) {
							console.log('Could not safe temp in db');
						}
					});
				}
				else {
					console.log('invalid message: ' + dataBuf.toString('hex') + ' size: '+ dataBuf.length);
				}
			});
		}).listen(5555);
	});
